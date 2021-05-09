#pragma once
#include <string>
#include <list>
#include <pthread.h>
#include "receivedNotification.hpp"
#include "pendingNotification.hpp"


using namespace std;

class Profile{
public:
    string id;
    pthread_mutex_t logincontrol_mutex;
    pthread_mutex_t receivenotification_mutex;
    pthread_mutex_t pendingnotification_mutex;
    pthread_cond_t not_empty;
    int activeSessions;
    int backup_ports[2];//backup ports for sessions.
    string backup_hosts[2];//saving ip for reconnecting reasons.
    list<string> followers;
    list<string> following;
    list<ReceivedNotification> receivedNotifications;
    list<PendingNotification> pendingNotifications;

    Profile();
    Profile(string i);
    void AddFollowing(string p);
    list<string> GetFollowing();
    void RemoveFollowing(string p);
    void AddFollower(string p);
    list<string> GetFollowers();
    void RemoveFollower(string p);
    bool AddReceivedNotification(ReceivedNotification r);
    void AddPendingNotification(PendingNotification p);
    list<ReceivedNotification> GetReceivedNotifications();
    list<PendingNotification> GetPendingNotification();
    void RemoveReceivedNotification(int r);
    void RemovePendingNotification(string p, int i);
    int GetFollowersNumber();
};
