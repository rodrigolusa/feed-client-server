#include <iostream>
#include "primaryManager.hpp"
#include "replicaManager.hpp"

int main(int argc, char *argv[]){


  replicaManager replica;
  int port = atoi(argv[1]);
  replica.init(port);

  while(true){


    if(replica.isPrimary()){
      primaryManager primary;
      primary.init(port + MAX_NUM_REPLICAS);
      while(replica.isPrimary()){
        primary.acceptConnections();
        primary.sendKeepAlive();
        replica.acceptReplicas();
        replica.receiveMessages();
      }
      primary.finalize();
    }
    else
      do{
        replica.acceptReplicas();
        replica.receiveMessages();

    }while(replica.isPrimary() != true);

  }

}
