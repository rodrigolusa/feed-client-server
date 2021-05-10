#include "replicacomms.hpp"

void ReplicaComms::connectionInterrupted(){

}

ReplicaComms::ReplicaComms(){
  this->active = false;
}
void ReplicaComms::init(int socket,replicaManager* replica){
  this->replica = replica;
  this->sckt = socket;
  cout << this->sckt << endl;
  this->active = true;
  pthread_mutex_init(&sendmessage_mutex,NULL);
}



void* readMessageFromReplica(void* args){

  ReplicaComms* comms = (ReplicaComms*) args;
  comms->setActive(true);
  replicaManager* replica = comms->replica;
    cout << comms->getSocket() << endl;
  string username,line,id, loginname, hostname, port;
  char* timestamp;
  stringstream lineStream;
  const char* header;
  while(true){

    packet* pkt = comms->readMessage();

    //if(timeout...) check for timeout if leader.
    if(pkt == NULL)
      continue;
    if(comms->getSocket() == replica->leader_socket){ //if this is the leader, then reply accordingly

        switch(pkt->type){

        case KEEP_ALIVE:
          cout << "primary diz: estou vivo" << endl;
        break;
        case SEND_HEADER:
          header = new char[pkt->length];
          header = pkt->_payload;
          line = string(header);
          delete[] header;
          lineStream << line;
          timestamp = pkt->timestamp;
          lineStream >> username;
          lineStream >> id;
          lineStream.str("");
          lineStream.clear();
          line.clear();

          break;
        case SEND_DATA:
          replica->addNotificationToBackup(username,stoi(id),timestamp,pkt);
          id.clear();
          username.clear();
          break;

          case LOGIN:
          header = new char[pkt->length];
          header = pkt->_payload;
          line = string(header);
          delete[] header;
          lineStream << line;
          lineStream >> loginname;
          lineStream >> hostname;
          lineStream >> port;
          lineStream.str("");
          lineStream.clear();
          line.clear();
          replica->addSessionToBackup(loginname,hostname,stoi(port));
          break;

          case LOGOUT:
          header = new char[pkt->length];
          header = pkt->_payload;
          line = string(header);
          delete[] header;
          lineStream << line;
          lineStream >> loginname;
          lineStream >> hostname;
          lineStream >> port;
          lineStream.str("");
          lineStream.clear();
          line.clear();
          replica->removeSessionFromBackup(loginname,hostname,stoi(port));
          break;

        }

        delete pkt;
    }

    else{


      }
    }
  }
