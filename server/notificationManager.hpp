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
#include "replicaManager.hpp"
#include <fcntl.h>
#include <set>
class replicaManager;
typedef struct{
  string follower;
  PendingNotification pn;
}PendingDelayed;
list<PendingDelayed> getPendingForFollowers(string profile, PendingNotification pn);
void SendNotification(string toProfile, ReceivedNotification rn);
void ClearNotifications(string profile, int port,replicaManager* replica);
void* NotificationConsumer(void* arg);
void* NotificationProducer(void* arg);
bool islistEmptyForClient(list<PendingNotification> notf_list, int socket);
void commitChanges(ReceivedNotification rn, list<PendingDelayed>, string username, replicaManager* replica);
void findNotificationsToRemove(string profile,int port,replicaManager* replica);
