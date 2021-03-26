#pragma once
#include <string>
#include <list>
#include "profile.hpp"
#include "receivedNotification.hpp"
#include "pendingNotification.hpp"
#include <iostream>

using namespace std;

class DataProfile{
public:
    string id;
    Profile p;
    list<string> followers;
    list<ReceivedNotification> receivedNotifications;
    list<PendingNotification> pendingNotifications;
    
    DataProfile(Profile i);
    void AddFollower(string p);
    list<string> GetFollowers();
    void RemoveFollower(string p);
    void AddReceivedNotification(ReceivedNotification r);
    void AddPendingNotification(PendingNotification p);
    list<ReceivedNotification> GetReceivedNotifications();
    list<PendingNotification> GetPendingNotification();
    void RemoveReceivedNotification(int r);
    void RemovePendingNotification(string p);
};