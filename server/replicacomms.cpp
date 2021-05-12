#include "replicacomms.hpp"

void ReplicaComms::connectionInterrupted(){
  this->setActive(false);
  close(this->sckt);
  replica->shutdownConnection(this);
}

ReplicaComms::ReplicaComms(){
  this->setActive(false);
  this->first_init = true;
}
void ReplicaComms::init(int sckt,replicaManager* replica){
  this->replica = replica;
  this->setActive(true);
  this->setSocket(sckt);
  if(this->first_init){//control so we dont init mutex twice.
  pthread_mutex_init(&sendmessage_mutex,NULL);
  this->first_init = false;
  }
}



void* readMessageFromReplica(void* args){

  ReplicaComms* comms = (ReplicaComms*) args;
  replicaManager* replica = comms->replica;
  while(replica->connecting_to_replicas);//wait for other replicas to connect
  string username,line,id, loginname, hostname, port, followed,follower;
  char* timestamp;
  stringstream lineStream;
  const char* header;
  struct timeval tv;
  tv.tv_sec = TIMEOUT / 1000000;
  tv.tv_usec = TIMEOUT;
  setsockopt(comms->getSocket(), SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
  while(comms->isActive()){

    packet* pkt = comms->readMessage();

    if(replica->ongoing_election){
        replica->electionActions(pkt,comms);
        continue;
      }

    if(replica->isPrimary()){
      if(pkt == NULL) //if pkt is empty, just ignore.
        continue;

        else if(pkt->type == ELECTION || pkt->type == COORDINATOR){
          replica->ongoing_election = true;
          replica->electionActions(pkt,comms);
          }

        }
    else{

     if(comms->getSocket() == replica->leader_socket){ //if this is the leader, then reply accordingly

       if(pkt == NULL){
         replica->ongoing_election = true;
         replica->sendElectiontoHigherIDs();
       }

       else{
        switch(pkt->type){

        case KEEP_ALIVE://if keep alive, just ignore.
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

          case FOLLOW:
          header = new char[pkt->length];
          header = pkt->_payload;
          line = string(header);
          delete[] header;
          lineStream << line;
          lineStream >> followed;
          lineStream >> follower;
          lineStream.str("");
          lineStream.clear();
          line.clear();
          replica->addFollowtoBackup(followed,follower);

          case ELECTION:
          replica->ongoing_election = true;
          replica->electionActions(pkt,comms);
          break;


        }
        delete pkt;
      }
    }

    else {
          if(pkt == NULL)
            continue;
          else if(pkt->type == ELECTION){
            replica->ongoing_election = true;
            replica->electionActions(pkt,comms);
            }
          }

  }
}

  replica->checkIfOnlyBackup();//have to check if this was the last comm.
  //replica->checkIfBackups();
  pthread_exit(NULL);
}
