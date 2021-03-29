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
#include "receivedNotification.hpp"
#include "pendingNotification.hpp"


#define PORT 4000

using namespace std;


class serverComms{
private:
	int sckt;
public:
	int init();
	int acceptConnections();
	void closeSocket();
};
	void* ClientManagement(void* arg);
