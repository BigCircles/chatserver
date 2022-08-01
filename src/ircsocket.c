#include "../include/ircsocket.h"
#include <string.h>
#include <sys/socket.h>


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

char* getipaddr(struct sockaddr toconvert){
   // Get Connected client's IP
   char *ip4address = NULL;
   const int SIZE = 20;
   ip4address = malloc(sizeof(char) *SIZE);
   memset(ip4address, 0, SIZE);

  struct sockaddr_in *ip4 = (struct sockaddr_in *) &toconvert;
  inet_ntop(AF_INET,&(ip4->sin_addr), ip4address, SIZE);
  printf("%s\n", ip4address);
  return ip4address;

}


