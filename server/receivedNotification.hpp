#pragma once
#include <string>

using namespace std;

class ReceivedNotification{
    public:
    int id;
    char* timestamp;
    string message;
    int size;
    int pendingFollowersToReceive;

    ReceivedNotification(int i, char* t, string m, int s, int p);
    ReceivedNotification();
};
