#pragma once
#include <string>

using namespace std;

class PendingNotification{
    public:
    string profileId;
    int notificationId;
    
    PendingNotification(string p, int n);
};