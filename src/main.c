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

struct addrinfo* initaddr(char* port);

int main(){
  // Select variables
  fd_set fileset;
  struct timeval timev;
  int retval;
  //Select Variables End
  struct addrinfo *result; 
  struct sockaddr my_addr, peer_addr;
  socklen_t addr_size;
  struct addrinfo hints, *res, *p;
  int readfd, recvfd, status;
  int connectedsize = 1;
  int connected[10];
  char ipstr[INET6_ADDRSTRLEN];

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
  hints.ai_family = AF_INET; // AF_INET or AF_INET6 to force version
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((status = getaddrinfo(NULL, PORT, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

  setsockopt(readfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
  setsockopt(readfd, SOL_SOCKET, SO_REUSEPORT, &(int){1}, sizeof(int));

  if((bind(readfd,res->ai_addr,res->ai_addrlen)< 0))
    printf("bind socket error: %s\n", strerror(errno));

  freeaddrinfo(result);

  if(listen(readfd, 3) < 0) 
    printf("listen socket error: %s\n", strerror(errno));
  
  printf("%s\n", "Server is listening for a connection...");

  char msg[100];
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

    retval = select(FD_SETSIZE,&fileset, NULL, NULL, &timev);
    if (retval == -1){
      printf("Select detected no message");
      printf("%d\n", retval);
    }
    else{
      if(FD_ISSET(connected[0], &fileset)){
        addr_size = sizeof peer_addr;
        if((recvfd = accept(readfd,(struct sockaddr *) &peer_addr,&addr_size ))< 0)
          printf("accept socket error: %s\n", strerror(errno));

        printf("%s\n", "Server Connection recieved");
        connected[connectedsize] = recvfd;
        connectedsize++;
      }else if(FD_ISSET(connected[1], &fileset)){
        recv(connected[1], msg, 100,0);
        printf("Client 1: %s\n", msg);
      }else if(FD_ISSET(connected[2], &fileset)){
        recv(connected[2], msg, 100,0);
        printf("Client 2: %s\n", msg);
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

  return result;
}

 
