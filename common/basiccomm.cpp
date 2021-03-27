#include "basiccomm.hpp"
using namespace std;

char* BasicComm::serializeData(packet* pkt){

  char* serializedData = new char[maxPacketSize];
  int offset = 0;
  int inc_offset = sizeof(uint16_t);
  int dataSize = pkt->length * sizeof(char);
  pkt->type = htons(pkt->type);
  pkt->seqn = htons(pkt->seqn);
  pkt->length = htons(pkt->length);
  pkt->timestamp = htons(pkt->timestamp);

  memcpy(serializedData,&(pkt->type),sizeof(uint16_t)); //copy to serialized data with offsets
  offset += inc_offset;
  memcpy(serializedData+offset,&(pkt->seqn),sizeof(uint16_t));
  offset += inc_offset;
  memcpy(serializedData+offset,&(pkt->length),sizeof(uint16_t));
  offset += inc_offset;
  memcpy(serializedData+offset,&(pkt->timestamp),sizeof(uint16_t));
  offset += inc_offset;
  memcpy(serializedData+offset,(char*)(pkt->_payload),dataSize);
  delete[] pkt;
  return serializedData;

}
packet* BasicComm::deserializeData(char* serializedData){
  int offset = 0;
  int inc_offset = sizeof(uint16_t);
  packet* pkt = new packet;

  memcpy(&(pkt->type),serializedData,sizeof(uint16_t)); //copy to serialized data with offsets
  offset += inc_offset;
  memcpy(&(pkt->seqn),serializedData+offset,sizeof(uint16_t));
  offset += inc_offset;
  memcpy(&(pkt->length),serializedData+offset,sizeof(uint16_t));
  offset += inc_offset;
  memcpy(&(pkt->timestamp),serializedData+offset,sizeof(uint16_t));
  offset += inc_offset;
  pkt->type = ntohs(pkt->type);
  pkt->seqn = ntohs(pkt->seqn);
  pkt->length = ntohs(pkt->length);
  pkt->timestamp = ntohs(pkt->timestamp);
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


int BasicComm::sendMessage(uint16_t cmd, char* data, uint16_t timestamp){
  int n;
  int msgtype = cmd;

  packet* pkt = new packet;
  pkt->type = cmd;
  pkt->seqn = this->seqnum;
  if(data != NULL)
    pkt->length = (strlen(data)) + 1;
  else
    pkt->length = 0;
  pkt->timestamp = timestamp;
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
      cout << "Error reading message" << endl;
      connectionInterrupted();
      return NULL;
    }
    if (n == 0){
      cout << "Connection lost" << endl;
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
//int BasicComm::ping(){}
