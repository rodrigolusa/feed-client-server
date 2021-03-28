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

pthread_mutex_t sessionvector_mutex;

#include "usersession.hpp"
#include "usersession.cpp"
#include "myDatabase.hpp"
#include "profile.hpp"

MyDatabase database;

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
