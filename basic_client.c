#include "pipe_networking.h"


int main(int argc, char *argv[]) {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  
  char line[100];
  while(1) {
    printf("What do you want?\n");
    printf("getting info\n");
    fgets(line, BUFFER_SIZE, stdin);
    printf("writing info\n");
    write(to_server, line, BUFFER_SIZE);
    printf("reading info\n");
		read(from_server,line,BUFFER_SIZE);
    printf("printing info\n");
		printf("Here:%s\n", line);

  }
}
