#include "pipe_networking.h"


int main(int argc, char *argv[]) {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  
  char line[100];
  while(1) {
    printf("What do you want?\n");
    fgets(line, BUFFER_SIZE, stdin);
    write(to_server, line, BUFFER_SIZE);
		read(from_server,line,BUFFER_SIZE);
		printf("Here:%s\n", line);

  }
}
