#include "basiccomm.hpp"
using namespace std;

char* getTimestamp(char* timestamp){
  time_t t = time(0);
  struct tm timestruct;
  timestruct = *localtime(&t);
  strftime(timestamp,13,"%H%M%S%d%m%y",&timestruct);
  return timestamp;
}
char* getDate(char* timestamp){
  getTimestamp(timestamp);
  char* date = new char[19];
  sprintf(date,"%c%c:%c%c:%c%c - %c%c/%c%c/%c%c",timestamp[0],timestamp[1],timestamp[2],timestamp[3],timestamp[4],timestamp[5],
timestamp[6],timestamp[7],timestamp[8],timestamp[9],timestamp[10],timestamp[11]);
  return date;
}


char* BasicComm::serializeData(packet* pkt){

  char* serializedData = new char[maxPacketSize];
  int offset = 0;
  int inc_offset = sizeof(uint16_t);
  int time_offset = sizeof(char)*TIMESTAMP_SIZE;
  int dataSize = pkt->length * sizeof(char);
  pkt->type = htons(pkt->type);
  pkt->seqn = htons(pkt->seqn);
  pkt->length = htons(pkt->length);

  memcpy(serializedData,&(pkt->type),sizeof(uint16_t)); //copy to serialized data with offsets
  offset += inc_offset;
  memcpy(serializedData+offset,&(pkt->seqn),inc_offset);
  offset += inc_offset;
  memcpy(serializedData+offset,&(pkt->length),inc_offset);
  offset += inc_offset;
  memcpy(serializedData+offset,(pkt->timestamp),sizeof(char)*TIMESTAMP_SIZE);
  offset += time_offset;
  memcpy(serializedData+offset,(char*)(pkt->_payload),dataSize);
  delete[] pkt;
  return serializedData;

}
packet* BasicComm::deserializeData(char* serializedData){
  int offset = 0;
  int inc_offset = sizeof(uint16_t);
  packet* pkt = new packet;
  int time_offset = sizeof(char)*TIMESTAMP_SIZE;
  memcpy(&(pkt->type),serializedData,inc_offset); //copy to serialized data with offsets
  offset += inc_offset;
  memcpy(&(pkt->seqn),serializedData+offset,inc_offset);
  offset += inc_offset;
  memcpy(&(pkt->length),serializedData+offset,inc_offset);
  offset += inc_offset;
  pkt->timestamp = new char[TIMESTAMP_SIZE];
  memcpy((pkt->timestamp),serializedData+offset,time_offset);
  offset += time_offset;
  pkt->type = ntohs(pkt->type);
  pkt->seqn = ntohs(pkt->seqn);
  pkt->length = ntohs(pkt->length);
  int dataSize = pkt->length * sizeof(char);
  pkt->_payload = new char[dataSize];
  bzero((char*)(pkt->_payload),dataSize);
  if(pkt->length != 0)
    memcpy((char*)(pkt->_payload),serializedData+offset,dataSize);
  else
    pkt->_payload = NULL;
  delete[] serializedData;


  return pkt;

}


int BasicComm::sendMessage(uint16_t cmd, char* data, char* timestamp){
  int n;
  int msgtype = cmd;

  packet* pkt = new packet;
  pkt->type = cmd;
  pkt->seqn = this->seqnum + 1;
  if(data != NULL)
    pkt->length = (strlen(data)) + 1;
  else
    pkt->length = 0;
  if(timestamp != NULL)
    pkt->timestamp = timestamp;
  else {
  char* timestamp = new char[TIMESTAMP_SIZE];
  pkt->timestamp = getTimestamp(timestamp);
  }
  pkt->_payload = data;
  char* serializedPkt = serializeData(pkt);
  n = write(this->sckt, serializedPkt, maxPacketSize);
  if (n < 0){
	 cout << "Error sending message" << endl;
   this->active = false;
   return -1;
  }
  this->seqnum++;
  return 0;
}

packet* BasicComm::readMessage(){
  int n;

  char* serialized = new char[maxPacketSize];
  bzero(serialized,maxPacketSize);
  int dataRecv = 0;
  while(dataRecv < maxPacketSize){
    n = read(this->sckt, serialized+dataRecv, maxPacketSize);
    if (n < 0){
      delete[] serialized;
      return NULL;
    }
    if (n == 0){
      connectionInterrupted();
      return NULL;
    }
    dataRecv += n;
  }
  packet* pkt = deserializeData(serialized);


  fprintf(stderr,"passei aqui\n");

  if (pkt->seqn == this->seqack + 1){//if packet is next in order, we can just inc SEQ ACK and return packet
    this->seqack++;
    this->seqack += this->numHigherAcks; //accounting for new packets(higher seq num) that were received before this one.
    this->numHigherAcks = 0; //we can also reset counter of acks above current one, once they are already accounted for
    fprintf(stderr,"passei aqui2\n");

    return pkt;
  }

  else if(pkt->seqn < this->seqack + 1){ //if sequence number of source is smaller than acknowledged by dest,
    fprintf(stderr,"passei aqui3\n");
                                  //it's definitely a replicate, we can just discard it.
    return NULL;
  }

  else{
    this->numHigherAcks++;
    return pkt;
  }
}

bool BasicComm::isActive(){
  return this->active;
}
void BasicComm::setActive(bool value){
  this->active = value;
}

int BasicComm::getSocket(){
  return this->sckt;
}

int initListeningSocket(int port){

struct sockaddr_in serv_addr, cli_addr;
int sckt;

if ((sckt = socket(AF_INET, SOCK_STREAM, 0)) == -1){
      cout << "ERROR opening socket";
      return -1;
    }
    int value=1;

if (setsockopt(sckt, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) == -1) {
    perror("setsockopt");
    exit(1);
}
serv_addr.sin_family = AF_INET;
serv_addr.sin_port = htons(port);
serv_addr.sin_addr.s_addr = INADDR_ANY;
bzero(&(serv_addr.sin_zero), 8);

if (::bind(sckt, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
  cout << "Port already in use" << endl;
  return -1;
}

listen(sckt, 5);

return sckt;
}



int createSocket(){
  int sckt;
  if ((sckt = socket(AF_INET, SOCK_STREAM, 0)) == -1)
      return -2;
  else
    return sckt;
}

int connectToSv(std::string host, int port, int sckt){
  struct hostent *server;
  struct sockaddr_in serv_addr;
  server = gethostbyname(host.c_str());
  if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);
  serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
  bzero(&(serv_addr.sin_zero), 8);

  if (connect(sckt,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
      return 0;
}
  else
    return 1;
  }


//int BasicComm::ping(){}
