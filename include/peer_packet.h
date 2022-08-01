#include <stdio.h>
#include <sys/socket.h>

#define PEERMSGLEN 540

struct peerinfo{
  //Personal Info
  char peername[20];
  char channel[20];
  char message[540];
  // Connection Info
  int file_desc;
  struct sockaddr address;
  socklen_t address_len;
};

