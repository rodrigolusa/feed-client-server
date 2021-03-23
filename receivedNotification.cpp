#include "receivedNotification.hpp"

using namespace std;

class receivedNotification{
    int id;
    int timestamp;
    string message;
    int size;
    int pendingFollowersToReceive;

    public:
    receivedNotification();
};

receivedNotification::receivedNotification(){}