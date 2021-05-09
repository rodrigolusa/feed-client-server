#include "replicaManager.hpp"
bool replicaManager::isPrimary(){
  return is_primary;
}

int replicaManager::init(int port){
  database.initDatabase();
  this->comms = new ReplicaComms();
  this->is_primary = false;
  for(int i = 0; i < MAX_NUM_REPLICAS-1; i++){
    this->connection_sockets[i] = -1;
    this->connection_ports[i] = -1;
  }

  this->listen_socket = initListeningSocket(port);
  cout << this->listen_socket << endl;
  pthread_t new_thread;
  pthread_create(&new_thread, NULL, acceptReplicas, this);

  if(this->listen_socket == -1)
    return -1;
  this->port = port;
  int idx = 0;
  this->serverconfd.open("server-conf.cfg",fstream::in);
  ServerInfo* info = new ServerInfo;

  while(GetServerFromFile(info)){
    if(info->port == this->port)
      continue;
      else{
        int socket = createSocket();
          if(connectToSv(info->hostname,info->port,socket)){
                this->comms->sendMessage(PORT,(char*)to_string(this->port).c_str(),NULL,socket);
                this->connection_sockets[idx] = socket;
                this->connection_ports[idx] = info->port;
                idx++;
              }
              else{
                  cout << info->hostname << " " << info->port << endl;
                continue;
          }
        }
      }


  this->serverconfd.close();
  delete info;

  int maxport = this->port;
  for(int i = 0; i < MAX_NUM_REPLICAS-1;i++){
    if(this->port < connection_ports[i])
      maxport =  connection_ports[i];
        fprintf(stderr,"%d\n",connection_ports[i]);
    }
    this->leader_port = maxport;

    if(this->port == maxport){ //if this replica has the highest ID, announce new leader and become primary.
      announceCoordinator();
      this->is_primary = true;
    }
  return 0;
}


bool replicaManager::GetServerFromFile(ServerInfo* info){
  string line, host, port;
  if(getline(this->serverconfd,line)){
      stringstream lineStream(line);
      lineStream >> host;
      lineStream >> port;
      info->hostname = host;
      info->port = stoi(port);
      return true;
  }
    return false;

}

void* acceptReplicas(void* args){
  replicaManager* replica = (replicaManager*) args;

  while(true){
  int newsockfd = 0;
  struct sockaddr_in cli_addr;
  socklen_t clilen;
  clilen = sizeof(struct sockaddr_in);
  if ((newsockfd = accept(replica->listen_socket, (struct sockaddr *) &cli_addr, &clilen)) == -1){
    cout << " Couldnt accept replica\n";
    continue;
  }

  packet* pkt = replica->comms->readMessage(newsockfd);
  const char* port = new char[pkt->length];
  port = pkt->_payload;

  bool foundSpot = false;
  //dont forget mutex here
  for(int i = 0; i < MAX_NUM_REPLICAS-1;i++){
    if(replica->connection_sockets[i] == -1){
      replica->connection_sockets[i] = newsockfd;
      replica->connection_ports[i] = atoi(port);
      foundSpot = true;
      break;
    }
  }

  if(!foundSpot)
    fprintf(stderr,"Maximum replica capacity reached, can't accept a new one\n");
  else
    cout << "Replica with port " << atoi(port) << "connected" << endl;;
  }
}



void replicaManager::announceCoordinator(){

  return;
}
