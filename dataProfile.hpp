#include <string>
#include "profile.hpp"
#include "receivedNotification.hpp"
#include "pendingNotification.hpp"

using namespace std;

class dataProfile{
    public:
    string id;
    profile followers[];
    receivedNotification receivedNotifications [];
    pendingNotification pendingNotifications [];
};