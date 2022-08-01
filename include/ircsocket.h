#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>

struct addrinfo* initaddr(char* port);
void getipaddr(struct sockaddr* toconvert, char *ip4dest, int buflen);

