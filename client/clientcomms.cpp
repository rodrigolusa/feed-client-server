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
  if(pkt->type == SUCCESS){
    char* dummieport = "4002";
    //sendMessage(BACKUP_PORT,(char*)to_string(this->backup_port).c_str());
    sendMessage(BACKUP_PORT,(char*)dummieport);
    pkt = readMessage();
    if(pkt->type == SUCCESS)
      return 0;
  }
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
    //pthread_mutex_lock(&(this->reconnecting_mutex));
    //while(this->not_reconnected){//waiting for reconnect
    //  pthread_cond_wait(&(this->reconnecting_cond),&(this->reconnecting_mutex));
    return ;
    };
    return;
  }


void ClientComms::closeSocket(){
  close(this->sckt);
}

void ClientComms::init(char* username, char* hostname, int port){
  setActive(false);
  this->username = username;
  this->hostname = hostname;
  this->port = port;
  this->seqnum = 0;
  this->seqack = 0;
  this->numHigherAcks = 0;


//  pthread_mutex_init(&(this->reconnecting_mutex),NULL);
  //pthread_cond_init(&(this->reconnecting_cond), NULL);
}
