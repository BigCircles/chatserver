#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>

struct addrinfo* initaddr(char* port);
char* getipaddr(struct sockaddr toconvert);

