#pragma once
#include <vector>
#include <list>
#include "profile.hpp"
#include "myDatabase.hpp"
#include "../common/basiccomm.hpp"
#include "servercomms.hpp"
using namespace std;

typedef struct{
  char* username;
  char* message;
  char* timestamp;
}QueuedMessage;

class Session: public BasicComm{
private: //session info, add more as it is needed
  string username;
public:
  list<QueuedMessage> sendingQueue;
  int attemptLogin();
  void connectionInterrupted();
  string getUsername();
  void flushsendingQueue();

  Session(int socket){
    this->sckt = socket;
    setActive(true);
    this->seqnum = 0;
  }
};
