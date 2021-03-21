#include <iostream>
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

class serverComms{
private:
	int sockfd;

public:
	int init();
	int acceptConnections();
	void closeSocket();
};

	void* ClientManagement(void* arg);
	char* readMessage(int socket);
	int writeMessage(int socket);
