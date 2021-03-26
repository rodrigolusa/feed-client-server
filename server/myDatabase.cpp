#include "myDatabase.hpp"


using namespace std;

MyDatabase::MyDatabase(){
};

void MyDatabase::AddProfile(Profile d){
    list<Profile>::iterator it;
    it = data.end();
    data.insert(it, d);
}

void MyDatabase::AddFollower(string profile, string follow){
    list<Profile>::iterator it;
    for(it = data.begin(); it!= data.end(); it++){
        if(it->id == profile){
            it->AddFollower(follow);
            break;
        }
    }
}

void MyDatabase::AddFollowing(string profile, string follow){
    list<Profile>::iterator it;
    for(it = data.begin(); it!= data.end(); it++){
        if(it->id == profile){
            it->AddFollowing(follow);
            break;
        }
    }
}

list<string> MyDatabase::GetFollowers(string profile){
    list<string> ret;
    list<Profile>::iterator it;
    for(it = data.begin(); it!= data.end(); it++){
        if(it->id == profile){
            ret = it->GetFollowers();
            break;
        }
    }
    return ret;
}

list<string> MyDatabase::GetFollowing(string profile){
    list<string> ret;
    list<Profile>::iterator it;
    for(it = data.begin(); it!= data.end(); it++){
        if(it->id == profile){
            ret = it->GetFollowing();
            break;
        }
    }
    return ret;
}

list<ReceivedNotification> MyDatabase::GetReceivedNotifications(string profile){
    list<ReceivedNotification> ret;
    list<Profile>::iterator it;
    for(it = data.begin(); it!= data.end(); it++){
        if(it->id == profile){
            ret = it->GetReceivedNotifications();
            break;
        }
    }
    return ret;
}

list<PendingNotification> MyDatabase::GetPendingNotifications(string profile){
    list<PendingNotification> ret;
    list<Profile>::iterator it;
    for(it = data.begin(); it!= data.end(); it++){
        if(it->id == profile){
            ret = it->GetPendingNotification();
            break;
        }
    }
    return ret;
}

void MyDatabase::AddReceivedNotifications(string profile, ReceivedNotification rn){
    list<Profile>::iterator it;
    for(it = data.begin(); it!= data.end(); it++){
        if(it->id == profile){
            it->AddReceivedNotification(rn);
            break;
        }
    }
}

void MyDatabase::AddPendingNotifications(string profile, PendingNotification pn){
    list<Profile>::iterator it;
    for(it = data.begin(); it!= data.end(); it++){
        if(it->id == profile){
            it->AddPendingNotification(pn);
            break;
        }
    }
}

void MyDatabase::RemoveReceivedNotifications(string profile, int id){
    list<Profile>::iterator it;
    for(it = data.begin(); it!= data.end(); it++){
        if(it->id == profile){
            it->RemoveReceivedNotification(id);
            break;
        }
    }
}

void MyDatabase::RemovePendingNotifications(string profile, string who, int id){
    list<Profile>::iterator it;
    for(it = data.begin(); it!= data.end(); it++){
        if(it->id == profile){
            it->RemovePendingNotification(who, id);
            break;
        }
    }
}
