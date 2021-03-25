#include "receivedNotification.hpp"

using namespace std;

ReceivedNotification::ReceivedNotification(int i, int t, string m, int s, int p){
    id = i;
    timestamp = t;
    message = m;
    size = s;
    pendingFollowersToReceive = p;
};
