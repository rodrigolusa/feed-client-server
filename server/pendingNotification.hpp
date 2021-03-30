#pragma once
#include <string>

using namespace std;

class PendingNotification{
    public:
    string profileId;
    int notificationId;
    int readings;
    
    PendingNotification(string p, int n);
    PendingNotification();
};