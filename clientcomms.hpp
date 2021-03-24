#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "basiccomm.hpp"
#include "basiccomm.cpp"

using namespace std;
#pragma once

#define PORT 4000

class ClientComms: public BasicComm{
private:
  bool connected;
public:
int connectToServer(char* hostname); //tries to establish connetion to server, returns 0 if successful and -1 otherwise
void closeConnection(); //closes connection between client and server
int login(char* username);
void setConnected(bool value);
ClientComms(){
  this->connected = false;
  this->seqnum = 0;
}

};
