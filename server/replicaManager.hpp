#pragma once
#include <iostream>
#define MAX_NUM_REPLICAS 10
class replicaManager{
private:
  int port;
  int sckt;
  bool is_primary;
public:
  bool isPrimary();
  int init();

};
