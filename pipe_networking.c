#include "pipe_networking.h"

// /*=========================
//   server_setup
//   args:
//   creates the WKP (upstream) and opens it, waiting for a
//   connection.
//   removes the WKP once a connection has been made
//   returns the file descriptor for the upstream pipe.
//   =========================*/
// int server_setup() {
//   int b;
//   int from_client;
//   char buffer[HANDSHAKE_BUFFER_SIZE];
//   printf("[server] handshake: making wkp\n");
//   b = mkfifo(WKP, 0600);
//   if ( b == -1 ) {
//     printf("mkfifo error %d: %s\n", errno, strerror(errno));
//     exit(-1);
//   }
//   //open & block
//   from_client = open(WKP, O_RDONLY, 0);
//   //remove WKP
//   remove(WKP);

//   printf("[server] handshake: removed wkp\n");
//   return from_client;
  
// }

// /*=========================
//   server_connect
//   args: int from_client
//   handles the subserver portion of the 3 way handshake
//   returns the file descriptor for the downstream pipe.
//   =========================*/
// int server_connect(int from_client) {
//     int to_client;
//     int b;
//     char buffer[HANDSHAKE_BUFFER_SIZE];

//   //read initial message
//   b = read(from_client, buffer, sizeof(buffer));
//   printf("[server] handshake received: -%s-\n", buffer);


//   to_client = open(buffer, O_WRONLY, 0);
//   //create SYN_ACK message
//   srand(time(NULL));
//   int r = rand() % HANDSHAKE_BUFFER_SIZE;
//   sprintf(buffer, "%d", r);

//   write(to_client, buffer, sizeof(buffer));
//   //rad and check ACK
//   read(from_client, buffer, sizeof(buffer));
//   int ra = atoi(buffer);
//   if (ra != r+1) {
//     printf("[server] handshake received bad ACK: -%s-\n", buffer);
//     exit(0);
//   }//bad response
//   printf("[server] handshake received: -%s-\n", buffer);

//   return from_client;
  
//   return to_client;
// }

/*=========================
  server_setup
  args:
  creates the WKP (upstream) and opens it, waiting for a
  connection.
  removes the WKP once a connection has been made
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
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

  printf("Removing WKP\n");
  remove(WKP);

  return from_client;

}

/*=========================
  server_connect
  args: int from_client
  handles the subserver portion of the 3 way handshake
  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {

  printf("Creating and Reading Into Secret Name\n");
  char* secret = calloc(HANDSHAKE_BUFFER_SIZE, sizeof(char));
  read(from_client, secret, HANDSHAKE_BUFFER_SIZE);

  printf("Opening Secret\n");
  int to_client = open(secret,O_WRONLY);

  printf("Writing to ACK\n");
  write(to_client,ACK,sizeof(ACK));

  printf("Recieving Response\n");
  char* returnval = calloc(HANDSHAKE_BUFFER_SIZE, sizeof(char));;
  read(from_client, returnval, HANDSHAKE_BUFFER_SIZE);
  printf("Response:%s\n",returnval);

  return to_client;
}

/*=========================
  server_handshake
  args: int * to_client
  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
// int server_handshake(int *to_client) {
//   printf("Creating WKP\n");
//   int check = mkfifo(WKP,0644);
//   if(check == -1) {
//     printf("error: %s\n", strerror(errno));
//     return 0;
//   }

//   printf("Opening WKP\n");
//   int from_client = open(WKP,O_RDONLY);
//   if(from_client == -1) {
//     printf("error: %s\n", strerror(errno));
//     return 0;
//   }

//   printf("Creating and Reading Into Secret Name\n");
//   char* secret = calloc(HANDSHAKE_BUFFER_SIZE, sizeof(char));
//   read(from_client, secret, HANDSHAKE_BUFFER_SIZE);

//   printf("Removing WKP\n");
//   remove(WKP);

//   printf("Opening Secret\n");
//   *to_client = open(secret,O_WRONLY);

//   printf("Writing to ACK\n");
//   write(*to_client,ACK,sizeof(ACK));

//   printf("Recieving Response\n");
//   char* returnval = calloc(HANDSHAKE_BUFFER_SIZE, sizeof(char));;
//   read(from_client, returnval, HANDSHAKE_BUFFER_SIZE);
//   printf("Response:%s\n",returnval);
  
//   return from_client;
// }


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