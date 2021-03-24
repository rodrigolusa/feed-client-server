#include "dataProfile.hpp"


using namespace std;

class dataProfile{
    string id;
    list<profile> followers;
    list<receivedNotification> receivedNotifications;
    list<pendingNotification> pendingNotifications;

    public:
    dataProfile(string i){
        id = i;       
    };

    void addFollow(profile p){
        list<profile>::iterator it;
        it = followers.end();
        followers.insert(it, p);
    };

    void addReceivedNotification(receivedNotification r){
        list<receivedNotification>::iterator it;
        it = receivedNotifications.end();
        receivedNotifications.insert(it, r);
    }

    void addPendingNotification(pendingNotification p){
        list<pendingNotification>::iterator it;
        it = pendingNotifications.end();
        pendingNotifications.insert(it, p);
    }

    list<profile> getFollowers(){
        return followers;
    }

    list<receivedNotification> getReceivedNotifications(){
        return receivedNotifications;
    }

    list<pendingNotification> getPendingNotification(){
        return pendingNotifications;
    }

     void removeFollow(profile p){
        /*
        list<profile>::iterator it;
        it = followers.end();
        for(it = followers.begin(); it!= followers.end(); it++){
            //TODO
            if(*it == p){
                followers.erase(it);
            }
        }
        */
    };

    void removeReceivedNotification(receivedNotification r){
        //TODO
    }

    void removePendingNotification(pendingNotification p){
        //TODO
    }

};