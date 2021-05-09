#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "myDatabase.hpp"
#include "../common/basiccomm.hpp"
#define MAX_NUM_REPLICAS 10

typedef struct{
  string hostname;
  int port;
}ServerInfo;
class replicaManager{
private:
  int port;
  fstream serverconfd;
  int listen_socket;
  int connection_sockets[MAX_NUM_REPLICAS-1];
  int connection_ports[MAX_NUM_REPLICAS-1];
  bool is_primary;
public:
  bool isPrimary();
  int init(int port);
  bool GetServerFromFile(ServerInfo* info);
  void announceCoordinator();
};
