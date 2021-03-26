#include "../common/basiccomm.hpp"

using namespace std;
#pragma once

#define PORT 4000

class ClientComms: public BasicComm{

public:
int connectToServer(char* hostname); //tries to establish connetion to server, returns 0 if successful and -1 otherwise
void closeConnection(); //closes connection between client and server
int login(char* username);
ClientComms(){
  setActive(false);
  this->seqnum = 0;
}

};
