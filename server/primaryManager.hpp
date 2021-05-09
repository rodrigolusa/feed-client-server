#pragma once
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <algorithm>
#include <pthread.h>

#include "myDatabase.hpp"
extern MyDatabase database;
#include "profile.hpp"
#include "usersession.hpp"
#include "notificationManager.hpp"
#include "receivedNotification.hpp"
#include "pendingNotification.hpp"




using namespace std;


class primaryManager{
private:
	int sckt;
	int port;
	//list<Replica> replicas;
public:
	int init(int port);
	int acceptConnections();
	void closeSocket();
	void sendKeepAlive();
};
