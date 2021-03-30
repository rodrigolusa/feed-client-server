#include "pendingNotification.hpp"

using namespace std;


PendingNotification::PendingNotification(string p, int n){
    profileId = p;
    notificationId = n;
    readings = 0;
};
PendingNotification::PendingNotification(){
};