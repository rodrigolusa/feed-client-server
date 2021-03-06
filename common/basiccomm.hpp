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
  FOLLOW,SEND_NAME,SEND_DATA,SEND_UNNAMED,LOGIN,LOGOUT, SUCCESS,
	ERROR,BACKUP_PORT, SEND_HEADER, BACKUP_INFO, KEEP_ALIVE,
	PORT, COORDINATOR, ANSWER, ELECTION, SOCKET_FREE,
	REMOVE_RECEIVED, REMOVE_PENDING, UPDATE_PENDING,
	UPDATE_RECEIVED
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
public:
  int sendMessage(uint16_t cmd,  char* data = NULL, char* timestamp = NULL);
  packet* readMessage();
	void setActive(bool value);
	bool isActive();
	int getSocket();
	void setSocket(int sckt);
	void resetSeqNums();
	virtual void connectionInterrupted() = 0;
	BasicComm();
};

char* getTimestamp(char* timestamp);
char* getDate(char* timestamp);
int connectToSv(std::string host, int port,int socket);
int initListeningSocket(int port);
int createSocket();
