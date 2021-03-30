#pragma once
#include <vector>
#include "profile.hpp"
#include "myDatabase.hpp"
#include "../common/basiccomm.hpp"
#include "servercomms.hpp"
using namespace std;
class Session: public BasicComm{
private: //session info, add more as it is needed
  string username;
  Profile* profile;
public:
  int attemptLogin();
  void connectionInterrupted();
  string getUsername();

  Session(int socket){
    this->sckt = socket;
    setActive(true);
    this->seqnum = 0;
  }
};
