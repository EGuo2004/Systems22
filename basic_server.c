#include "pipe_networking.h"


int main() {

  int to_client;
  int from_client;
  char line[BUFFER_SIZE];
  while(1) {
	  from_client = server_setup();
	  int f = fork();
	  if(f) {
		  //parent(server)
		  remove(WKP);
	  } else { 
		  //child(subserver)
		  to_client=server_connect(from_client);
		  printf("connected\n");
		  while(1) {
		   printf("reading\n");
		  if((read(from_client,line,sizeof(line))) == 0) {
			  break;
		  }
		  int counter;
		  printf("toupper\n");
		  for(counter = 0; counter < strlen(line); counter++) {
			  line[counter] = toupper(line[counter]);
		  }
		  printf("writing\n");
		  write(to_client, line, sizeof(line));
  	      }
      }
   }
}