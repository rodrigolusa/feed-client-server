#include "clientcomms.hpp"

#define PORT 4000


int ClientComms::connectToServer(){ //tries to establish connetion to server, returns 0 if successful and -1 otherwise
  int n;
  int npulses = 5;
  bool connected = false;

  while(npulses != 0 && connected == false){
  struct hostent *server;
  struct sockaddr_in serv_addr;
  server = gethostbyname(this->hostname);
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
      cout << "Could not connect to server " << endl;
      for(int i = 5; i > 0; i--){
        cout << "Trying again in " << i << " seconds" << endl;
        sleep(1);
            }
        }
      else
      connected = true;
      npulses--;
      }

  if(connected)
      return 0;
  else
    return -1;
}


int ClientComms::login(){
  sendMessage(LOGIN,this->username);
  packet* pkt = readMessage();
  if(pkt->type == SUCCESS)
    return 0;
  else
  {
    return -1;
  }
}

void ClientComms::attemptReconnect(){
  //try to connect
  int isConnected = connectToServer();
  if (isConnected != 0)
  {
      cout << "Error connecting to server\n";
      exit(2);
  }

  //try to login
  int isLogged = login();
  if (isLogged != 0)
  {
      cout << "Login error, disconnect one device to continue";
      close(this->sckt);
      exit(2);
  }
  setActive(true);
}
void ClientComms::connectionInterrupted(){ //closes connection between client and server
  if(this->active){
    sendMessage(LOGOUT);
    close(this->sckt);
    return;
    }
  else{
    attemptReconnect();
    return;
  }
}

void ClientComms::closeSocket(){
  close(this->sckt);
}

int main(int argc, char *argv[])
{
  if (argc < 3) {
  fprintf(stderr,"usage %s hostname\n", argv[0]);
  exit(0);
  }
  int port = atoi(argv[3]);
  ClientComms manager(argv[1],argv[2],port);
  char *buffer = new char[256];
  int n;
  if(manager.connectToServer() != 0)
    cout << "Error connecting to server\n";

  if(manager.login() == 0){
    cout << "Login successful" << endl << "Welcome, " << argv[1];
    manager.setActive(true);
    }
  else{
    cout << "Login error, disconnect one device to continue";
    manager.closeSocket();
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

	manager.connectionInterrupted();
  return 0;
}
