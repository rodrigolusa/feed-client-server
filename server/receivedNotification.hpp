#pragma once
#include <string>

using namespace std;

class ReceivedNotification{
    public:
    int id;
    int timestamp;
    string message;
    int size;
    int pendingFollowersToReceive;

    ReceivedNotification(int i, int t, string m, int s, int p);
    ReceivedNotification();
};