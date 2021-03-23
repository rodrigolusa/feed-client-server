
#pragma once
using namespace std;
class Session{
private: //session info, add more as it is needed
  int socket;
  string username;
  bool active;
public:
  int attemptLogin();
  char* readMessage();
	int sendMessage(char* msg);
  void terminateSession();
  bool isActive();
  string getUsername();

  Session(int socket){
    this->socket = socket;
    this->active = true;
  }
};
  vector<Session*> clientsessions;
