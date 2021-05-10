#pragma once

#include "../common/basiccomm.hpp"
#include "replicaManager.hpp"
#include <sstream>
using namespace std;

enum REPLICA_CMDS{
KEEP_ALIVE,PORT
};

class replicaManager;


class ReplicaComms: public BasicComm{

public:
  pthread_mutex_t sendmessage_mutex;
  replicaManager* replica;

  void connectionInterrupted();
  void init(int socket,replicaManager* replica);
  ReplicaComms();
};

  void* readMessageFromReplica(void* args);
