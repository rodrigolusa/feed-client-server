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
#include "usersession.hpp"
#include "notificationManager.hpp"
#define MAX_NUM_REPLICAS 10
#define TIMEOUT 30000


class Session;
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
  pthread_cond_t changing_role;
  int listen_socket;
  bool candidate;
  bool ongoing_election;
  int primary_socket;
  bool connecting_to_replicas;
  int connection_sockets[MAX_NUM_REPLICAS-1];
  int connection_ports[MAX_NUM_REPLICAS-1];
  ReplicaComms* comms[MAX_NUM_REPLICAS-1];
  fstream serverconfd;
  int leader_port;
  int leader_socket;
  bool isPrimary();
  void setPrimary(bool value);
  int init(int port);
  bool GetServerFromFile(ServerInfo* info);
  void announceCoordinator();
  void sendmessagetoAllReplicas(uint16_t cmd, char* data=NULL, char* timestamp=NULL);
  void addSessionToBackup(string username, string hostname, int port);
  void removeSessionFromBackup(string username, string hostname, int port);
  void addFollowtoBackup(string followed,string follower);
  void addNotificationToBackup(string profile,int id, char* timestamp, packet* pkt );
  void shutdownConnection(ReplicaComms* comms);
  void terminatePrimaryOperations();
  void setLeader(int socket);
  void sendLogoutToReplicas(Session* user);
  void electionActions(packet* pkt,ReplicaComms* comms);
  void sendElectiontoHigherIDs();
  void checkIfOnlyBackup();
};

  void* acceptReplicas(void* args);
