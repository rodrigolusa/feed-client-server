#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;
#pragma once

#define PORT 4000

class ClientComms{
private:
  int sockfd;
public:

int connectToServer(char* hostname); //tries to establish connetion to server, returns 0 if successful and -1 otherwise
void closeConnection(); //closes connection between client and server
int sendMessage(char* msg);
int login(char* username);
char* readMessage();

};
