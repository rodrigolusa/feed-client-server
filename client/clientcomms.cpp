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
    sendMessage(BACKUP_PORT,(char*)to_string(this->backup_port).c_str());
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
    pthread_mutex_lock(&(this->reconnecting_mutex));
    while(this->not_reconnected)//waiting for reconnect
      pthread_cond_wait(&(this->has_reconnected),&(this->reconnecting_mutex));
    pthread_mutex_unlock(&(this->reconnecting_mutex));
    return;
  }
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
  this->not_reconnected = true;
  this->not_initialized_listen = true;
  pthread_mutex_init(&reconnecting_mutex,NULL);
  pthread_cond_init(&has_reconnected, NULL);
  pthread_t new_thread;
  pthread_create(&new_thread, NULL, waitForBackupConn, this);

  pthread_mutex_lock(&(this->reconnecting_mutex));//locking thread to guarantee we have backup port once we login
  while(this->not_initialized_listen)//waiting
    pthread_cond_wait(&(this->has_reconnected),&(this->reconnecting_mutex));
  pthread_mutex_unlock(&(this->reconnecting_mutex));
}



void* waitForBackupConn(void* args){


  ClientComms* comms = (ClientComms*) args;

  pthread_mutex_lock(&(comms->reconnecting_mutex));

  comms->backup_port = DEFAULT_BACKUP_PORT;

  int socket = 0;
  socket = initListeningSocket(comms->backup_port);
  while(socket == -1){
    comms->backup_port += 1;
    socket = initListeningSocket(comms->backup_port);//if port is unavailable, try next port..
  }
  comms->not_initialized_listen = false;
  pthread_mutex_unlock(&(comms->reconnecting_mutex));
  pthread_cond_broadcast(&(comms->has_reconnected));//allowing comms to continue after backup port is set.


  comms->backup_socket = socket;
  int newsockfd = 0;
  struct sockaddr_in cli_addr;
  socklen_t clilen;
  clilen = sizeof(struct sockaddr_in);
  if ((newsockfd = accept(comms->backup_socket, (struct sockaddr *) &cli_addr, &clilen)) == -1){
    cout << " Couldnt accept backup\n";
    exit(-1);
  }
  pthread_mutex_lock(&(comms->reconnecting_mutex));

  comms->closeSocket();
  comms->setSocket(newsockfd);
  comms->login();

  comms->not_reconnected = false;
  pthread_mutex_unlock(&(comms->reconnecting_mutex));
  pthread_cond_broadcast(&(comms->has_reconnected));//allowing comms to continue after reconnected




}
