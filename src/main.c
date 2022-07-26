#include <stdio.h>

void printtutorial(char* line);

int main(){
  printtutorial("Hello Make with Included headers");
  return 0;
}

void printtutorial(char* line){
  printf("%s\n", line);
}
 
