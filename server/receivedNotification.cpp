#include "receivedNotification.hpp"

using namespace std;

class receivedNotification{
    int id;
    int timestamp;
    string message;
    int size;
    int pendingFollowersToReceive;

    public:
    receivedNotification(int i, int t, string m, int s, int p){
        id = i;
        timestamp = t;
        message = m;
        size = s;
        pendingFollowersToReceive = p;
    };
};
