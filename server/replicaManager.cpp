#include "replicaManager.hpp"
bool replicaManager::isPrimary(){
  return is_primary;
}

int replicaManager::init(int port){
  database.initDatabase();
  this->is_primary = false;
  for(int i = 0; i < MAX_NUM_REPLICAS-1; i++){
    this->connection_sockets[i] = -1;
    this->connection_ports[i] = -1;
  }
  this->listen_socket = initListeningSocket(port);
  if(this->listen_socket == -1)
    return -1;
  this->port = port;

  this->serverconfd.open("serverconf.cfg",fstream::in);
  ServerInfo* info = new ServerInfo;
  int socket = createSocket();
  while(GetServerFromFile(info)){
    if(connectToSv(info->hostname,info->port,socket)){
      for(int i = 0; i < MAX_NUM_REPLICAS-1; i++){ //put socket on connected list
        if(this->connection_sockets[i] == -1){
          this->connection_sockets[i] = socket;
          this->connection_ports[i] = info->port;
          int socket = createSocket();
          break;
        }
        else
          continue;
        }
    }
  }
  delete info;

  int maxport = this->port;
  for(int i = 0; i < MAX_NUM_REPLICAS-1;i++){
    if(this->port < connection_ports[i])
      maxport =  connection_ports[i];
    }

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

void replicaManager::announceCoordinator(){
  return;
}
