#include <string>

using namespace std;

class receivedNotification{
    public:
    int id;
    int timestamp;
    string message;
    int size;
    int pendingFollowersToReceive;
};