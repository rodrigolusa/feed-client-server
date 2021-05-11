#include <iostream>
#include "primaryManager.hpp"
#include "replicaManager.hpp"
#include <unistd.h>
int main(int argc, char *argv[]){


  replicaManager replica;
  int port = atoi(argv[1]);
  if(replica.init(port) == -1)
    exit(1);
  while(true){


    if(replica.isPrimary()){
      primaryManager primary;
      primary.init(&replica);
      while(replica.isPrimary()){
        primary.sendKeepAlive(&replica);
        usleep(10000);
      }
      //primary.finalize();
    }
    else{
        fprintf(stderr,"Sou uma replica\n");
      do{
          usleep(100000);

        //replica.receiveMessages();

    }while(replica.isPrimary() != true);
    }
  }

}
