#include "profile.hpp"


using namespace std;

Profile::Profile(){
    
}

Profile::Profile(string i){
    id = i;   
};

void Profile::AddFollower(string p){
    bool exists = false;
    list<string>::iterator it;
    for(it = followers.begin(); it!= followers.end(); it++){
        if(*it == p){
            exists = true;
            break;
        }
    }
    if(!exists){
        it = followers.end();
        followers.insert(it, p);
    }
}

list<string> Profile::GetFollowers(){
    return followers;
}

void Profile::RemoveFollower(string p){  
    list<string>::iterator it;
    it = followers.end();
    for(it = followers.begin(); it!= followers.end(); it++){
        
        if(*it == p){
            followers.erase(it);
            break;
        }
    } 
}

void Profile::AddFollowing(string p){
    bool exists = false;
    list<string>::iterator it;
    for(it = following.begin(); it!= following.end(); it++){
        if(*it == p){
            exists = true;
            break;
        }
    }
    if(!exists){
        it = following.end();
        following.insert(it, p);
    }
}

list<string> Profile::GetFollowing(){
    return following;
}

void Profile::RemoveFollowing(string p){  
    list<string>::iterator it;
    it = following.end();
    for(it = following.begin(); it!= following.end(); it++){
        
        if(*it == p){
            following.erase(it);
            break;
        }
    } 
}

int Profile::GetFollowersNumber(){
    return followers.size();
}

void Profile::AddReceivedNotification(ReceivedNotification r){
    list<ReceivedNotification>::iterator it;
    it = receivedNotifications.end();
    receivedNotifications.insert(it, r);
}

void Profile::AddPendingNotification(PendingNotification p){
    list<PendingNotification>::iterator it;
    it = pendingNotifications.end();
    pendingNotifications.insert(it, p);
}

list<ReceivedNotification> Profile::GetReceivedNotifications(){
    return receivedNotifications;
}

list<PendingNotification> Profile::GetPendingNotification(){
    return pendingNotifications;
}

void Profile::RemoveReceivedNotification(int r){
    list<ReceivedNotification>::iterator it;
    it = receivedNotifications.end();
    for(it = receivedNotifications.begin(); it!= receivedNotifications.end(); it++){       
        if(it->id == r){
            receivedNotifications.erase(it);
            break;
        }
    }
}

void Profile::RemovePendingNotification(string p, int i){
    list<PendingNotification>::iterator it;
    it = pendingNotifications.end();
    for(it = pendingNotifications.begin(); it!= pendingNotifications.end(); it++){
        
        if(it->profileId == p && it->notificationId == i){
            pendingNotifications.erase(it);
            break;
        }
    } 
}