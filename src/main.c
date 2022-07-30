#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT "8080"
#define RECV_BUF_SIZE 500

struct addrinfo* initaddr(char* port);

int main(){
  // Select variables
  fd_set fileset;
  struct timeval timev;
  int ret_code;
  // Socket Variables
  struct sockaddr my_addr, peer_addr;
  socklen_t addr_size;
  struct addrinfo hints, *res, *p;
  int readfd, recvfd, status;
  int connectedsize = 1;
  int connected[10];


  struct addrinfo *test;

  if((readfd = socket(AF_INET, SOCK_STREAM, 0)) <0)
    printf("socket creation error: %s\n", strerror(errno));
   
  // Select setup
  FD_ZERO(&fileset);
  FD_SET(readfd, &fileset);
  timev.tv_sec = 1;
  timev.tv_usec = 0;
  // End select setup
  //
  memset(connected, 0, sizeof(connected));
  memset(&hints, 0, sizeof hints);

  res = initaddr(PORT);
  if (res == NULL)
    printf("Addrinfo Error: %s\n", "Something went wrong: NULL");

  setsockopt(readfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
  setsockopt(readfd, SOL_SOCKET, SO_REUSEPORT, &(int){1}, sizeof(int));

  if((bind(readfd,res->ai_addr,res->ai_addrlen)< 0))
    printf("bind socket error: %s\n", strerror(errno));

  freeaddrinfo(res);

  if(listen(readfd, 3) < 0) 
    printf("listen socket error: %s\n", strerror(errno));
  
  printf("%s\n", "Server is listening for a connection...");

  char msg[RECV_BUF_SIZE];
  memset(msg,0,sizeof(msg));
  int len = strlen(msg);
  int bytes_sent = 0;

  connected[0] = readfd; 
  while(1){
    // ADD all current connections (including server)
    // to the select set
    memset(msg,0,sizeof(msg));
    for(int i =0; i< connectedsize;i++){
      FD_SET(connected[i], &fileset);
    }
    timev.tv_sec = 3;
    timev.tv_usec = 0;

    ret_code = select(FD_SETSIZE,&fileset, NULL, NULL, &timev);
    // Select Error
    if(ret_code == -1){
      printf("%s\n", strerror(errno));
      continue;
    }
    if(FD_ISSET(connected[0], &fileset)){
      addr_size = sizeof peer_addr;
      if((recvfd = accept(readfd,(struct sockaddr *) &peer_addr,&addr_size ))< 0)
        printf("accept socket error: %s\n", strerror(errno));
      connected[connectedsize] = recvfd;
      connectedsize++;


      printf("%s\n", "Server Connection recieved");

      // Get Connected client's IP
      char addrstorage[20];
      struct sockaddr_in *ip4 = (struct sockaddr_in *) &peer_addr;
      inet_ntop(AF_INET,&(ip4->sin_addr), addrstorage, sizeof(addrstorage));
      printf("addrstorage is: %s\n", addrstorage);

      connected[connectedsize] = recvfd;
      connectedsize++;
    }
    // Check client sockets for activity
    for(int x =1; x < connectedsize; x++){
      if(FD_ISSET(connected[x], &fileset)){
        // Recv msg from client
        recv(connected[1], msg,RECV_BUF_SIZE ,0);
        printf("%s\n", msg);
        //Tokenize msg from client (Break packet into header / msg and 
        // Parse msg from client
        // for now print msg to client for debugging

      }
    }
  }
  close(readfd);
  close (recvfd);
  return 0;
}

struct addrinfo* initaddr(char* port){
  int s;
  struct addrinfo hints, *result;

  result = NULL;

  memset(&hints, 0, sizeof(hints));

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  s = getaddrinfo(NULL, port, &hints, &result);
  if (s != 0)
    return NULL;

  return result;
}

 
