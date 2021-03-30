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
    int activeSessions;
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
    void AddReceivedNotification(ReceivedNotification r);
    void AddPendingNotification(PendingNotification p);
    list<ReceivedNotification> GetReceivedNotifications();
    list<PendingNotification> GetPendingNotification();
    void RemoveReceivedNotification(int r);
    void RemovePendingNotification(string p, int i);
    int GetFollowersNumber();
};
