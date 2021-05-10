#pragma once
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <algorithm>
#include <pthread.h>

#include "myDatabase.hpp"
#include "profile.hpp"
#include "usersession.hpp"
#include "notificationManager.hpp"
#include "receivedNotification.hpp"
#include "pendingNotification.hpp"
#include "replicaManager.hpp"

#define DEFAULT_PORT 4000


using namespace std;

class Session;
class replicaManager;

typedef struct{
	replicaManager* replica;
	Session* session;
}Session_Replica;

class primaryManager{
private:
	int port;
	//list<Replica> replicas;
public:
	int init(replicaManager* replica);
	int sckt;
	void closeSocket();
	void sendKeepAlive();
};


typedef struct{
	replicaManager* replica;
	primaryManager* primary;
}Managers;

void* acceptClients(void*);//thread to accept clients
