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
#include "receivedNotification.hpp"
#include "pendingNotification.hpp"
#include "usersession.hpp"
void SendNotification(string toProfile, ReceivedNotification rn);
void ClearNotifications(string profile);
void* NotificationConsumer(void* arg);
void* NotificationProducer(void* arg);
bool islistEmptyForClient(list<PendingNotification> notf_list, int socket);
