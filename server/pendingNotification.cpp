#include "pendingNotification.hpp"

using namespace std;


PendingNotification::PendingNotification(string p, int n){
    profileId = p;
    notificationId = n;
    last_read_by = -2;
};
PendingNotification::PendingNotification(){
  last_read_by = -2;
};
