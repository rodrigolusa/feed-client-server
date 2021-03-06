#include "profile.hpp"
#include <iostream>

using namespace std;

Profile::Profile(){
  //activeSessions = 0;
  //pthread_mutex_init(&logincontrol_mutex,NULL);
  //pthread_mutex_init(&pendingnotification_mutex,NULL);
  //pthread_mutex_init(&receivenotification_mutex,NULL);
  //pthread_cond_init(&not_empty, NULL);
}

Profile::Profile(string i){
    id = i;
    activeSessions = 0;
    pthread_mutex_init(&logincontrol_mutex,NULL);
    pthread_mutex_init(&pendingnotification_mutex,NULL);
    pthread_mutex_init(&receivenotification_mutex,NULL);
    pthread_cond_init(&not_empty, NULL);
    for(int i = 0; i < 2;i++){
      backup_hosts[i] = "-1";
      backup_ports[i] = -1;
    }
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

bool Profile::AddReceivedNotification(ReceivedNotification r){
    if(pthread_mutex_lock(&(this->receivenotification_mutex)) == 0){
    list<ReceivedNotification>::iterator it;
    it = receivedNotifications.end();
    receivedNotifications.insert(it, r);
    pthread_mutex_unlock(&(this->receivenotification_mutex));
    return true;
  }
  else return false;
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

    this->pendingNotifications.erase(std::remove_if(this->pendingNotifications.begin(), this->pendingNotifications.end(), [&p,i](PendingNotification notif){ return (notif.profileId == p && notif.notificationId == i); }), this->pendingNotifications.end());


}
