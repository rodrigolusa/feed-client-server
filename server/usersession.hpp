#pragma once
#include "../common/basiccomm.hpp"
#include "../common/basiccomm.cpp"
using namespace std;
class Session: public BasicComm{
private: //session info, add more as it is needed
  string username;
public:
  int attemptLogin();
  void closeConnection();
  string getUsername();

  Session(int socket){
    this->sckt = socket;
    setActive(true);
    this->seqnum = 0;
  }
};
  vector<Session*> clientsessions;
