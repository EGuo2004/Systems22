#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client
  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  printf("Creating WKP\n");
  int check = mkfifo(WKP,0644);
  if(check == -1) {
    printf("error: %s\n", strerror(errno));
    return 0;
  }

  printf("Opening WKP\n");
  int from_client = open(WKP,O_RDONLY);
  if(from_client == -1) {
    printf("error: %s\n", strerror(errno));
    return 0;
  }

  printf("Creating and Reading Into Secret Name\n");
  char* secret = calloc(HANDSHAKE_BUFFER_SIZE, sizeof(char));
  read(from_client, secret, HANDSHAKE_BUFFER_SIZE);

  printf("Removing WKP\n");
  remove(WKP);

  printf("Opening Secret\n");
  *to_client = open(secret,O_WRONLY);

  printf("Writing to ACK\n");
  write(*to_client,ACK,sizeof(ACK));

  printf("Recieving Response\n");
  char* returnval = calloc(HANDSHAKE_BUFFER_SIZE, sizeof(char));;
  read(from_client, returnval, HANDSHAKE_BUFFER_SIZE);
  printf("Response:%s\n",returnval);
  
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server
  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.
  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  printf("Creating FIFO\n");
  char *secret = calloc(HANDSHAKE_BUFFER_SIZE, sizeof(char));;
  sprintf(secret, "%d", getpid());
  mkfifo(secret, 0644);

  printf("Opening WKP\n");
  *to_server = open(WKP,O_WRONLY);

  printf("Writing to Secret\n");
  write(*to_server,secret,HANDSHAKE_BUFFER_SIZE);

  printf("Opening Secret\n");
  int from_server = open(secret, O_RDONLY);

  printf("Reading Secret\n");
  char *ack = calloc(HANDSHAKE_BUFFER_SIZE, sizeof(char));
  read(from_server, ack, HANDSHAKE_BUFFER_SIZE);

  printf("Recieved Secret Name:%s\n", ack);

  printf("Removing Secret\n");
  remove(secret);

  if (!strcmp(ack, ACK)) {
      write(*to_server, "Complete", strlen("Complete"));
  }

  return from_server;
}
