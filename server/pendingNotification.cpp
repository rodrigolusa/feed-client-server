#include "pendingNotification.hpp"

using namespace std;


PendingNotification::PendingNotification(string p, int n){
    profileId = p;
    notificationId = n;
    last_read_by = -1;
};
PendingNotification::PendingNotification(){
  last_read_by = -1;
};
