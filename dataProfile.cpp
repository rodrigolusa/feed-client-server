#include "dataProfile.hpp"


using namespace std;

class dataProfile{
    string id;
    profile followers[];
    receivedNotification receivedNotifications [];
    pendingNotification pendingNotifications [];

    public:
    dataProfile();
};

dataProfile::dataProfile(){}