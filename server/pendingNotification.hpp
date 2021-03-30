#pragma once
#include <string>

using namespace std;

class PendingNotification{
    public:
    string profileId;
    int notificationId;
    int last_read_by;

    PendingNotification(string p, int n);
    PendingNotification();
};
