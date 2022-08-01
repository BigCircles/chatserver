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
// Custom Headers
#include "../include/ircsocket.h"
#include "../include/peer_packet.h"

#define PORT "8080"
#define RECV_BUF_SIZE 500

void setfdset(struct timeval *time,struct peerinfo conn[], int maxsize, fd_set *set);
int createlistensocket(struct addrinfo* results);

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
  //EXPERIMENTAL
  struct peerinfo connections[10] = {0};
  // Select setup
  FD_ZERO(&fileset);
  FD_SET(readfd, &fileset);
  timev.tv_sec = 1;
  timev.tv_usec = 0;
  //Recieved msg variables 
  int bytes_sent = 0;
  
  for(int x = 0; x < 10; x++){
    memset(&connections[x].file_desc, 0, sizeof(connections[x].file_desc));
  }
  memset(&hints, 0, sizeof hints);

  if((res = initaddr(PORT)) == NULL)
    printf("Addrinfo Error: %s\n", "Something went wrong: NULL");

  readfd = createlistensocket(res);
  freeaddrinfo(res);
  
  printf("%s\n", "Server is listening for a connection...");
  //connected[0] = readfd; 
  connections[0].file_desc = readfd;

  while(1){
    setfdset(&timev, connections, 10, &fileset);

    status = select(FD_SETSIZE,&fileset, NULL, NULL, &timev);
    if(status == -1){
      fprintf(stderr, "Select error: %s\n", strerror(errno));
      continue;
    }
  
    // IF NEW CONNECTION DETECTED
    if(FD_ISSET(connections[0].file_desc, &fileset)){
      addr_size = sizeof peer_addr;
      if((recvfd = accept(readfd,(struct sockaddr *) &peer_addr,&addr_size ))< 0){
        printf("accept socket error: %s\n", strerror(errno));
        return 1;
      }
      for(int x =1; x <10;x++){
        if(connections[x].file_desc == 0){
          connections[x].file_desc = recvfd;
          connections[x].address = peer_addr;
          connections[x].address_len = addr_size;
          break;
        }
      }
      printf("%s\n", "Server Connection recieved");

      char ip4addr[16] = "";
      getipaddr(&peer_addr, ip4addr, 16);
      printf("%s\n", ip4addr);
    }
    // Check client sockets for activity
    for(int x =1; x < 10; x++){
      if(FD_ISSET(connections[x].file_desc, &fileset)){
        memset(connections[x].message, 0, sizeof(connections[x].message));
        bytes_sent = recv(connections[x].file_desc, connections[x].message,PEERMSGLEN, 0) ;
        if(bytes_sent == 0){
          connections[x].file_desc = 0;
          printf("Client %d: Connection closed\n", x);
          break;
        }
        printf("Client %d: %s\n", x, connections[x].message);
        //Tokenize msg from client (Break packet into header / msg and 
        // Parse msg from client
        // for now print msg to client for debugging
      }
    }
  }
  close(readfd);
  close (recvfd);
  for(int x=0; x < 10; x++){
   close(connections[x].file_desc); 
  }
  return 0;
}

void setfdset(struct timeval *time,struct peerinfo conn[], int maxsize, fd_set *set){
    for(int i =0; i< maxsize;i++){
      FD_SET(conn[i].file_desc, set);
    }
    time->tv_sec = 3;
    time->tv_usec = 0;
}

int createlistensocket(struct addrinfo* results){
  // Create Socket
  int listensocket = socket(AF_INET, SOCK_STREAM, 0);
  if(listensocket == -1){
    fprintf(stderr, "Socket Creation Error: %s\n", strerror(errno));
  }
  
  setsockopt(listensocket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
  setsockopt(listensocket, SOL_SOCKET, SO_REUSEPORT, &(int){1}, sizeof(int));

  if(bind(listensocket, results->ai_addr, results->ai_addrlen) == -1){
    fprintf(stderr, "Socket bind error: %s\n", strerror(errno));
  }

  if(listen(listensocket, 5) == -1)
    fprintf(stderr, "Socket bind error: %s\n", strerror(errno));

  return listensocket;
}

