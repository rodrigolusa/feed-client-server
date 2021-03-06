#pragma once

#include "../common/basiccomm.hpp"
#include "replicaManager.hpp"
#include <sstream>
using namespace std;


class replicaManager;


class ReplicaComms: public BasicComm{
private:
  bool first_init;
public:
  pthread_mutex_t sendmessage_mutex;
  replicaManager* replica;

  void connectionInterrupted();
  void init(int socket,replicaManager* replica);
  ReplicaComms();
};

  void* readMessageFromReplica(void* args);
