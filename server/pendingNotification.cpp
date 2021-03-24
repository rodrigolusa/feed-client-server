#include "pendingNotification.hpp"

using namespace std;

class pendingNotification{
    string profileId;
    int notificationId;

    public:
    pendingNotification(string p, int n){
        profileId = p;
        notificationId = n;
    };
};
