#include "../include/ircsocket.h"


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

void getipaddr(struct sockaddr* toconvert, char* ip4dest, int buflen){
   // Get Connected client's IP
  int len = strlen(ip4dest);
  struct sockaddr_in *ip4 = (struct sockaddr_in *)toconvert;
  inet_ntop(AF_INET,&ip4->sin_addr, ip4dest, buflen);
}


