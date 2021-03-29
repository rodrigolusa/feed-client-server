#include "../common/basiccomm.hpp"
using namespace std;
#pragma once

#define PORT 4000

class ClientComms: public BasicComm{
private:
char* username;
char* hostname;
int port;
public:
int connectToServer(); //tries to establish connetion to server, returns 0 if successful and -1 otherwise
void connectionInterrupted(); //closes connection between client and server
int login();
void attemptReconnect();
void terminateConnection();
void closeSocket();
void init(char* username,char* hostname, int port);


};
