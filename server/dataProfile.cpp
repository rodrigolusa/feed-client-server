#include "dataProfile.hpp"


using namespace std;

DataProfile::DataProfile(Profile i){
    id = i.id;
    p = i;     
};

void DataProfile::AddFollower(string p){
    list<string>::iterator it;
    it = followers.end();
    followers.insert(it, p);
}

list<string> DataProfile::GetFollowers(){
    return followers;
}

void DataProfile::RemoveFollower(string p){  
    list<string>::iterator it;
    it = followers.end();
    for(it = followers.begin(); it!= followers.end(); it++){
        
        if(*it == p){
            followers.erase(it);
            break;
        }
    } 
}

void DataProfile::AddReceivedNotification(ReceivedNotification r){
    list<ReceivedNotification>::iterator it;
    it = receivedNotifications.end();
    receivedNotifications.insert(it, r);
}

void DataProfile::AddPendingNotification(PendingNotification p){
    list<PendingNotification>::iterator it;
    it = pendingNotifications.end();
    pendingNotifications.insert(it, p);
}

list<ReceivedNotification> DataProfile::GetReceivedNotifications(){
    return receivedNotifications;
}

list<PendingNotification> DataProfile::GetPendingNotification(){
    return pendingNotifications;
}

void DataProfile::RemoveReceivedNotification(int r){
    list<ReceivedNotification>::iterator it;
    it = receivedNotifications.end();
    for(it = receivedNotifications.begin(); it!= receivedNotifications.end(); it++){
        
        if(it->id == r){
            receivedNotifications.erase(it);
            break;
        }
    }
}

void DataProfile::RemovePendingNotification(string p){
    list<PendingNotification>::iterator it;
    it = pendingNotifications.end();
    for(it = pendingNotifications.begin(); it!= pendingNotifications.end(); it++){
        
        if(it->profileId == p){
            pendingNotifications.erase(it);
            break;
        }
    } 
}