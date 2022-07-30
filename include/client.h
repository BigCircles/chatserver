#include <stdio.h>
#include <sys/socket.h>

struct client {
char clientname[25];
char channelname[30];
char message[350];
struct sockaddr client_addr;
};
