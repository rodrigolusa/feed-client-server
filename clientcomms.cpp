#include "clientcomms.hpp"

#define PORT 4000


int ClientComms::connectToServer(char* hostname){ //tries to establish connetion to server, returns 0 if successful and -1 otherwise
  int n;
  struct sockaddr_in serv_addr;
  struct hostent *server;



server = gethostbyname(hostname);
if (server == NULL) {
      fprintf(stderr,"ERROR, no such host\n");
      exit(0);
  }

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
      return -1;

serv_addr.sin_family = AF_INET;
serv_addr.sin_port = htons(PORT);
serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
bzero(&(serv_addr.sin_zero), 8);


if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
      printf("ERROR connecting\n");
      return -1;
    }
    return 0;
}

void ClientComms::closeConnection(){ //closes connection between client and server
  sendMessage("F");
  close(sockfd);
}
int ClientComms::sendMessage(char* msg){
  int n;

  n = write(sockfd, msg, strlen(msg));
  if (n < 0){
	 printf("ERROR writing to socket\n");
   return -1;
  }
  else
  return 0;

}
char* ClientComms::readMessage(){
  char* buffer = new char[256];
  int n;
  bzero(buffer,256);
  n = read(sockfd, buffer, 256);
  if (n < 0)
  cout << "ERROR reading from socket\n";
  return buffer;
}

int ClientComms::login(char* username){
  sendMessage(username);
  char* loginMsg = readMessage();
  if(loginMsg[0] == '0')
    return 0;
  else
  {
    return -1;
  }
}

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

  if(manager.login(argv[1]) == 0)
    cout << "Login successful" << endl << "Welcome, " << argv[1];
  else{
    cout << "Login error, disconnect one device to continue";
    manager.closeConnection();
    return 0;
  }
  printf("Enter the message: ");
  bzero(buffer, 256);
  fgets(buffer, 256, stdin);
  manager.sendMessage(buffer);
  cout << manager.readMessage();


	manager.closeConnection();
  return 0;
}
