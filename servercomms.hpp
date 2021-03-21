#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#pragma once
using namespace std;

#define PORT 4000

typedef struct{
	int socket;
	string username;
}session;


class serverComms{
private:
	int sockfd;
	vector<session> clientsessions;
public:
	int init();
	int acceptConnections();
	void closeSocket();
	int attemptLogin(session* user);
};

	void* ClientManagement(void* arg);
	char* readMessage(int socket);
	int writeMessage(int socket,char* msg);
