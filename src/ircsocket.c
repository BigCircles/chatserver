#include "../include/ircsocket.h"
#include <netdb.h>
#include <stdio.h>
#include <string.h>


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

/*
 *Synopsis: Retrieves current time at the call time and the length of the msg formatted in key:value format.

RETURN: a char* containing the key:value pairs of time and also leng of msg.
Be aware that this does need to managed with free().
 * */
char* getheaderinfo(char* message){
  //Time variables
  char* currtime;
  char* currtimeheader;
  // msglen variables
  char* msglenheader;
  char* msglen;
  msglen = malloc(sizeof(char) * 3);
  int len = 0;
  //Return variables
  char* headerstring = NULL;
  int stringlength = 0;

  time_t tm = time(NULL);
  currtime = ctime(&tm);
  printf("ctime call is: %s\n", currtime);
  currtimeheader = setheaderline("time", currtime);

  len = strlen(message);
  sprintf(msglen, "%d", len);
  msglenheader = (setheaderline("length", msglen));

  stringlength = (strlen(currtimeheader) + strlen(msglenheader));
  headerstring = malloc(sizeof(char) * stringlength+1);
  strcat(headerstring, currtimeheader);
  strcat(headerstring, msglenheader);
  return headerstring;
}

char* setheaderline(char* key, char* value){
  int size = 32;
  int index = 0;
  char* line = malloc(sizeof(char) * size);
  memset(line, 0, size);
  if (line == NULL)
    fprintf(stderr, "Set Header Line, Malloc Error");
  char* cursor = line;

  while(*key != '\0'){
    *cursor = *key;
    
    key++;
    cursor++;
    if(cursor == (line+(size-2))){
     size = size*2;
     line = realloc(line, (size));
    }
  }

 *cursor = ':';
 cursor++;
 *cursor = ' ';
 cursor++;

 while(*value != '\0'){
   *cursor = *value;

   value++;
   cursor++;
   if(cursor == (line+(size-2))){
     size = size*2;
     line = realloc(line, (size));
   }
 }
 //*cursor = '\n';
 //cursor++;
 *cursor = '\0'; 
 return line;
}


