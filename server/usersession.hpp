#pragma once
#include <vector>
#include <list>
#include "profile.hpp"
#include "myDatabase.hpp"
#include "../common/basiccomm.hpp"
#include "primaryManager.hpp"
using namespace std;

typedef struct{
  char* username;
  char* message;
  char* timestamp;
}QueuedMessage;

class Session: public BasicComm{
private: //session info, add more as it is needed
  string username;
  string hostname;
  int port;
public:
  list<QueuedMessage> sendingQueue;
  int attemptLogin();
  void connectionInterrupted();
  string getUsername();
  void setUsername(string name);
  void setHostname(string host);
  void flushsendingQueue();
  void setPort(int port);
  int getPort();
  string getHostname();

  Session(int socket,string hostname){
    this->sckt = socket;
    this->hostname = hostname;
    setActive(true);
    this->port = -1;
  }
};
