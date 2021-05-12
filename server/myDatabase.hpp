#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <list>
#include <algorithm>
#include "profile.hpp"
#include <pthread.h>
#include "../common/basiccomm.hpp"

using namespace std;

class MyDatabase{
public:
    list<Profile> data;
    fstream f_followers;
    fstream f_profiles;
    fstream received_notifs_f;
    fstream pending_notifs_f;
    bool init;

    MyDatabase();
    void AddProfile(Profile d);
    Profile* getProfile(string name);
    bool ExistsProfile(string id);
    int GetActiveSessionsNumber(string id);
    void AddSessionCount(string id,string host, int port);
    void SubtractSessionCount(string id,string host, int port);
    void AddFollower(string profile, string follow);
    void AddFollowing(string profile, string follow);
    list<string> GetFollowers(string profile);
    list<string> GetFollowing(string profile);
    list<ReceivedNotification> GetReceivedNotifications(string profile);
    list<PendingNotification> GetPendingNotifications(string profile);
    ReceivedNotification* GetReceivedNotification(string profile, int id);
    void AddReceivedNotifications(string profile, ReceivedNotification rn);
    void AddPendingNotifications(string profile, PendingNotification pn);
    bool AddPendingNotificationInFollower(string follower, PendingNotification pn);
    void RemoveReceivedNotifications(string profile, int id);
    void RemovePendingNotifications(string profile, string who, int id);
    void PrintDatabase();
    void ReadDatabase(string file);
    int GetFollowersNumber(string profile);
    int GetReadings(string profile);
    void IncrementReading(string profile);
    void WriteProfile(string id,string hostname, int port);
    void initProfiles();
    void initFollowers();
    void WriteFollower(string follower, string followed);
    void initDatabase();
    void RemovePendingsFile(PendingNotification* pendingstoRemove, int removeSize, string profile, PendingNotification* pendingstoUpdate = NULL,int updateSize = 0 ,int port = 0);
    void WriteReceivedFile(string username,ReceivedNotification rn);
    void WritePendingFile(string follower,PendingNotification pn);
    void RemoveReceivedFromFile(int idToRemove);
    void initPendingNotif();
    void initReceivedNotif();
    void UpdateProfileInFile(string profile, string oldhost, int oldport, string newhost = "-1",int newport = -1);
    int GetActiveFollowersNumber(string profile);
    void UpdateReceivedNotificationInFile(int notificationId, int count,string profile);
    PendingNotification* GetPendingNotification(string follower,string profile, int id);
};
extern MyDatabase database;
extern int notificationId;
