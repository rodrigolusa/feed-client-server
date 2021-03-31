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
  pkt->seqn = this->seqnum;
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
      return NULL;
    }
    if (n == 0){
      connectionInterrupted();
      return NULL;
    }
    dataRecv += n;
  }
  packet* pkt = deserializeData(serialized);
  return pkt;
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
//int BasicComm::ping(){}
