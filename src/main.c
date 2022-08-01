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
#include "../include/ircsocket.h"

#define PORT "8080"
#define RECV_BUF_SIZE 500

void setfdset(struct timeval *time,int conn[], int maxsize, fd_set *set);

int main(){
  // Select variables
  fd_set fileset;
  struct timeval timev;
  int ret_code;
  // Socket Variables
  struct sockaddr my_addr, peer_addr;
  socklen_t addr_size;
  struct addrinfo hints, *res;
  int readfd, recvfd, status;
  int connected[10];

  if((readfd = socket(AF_INET, SOCK_STREAM, 0)) <0)
    printf("socket creation error: %s\n", strerror(errno));
   
  // Select setup
  FD_ZERO(&fileset);
  FD_SET(readfd, &fileset);
  timev.tv_sec = 1;
  timev.tv_usec = 0;
  // End select setup
  
  memset(connected, 0, sizeof(connected));
  memset(&hints, 0, sizeof hints);

  if((res = initaddr(PORT)) == NULL)
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

    memset(msg,0,sizeof(msg));
    setfdset(&timev, connected, 10, &fileset);

    ret_code = select(FD_SETSIZE,&fileset, NULL, NULL, &timev);
    if(ret_code  == -1){
      printf("Select error: %s\n", strerror(errno));
      continue;
    }

    if(FD_ISSET(connected[0], &fileset)){
      addr_size = sizeof peer_addr;
      if((recvfd = accept(readfd,(struct sockaddr *) &peer_addr,&addr_size ))< 0)
        printf("accept socket error: %s\n", strerror(errno));

      for(int x =1; x <10;x++){
        if(connected[x] == 0){
          connected[x] = recvfd;
          break;
        }
      }
      printf("%s\n", "Server Connection recieved");

      char addrstorage[20];
      struct sockaddr_in *ip4 = (struct sockaddr_in *) &peer_addr;
      inet_ntop(AF_INET,&(ip4->sin_addr), addrstorage, sizeof(addrstorage));
      printf("addrstorage is: %s\n", addrstorage);
    }
    // Check client sockets for activity
    for(int x =1; x < 10; x++){
      if(FD_ISSET(connected[x], &fileset)){
        bytes_sent = recv(connected[x], msg,RECV_BUF_SIZE ,0);
        if(bytes_sent == 0){
          connected[x] = 0;
          break;
        }
        printf("Client %d: %s\n", x,  msg);
        //Tokenize msg from client (Break packet into header / msg and 
        // Parse msg from client
        // for now print msg to client for debugging
      }
    }
  }
  close(readfd);
  for(int x; x < 10; x++){
   close(connected[x]); 
  }
 // close (recvfd);
  return 0;
}

void setfdset(struct timeval *time,int conn[], int maxsize, fd_set *set){
    for(int i =0; i< maxsize;i++){
      FD_SET(conn[i], set);
    }
    time->tv_sec = 3;
    time->tv_usec = 0;
}

