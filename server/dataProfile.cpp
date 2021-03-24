#include "dataProfile.hpp"


using namespace std;

class dataProfile{
    profile prof;
    list<string> followers;
    list<receivedNotification> receivedNotifications;
    list<pendingNotification> pendingNotifications;

    public:
    dataProfile(profile i){
        prof = i;       
    };

    void addFollower(string p){
        list<string>::iterator it;
        it = followers.end();
        followers.insert(it, p);
    }

    list<string> getFollowers(){
        return followers;
    }

    void removeFollower(string p){  
        list<string>::iterator it;
        it = followers.end();
        for(it = followers.begin(); it!= followers.end(); it++){
            
            if(*it == p){
                followers.erase(it);
            }
        } 
    }

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

    list<receivedNotification> getReceivedNotifications(){
        return receivedNotifications;
    }

    list<pendingNotification> getPendingNotification(){
        return pendingNotifications;
    }

    void removeReceivedNotification(int r){
        list<receivedNotification>::iterator it;
        it = receivedNotifications.end();
        for(it = receivedNotifications.begin(); it!= receivedNotifications.end(); it++){
            
            if(it->id == r){
                receivedNotifications.erase(it);
            }
        }
    }

    void removePendingNotification(string p){
        list<pendingNotification>::iterator it;
        it = pendingNotifications.end();
        for(it = pendingNotifications.begin(); it!= pendingNotifications.end(); it++){
            
            if(it->profileId == p){
                pendingNotifications.erase(it);
            }
        } 
    }

};