#include "replicaManager.hpp"
bool replicaManager::isPrimary(){
  return is_primary;
}

int replicaManager::init(int port){
  database.initDatabase();
  pthread_mutex_init(&isprimary_mutex,NULL);
  this->is_primary = false;
  for(int i = 0; i < MAX_NUM_REPLICAS-1; i++){
    this->connection_sockets[i] = -1;
    this->connection_ports[i] = -1;
    this->comms[i] = new ReplicaComms();
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
                this->comms[idx]->init(socket,this);
                this->comms[idx]->sendMessage(PORT,(char*)to_string(this->port).c_str(),NULL);
                this->connection_sockets[idx] = socket;
                this->connection_ports[idx] = info->port;
                pthread_t new_thread2;
                pthread_create(&new_thread2, NULL, readMessageFromReplica, this->comms[idx]);
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
    if(maxport < connection_ports[i]){
      this->leader_socket = connection_sockets[i];
      maxport =  connection_ports[i];
    }
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


void replicaManager::addNotificationToBackup(string profile,int id, char* timestamp, packet* pkt ){

    ReceivedNotification rn;
    PendingNotification pn;
    list<string>::iterator it;
    list<string> followers;
    if(id > notificationId)
      notificationId = id;

    Profile* prof = database.getProfile(profile);
    rn.id = id;
    rn.timestamp = timestamp;
    rn.message = pkt->_payload;
    rn.size = pkt->length;
    rn.pendingFollowersToReceive = database.GetFollowersNumber(profile) - database.GetActiveFollowersNumber(profile);
    prof->AddReceivedNotification(rn); // dont need to wait for mutex, already guaranteed we will get it(serialized).
    pn.profileId = profile;
    pn.notificationId = id;
    database.AddPendingNotifications(profile,pn);
    database.PrintDatabase();
    followers = database.GetFollowers(profile);

    for(it = followers.begin(); it != followers.end();it++)
    {
      for(int i = 0; i < 2; i++){
        prof = database.getProfile(*it);
          if(prof->backup_ports[i] != -1)//if session is active
            ClearNotifications(*it,prof->backup_ports[i],this);
    }
  }
    database.PrintDatabase();
}





void replicaManager::announceCoordinator(){

  return;
}


void* acceptReplicas(void* args){
  replicaManager* replica = (replicaManager*) args;
    int newsockfd = 0;
  while(true){
  struct sockaddr_in cli_addr;
  socklen_t clilen;
  clilen = sizeof(struct sockaddr_in);
  if ((newsockfd = accept(replica->listen_socket, (struct sockaddr *) &cli_addr, &clilen)) == -1){
    cout << " Couldnt accept replica\n";
    continue;
  }

  bool foundSpot = false;

  for(int i = 0; i < MAX_NUM_REPLICAS-1;i++){
    if(replica->connection_sockets[i] == -1){
      replica->connection_sockets[i] = newsockfd;
      replica->comms[i]->init(newsockfd,replica);
      packet* pkt = replica->comms[i]->readMessage();
      while(pkt == NULL)
        pkt = replica->comms[i]->readMessage();
      cout << pkt->_payload << endl;
      const char* port = new char[pkt->length];
      port = pkt->_payload;
      replica->connection_ports[i] = atoi(port);
      pthread_t new_thread;
      pthread_create(&new_thread, NULL, readMessageFromReplica, replica->comms[i]);
      foundSpot = true;
      cout << "Replica with port " << atoi(port) << " connected" << endl;
      break;
    }
  }
  newsockfd =0;

  if(!foundSpot)
    fprintf(stderr,"Maximum replica capacity reached, can't accept a new one\n");

    }
  }

void replicaManager::sendmessagetoAllReplicas(uint16_t cmd, char* data, char* timestamp){

  for(int i = 0; i < MAX_NUM_REPLICAS-1;i++){
    if(this->comms[i]->isActive()){
      pthread_mutex_lock(&(this->comms[i]->sendmessage_mutex));
      if(this->comms[i]->sendMessage(cmd,data) == -1){
        this->connection_ports[i] = -1;
        this->connection_sockets[i] = -1;
      }
      pthread_mutex_unlock(&(this->comms[i]->sendmessage_mutex));

    }
    else
      continue;
    }
}

void replicaManager::addSessionToBackup(string username, string hostname, int port){
  if(!database.ExistsProfile(username)){
    Profile prof(username);
    database.AddProfile(prof);
    database.AddSessionCount(username,hostname,port);
    cout << "novo usuario " << username << " no host " << hostname << " e porta " << port << endl;
  }
  else{
    Profile* prof = database.getProfile(username);
    database.AddSessionCount(username,hostname,port);

    cout << username << "logado no host " << hostname << " e porta " << port << endl;
  }
}


void replicaManager::removeSessionFromBackup(string username, string hostname, int port){
  Profile* prof = database.getProfile(username);
  database.SubtractSessionCount(username,hostname,port);
  cout << username << "deslogando no host " << hostname << " e porta " << port << endl;
}

void replicaManager::addFollowtoBackup(string followed, string follower){
  database.AddFollowing(follower, followed);
  database.AddFollower(followed, follower);
  cout << "Pedido de follow de " << follower << " para seguir " << followed << endl;
}


void replicaManager::shutdownConnection(ReplicaComms* comms){

  for(int i = 0; i < MAX_NUM_REPLICAS-1; i++){
    if(this->comms[i] == comms){
      cout << "Replica with port " << this->connection_ports[i] << "disconnected" << endl;
      this->connection_ports[i] = -1;
      this->connection_sockets[i] = -1;
      break;
    }
  }
}
