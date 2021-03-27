#pragma once
#include <string>
#include <list>
#include "profile.hpp"

using namespace std;

class MyDatabase{
public:
    list<Profile> data;

    MyDatabase();
    void AddProfile(Profile d);
    void AddFollower(string profile, string follow);
    void AddFollowing(string profile, string follow);
    list<string> GetFollowers(string profile);
    list<string> GetFollowing(string profile);
    list<ReceivedNotification> GetReceivedNotifications(string profile);
    list<PendingNotification> GetPendingNotifications(string profile);
    void AddReceivedNotifications(string profile, ReceivedNotification rn);
    void AddPendingNotifications(string profile, PendingNotification pn);
    void RemoveReceivedNotifications(string profile, int id);
    void RemovePendingNotifications(string profile, string who, int id);   
};