#include "../common/basiccomm.hpp"

using namespace std;
#pragma once

#define PORT 4000
#define DEFAULT_BACKUP_PORT 51000
class ClientComms: public BasicComm{
private:
char* username;
char* hostname;
int port;
public:
bool not_reconnected;
bool not_initialized_listen;
pthread_mutex_t reconnecting_mutex;
pthread_cond_t has_reconnected;
int backup_socket;
int backup_port;
int connectToServer(); //tries to establish connetion to server, returns 0 if successful and -1 otherwise
void connectionInterrupted(); //closes connection between client and server
int login();
void attemptReconnect();
void terminateConnection();
void closeSocket();
void init(char* username,char* hostname, int port);



};
void* waitForBackupConn(void* args);
