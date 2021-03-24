#pragma once
#include "../common/basiccomm.hpp"
#include "../common/basiccomm.cpp"
using namespace std;
class Session: public BasicComm{
private: //session info, add more as it is needed
  string username;
  bool active;
public:
  int attemptLogin();
  void terminateSession();
  bool isActive();
  string getUsername();

  Session(int socket){
    this->sckt = socket;
    this->active = true;
    this->seqnum = 0;
  }
};
  vector<Session*> clientsessions;
