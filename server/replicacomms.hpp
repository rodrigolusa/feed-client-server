#pragma once

#include "../common/basiccomm.hpp"
using namespace std;

enum REPLICA_CMDS{
KEEP_ALIVE,PORT
};
class ReplicaComms: public BasicComm{

public:
  packet* readMessageFromSocket(int socket);
  void connectionInterrupted();

};
