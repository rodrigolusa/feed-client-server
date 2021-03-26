#include "clientcomms.hpp"

#define PORT 4000


int ClientComms::connectToServer(char* hostname){ //tries to establish connetion to server, returns 0 if successful and -1 otherwise
  int n;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  int sockfd;
  server = gethostbyname(hostname);
  if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

  if ((this->sckt = socket(AF_INET, SOCK_STREAM, 0)) == -1)
      return -1;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
  bzero(&(serv_addr.sin_zero), 8);


if (connect(this->sckt,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
      printf("ERROR connecting\n");
      return -1;
    }
    return 0;
}


int ClientComms::login(char* username){
  sendMessage(LOGIN,username);
  packet* pkt = readMessage();
  if(pkt->type == SUCCESS)
    return 0;
  else
  {
    return -1;
  }
}


void ClientComms::closeConnection(){ //closes connection between client and server
  if(this->active)
    sendMessage(LOGOUT);
  close(this->sckt);
  exit(2);
}
/*
int main(int argc, char *argv[])
{
  if (argc < 3) {
  fprintf(stderr,"usage %s hostname\n", argv[0]);
  exit(0);
  }

  ClientComms manager;
  char *buffer = new char[256];
  int n;
  if(manager.connectToServer(argv[2]) != 0)
    cout << "Error connecting to server\n";

  if(manager.login(argv[1]) == 0){
    cout << "Login successful" << endl << "Welcome, " << argv[1];
    manager.setActive(true);
    }
  else{
    cout << "Login error, disconnect one device to continue";
    manager.closeConnection();
    return -1;
  }
  buffer[0] = 'c';
  while(buffer[0] != 'A'){
    printf("Enter the message: ");
    bzero(buffer, 256);
    fgets(buffer, 256, stdin);
    manager.sendMessage(SEND,buffer);
    manager.readMessage();
  }

	manager.closeConnection();
  return 0;
}
*/
