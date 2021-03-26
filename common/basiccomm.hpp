#pragma once
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
typedef struct __packet{
	uint16_t type;
	uint16_t seqn;
	uint16_t length;
	uint16_t timestamp;
	const char* _payload;
} packet;
#define MAX_TWEET_SIZE 128


enum CMDS{
  FOLLOW,SEND,LOGIN,LOGOUT, SUCCESS,ERROR
};

class BasicComm{
private:
  char* serializeData(packet* pkt);
  packet* deserializeData(char* data);
  int maxPacketSize = sizeof(uint16_t) * 4 + sizeof(char) * (MAX_TWEET_SIZE+1);
protected:
	bool active;
  int sckt;
  uint16_t seqnum;
public:
  int sendMessage(uint16_t cmd,  char* data = NULL, uint16_t timestamp = 0);
  packet* readMessage();
	void setActive(bool value);
	bool isActive();
	virtual void closeConnection() = 0;
};
