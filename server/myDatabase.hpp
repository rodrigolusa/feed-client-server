#pragma once
#include <string>
#include <iostream>
#include <list>
#include "profile.hpp"
#include <pthread.h>

using namespace std;

class MyDatabase{
public:
    list<Profile> data;

    MyDatabase();
    void AddProfile(Profile d);
    Profile* getProfile(string name);
    bool ExistsProfile(string id);
    int GetActiveSessionsNumber(string id);
    void AddSessionCount(string id);
    void SubtractSessionCount(string id);
    void AddFollower(string profile, string follow);
    void AddFollowing(string profile, string follow);
    list<string> GetFollowers(string profile);
    list<string> GetFollowing(string profile);
    list<ReceivedNotification> GetReceivedNotifications(string profile);
    list<PendingNotification> GetPendingNotifications(string profile);
    ReceivedNotification GetReceivedNotification(string profile, int id);
    void AddReceivedNotifications(string profile, ReceivedNotification rn);
    void AddPendingNotifications(string profile, PendingNotification pn);
    void AddPendingNotificationInFollower(string follower, PendingNotification pn);
    void RemoveReceivedNotifications(string profile, int id);
    void RemovePendingNotifications(string profile, string who, int id);
    void WriteDatabase(string file);
    void ReadDatabase(string file);
    int GetFollowersNumber(string profile);
    int GetReadings(string profile);
    void IncrementReading(string profile);
};
