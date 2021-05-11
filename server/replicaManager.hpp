#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include "myDatabase.hpp"
#include <pthread.h>
#include "../common/basiccomm.hpp"
#include "replicacomms.hpp"
#include "notificationManager.hpp"
#define MAX_NUM_REPLICAS 10

class ReplicaComms;

typedef struct{
  string hostname;
  int port;
}ServerInfo;
class replicaManager{
private:
    int port;
      bool is_primary;
public:
  pthread_mutex_t isprimary_mutex;
  int listen_socket;
  int connection_sockets[MAX_NUM_REPLICAS-1];
  int connection_ports[MAX_NUM_REPLICAS-1];
  ReplicaComms* comms[MAX_NUM_REPLICAS-1];
  fstream serverconfd;
  int leader_port;
  int leader_socket;
  bool isPrimary();
  int init(int port);
  bool GetServerFromFile(ServerInfo* info);
  void announceCoordinator();
  void sendmessagetoAllReplicas(uint16_t cmd, char* data=NULL, char* timestamp=NULL);
  void addSessionToBackup(string username, string hostname, int port);
  void removeSessionFromBackup(string username, string hostname, int port);
  void addFollowtoBackup(string followed,string follower);
  void addNotificationToBackup(string profile,int id, char* timestamp, packet* pkt );

};

  void* acceptReplicas(void* args);
