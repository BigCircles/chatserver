#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

struct addrinfo* initaddr();

int main(){
  struct addrinfo *result; 
  result = initaddr();
  return 0;
}

struct addrinfo* initaddr(){
  int s;
  struct addrinfo hints, *result;

  memset(&hints, 0, sizeof(hints));

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_protocol = 0;
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;

  s = getaddrinfo(NULL, "0000", &hints, &result);

  if (s != 0)
    printf("%s\n", "error in getaddrinfo");
  printf("%s\n", "addr returned");

  return result;
}

 
