#pragma once


#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
//#include "utils.hpp"
typedef struct __packet{
	uint16_t type;
	uint16_t seqn;
	uint16_t length;
	char* timestamp;
	const char* _payload;
} packet;
#define MAX_TWEET_SIZE 128
#define TIMESTAMP_SIZE 13


enum CMDS{
  FOLLOW,SEND_NAME,SEND_DATA,SEND_UNNAMED,LOGIN,LOGOUT, SUCCESS,ERROR,BACKUP_PORT
};

class BasicComm{
private:
  char* serializeData(packet* pkt);
  packet* deserializeData(char* data);
  int maxPacketSize = sizeof(uint16_t) * 3 + sizeof(char) * (MAX_TWEET_SIZE+1) + sizeof(char) * TIMESTAMP_SIZE;
protected:
	bool active;
  int sckt;
  uint16_t seqnum;
	uint16_t seqack;
	uint16_t numHigherAcks;
public:
  int sendMessage(uint16_t cmd,  char* data = NULL, char* timestamp = NULL);
  packet* readMessage();
	void setActive(bool value);
	bool isActive();
	int getSocket();
	virtual void connectionInterrupted() = 0;
};

char* getTimestamp(char* timestamp);
char* getDate(char* timestamp);
int connectToSv(std::string host, int port,int socket);
int initListeningSocket(int port);
int createSocket();
