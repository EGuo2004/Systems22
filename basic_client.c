#include "pipe_networking.h"


int main(int argc, char *argv[]) {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  
  char line[100];
  while(1) {
    printf("What do you want?\n");
    fgets(line, sizeof(line), stdin);
    write(to_server, line, sizeof(line));
		read(from_server,line,sizeof(line));
		printf("Here:%s\n", line);
  }
}
