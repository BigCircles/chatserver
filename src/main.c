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
#define MAXCONNECTIONS 10

void setfdset(struct timeval *time,struct peerinfo conn[], int maxsize, fd_set *set);
int createlistensocket(struct addrinfo* results);
void acceptnewconn(struct peerinfo peers[MAXCONNECTIONS]);

int main(){
  // Select variables
  fd_set fileset;
  struct timeval timev;
  // Socket Variables
  struct addrinfo hints, *res;
  int readfd, recvfd, status;
  struct peerinfo connections[MAXCONNECTIONS] = {0};
  // Select setup
  FD_ZERO(&fileset);
  //Recieved msg variables 
  int bytes_sent = 0;
  
  for(int x = 0; x < 10; x++){
    //memset(&connections[x].file_desc, 0, sizeof(connections[x].file_desc));
    memset(&connections[x].file_desc, 0, sizeof(int));
  }
  memset(&hints, 0, sizeof hints);

  if((res = initaddr(PORT)) == NULL)
    printf("Addrinfo Error: %s\n", "Something went wrong: NULL");

  readfd = createlistensocket(res);
  printf("%s\n", "Server is listening for a connection...");
  freeaddrinfo(res);
  connections[0].file_desc = readfd;

  while(1){
    setfdset(&timev, connections, 10, &fileset);

    status = select(FD_SETSIZE,&fileset, NULL, NULL, &timev);
    if(status == -1){
      fprintf(stderr, "Select error: %s\n", strerror(errno));
      continue;
    }
    //  NEW SERVER CONNECTION DETECTED 
    if(FD_ISSET(connections[0].file_desc, &fileset)){
      acceptnewconn(connections);
    }
    // CLIENT SOCKET ACTIVITY
    for(int x =1; x < 10; x++){ 
      if(FD_ISSET(connections[x].file_desc, &fileset)){
        // max msg size of client
        memset(connections[x].message, 0, 540);
        bytes_sent = recv(connections[x].file_desc, connections[x].message,MESSAGELEN, 0) ;
        if(bytes_sent == 0){
          close(connections[x].file_desc);
          connections[x].file_desc = 0;
          printf("Client %d: Connection closed\n", x);
          break;
        }
        printf("Client %d: %s\n", x, connections[x].message);

      }
    }
  }
  close(readfd);
  close(recvfd);
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

void acceptnewconn(struct peerinfo peers[MAXCONNECTIONS]){
    // IF NEW CONNECTION DETECTED
  int recvfd = 0;
  struct sockaddr peer_addr;
  socklen_t addr_size;
  addr_size = sizeof peer_addr;

    //CONNECTIONS[0] IS always host
    if((recvfd = accept(peers[0].file_desc,(struct sockaddr *) &peer_addr,&addr_size ))< 0){
      printf("accept socket error: %s\n", strerror(errno));
      return;
    }

    for(int x =1; x <MAXCONNECTIONS;x++){
      if(peers[x].file_desc == 0){
        peers[x].file_desc = recvfd;
        peers[x].address = peer_addr;
        peers[x].address_len = addr_size;
        break;
      }
        char* servfullmsg = "Connection reject, server full!";
        fprintf(stderr, "%s\n", servfullmsg);
        send(recvfd, servfullmsg, strlen(servfullmsg) ,0);
    }
    printf("%s\n", "Server Connection recieved");
    char ip4addr[16] = "";
    getipaddr(&peer_addr, ip4addr, 16);
    printf("%s\n", ip4addr);
}
