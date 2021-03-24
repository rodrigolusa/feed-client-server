#include <string>
#include <list>
#include "profile.hpp"
#include "receivedNotification.hpp"
#include "pendingNotification.hpp"

using namespace std;

class dataProfile{
    public:
    string id;
    list<profile> followers;
    list<receivedNotification> receivedNotifications;
    list<pendingNotification> pendingNotifications;
};