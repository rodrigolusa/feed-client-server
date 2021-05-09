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

#define DEFAULT_PORT 4000


using namespace std;


class primaryManager{
private:
	int sckt;
	int port;
	//list<Replica> replicas;
public:
	int init();
	int acceptConnections();
	void closeSocket();
	void sendKeepAlive();
};
