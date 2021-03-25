#include <string>
#include <list>
#include "profile.hpp"
#include "receivedNotification.hpp"
#include "pendingNotification.hpp"

using namespace std;

class DataProfile{
    public:
    string id;

    list<string> followers;
    list<ReceivedNotification> receivedNotifications;
    list<PendingNotification> pendingNotifications;
    
    DataProfile(Profile i);
    void AddFollower(string p);
    list<string> GetFollowers();
    void RemoveFollower(string p);
    void AddReceivedNotification(ReceivedNotification r);
    void AddPendingNotification(PendingNotification p);
    list<ReceivedNotification> GetReceivedNotifications();
    list<PendingNotification> GetPendingNotification();
    void RemoveReceivedNotification(int r);
    void RemovePendingNotification(string p);
};