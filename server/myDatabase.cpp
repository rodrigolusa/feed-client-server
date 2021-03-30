#include "myDatabase.hpp"


using namespace std;

MyDatabase::MyDatabase(){
};

void MyDatabase::AddProfile(Profile d){
    list<Profile>::iterator it;
    it = data.end();
    data.insert(it, d);
    if(!init)
      WriteProfile(d.id);
}

Profile* MyDatabase::getProfile(string p){
  list<Profile>::iterator it;
  if(data.size() > 0){
      for(it = data.begin(); it!= data.end(); it++){
          if(it->id == p){
              return &(*it);
              break;
          }
      }
  }
  else{
  return NULL;
  }
}

bool MyDatabase::ExistsProfile(string p){
    list<Profile>::iterator it;
    //cout << data.size() << endl;
    if(data.size() > 0){
        for(it = data.begin(); it!= data.end(); it++){
            if(it->id == p){
                return true;
                break;
            }
        }
    }
    else{
    return false;
    }
}

int MyDatabase::GetFollowersNumber(string p){
    list<Profile>::iterator it;
    for(it = data.begin(); it!= data.end(); it++){
        if(it->id == p){
            return it->GetFollowersNumber();
            break;
        }
    }
    return 0;
}

int MyDatabase::GetActiveSessionsNumber(string id){
    list<Profile>::iterator it;
    for(it = data.begin(); it!= data.end(); it++){
        if(it->id == id){
            return it->activeSessions;
            break;
        }
    }
    return -1;
}

void MyDatabase::AddSessionCount(string id){
    list<Profile>::iterator it;
    for(it = data.begin(); it!= data.end(); it++){
        if(it->id == id){
            it->activeSessions++;
            break;
        }
    }
}

void MyDatabase::SubtractSessionCount(string id){
    list<Profile>::iterator it;
    for(it = data.begin(); it!= data.end(); it++){
        if(it->id == id){
            it->activeSessions--;
            break;
        }
    }
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
            if(!init)
              WriteFollower(profile,follow);
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

ReceivedNotification MyDatabase::GetReceivedNotification(string profile, int id){
    ReceivedNotification ret;
    list<Profile>::iterator it;
    for(it = data.begin(); it!= data.end(); it++){
        if(it->id == profile){
            list<ReceivedNotification>::iterator it_r;
            for(it_r = it->receivedNotifications.begin(); it_r != it->receivedNotifications.end(); it_r++){
                if(it_r->id == id){
                    ret = *it_r;
                    break;
                }
            }
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
            list<string>::iterator it_f;
            for(it_f = it->followers.begin(); it_f != it->followers.end(); it_f++){
                AddPendingNotificationInFollower(*it_f, pn);
            }
            break;
        }
    }
}

void MyDatabase::AddPendingNotificationInFollower(string follower, PendingNotification pn){
    list<Profile>::iterator it;
    for(it = data.begin(); it!= data.end(); it++){
        if(it->id == follower){
            it->AddPendingNotification(pn);
            pthread_cond_broadcast(&(it->not_empty));
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

//Metodo utilizado para teste
void MyDatabase::WriteDatabase(string file){
    list<Profile>::iterator it;
    for(it = data.begin(); it!= data.end(); it++){
        cout << "Perfil: " << it->id << endl;

        cout << "  Seguidores: " << endl;
        list<string>::iterator it_seguidores;
        for(it_seguidores = it->followers.begin(); it_seguidores != it->followers.end(); it_seguidores++){
            cout << "    " << *it_seguidores << endl;
        }

        cout << "  Notificações recebidas: " << endl;
        list<ReceivedNotification>::iterator it_notifications;
        for(it_notifications = it->receivedNotifications.begin(); it_notifications != it->receivedNotifications.end(); it_notifications++){
            cout << "    " << it_notifications->id << ", " << it_notifications->timestamp << ", " << it_notifications->message << ", " << it_notifications->size << ", " << it_notifications->pendingFollowersToReceive << endl;
        }

        cout << "  Notificações pendentes para: " << endl;
        list<PendingNotification>::iterator it_pending;
        for(it_pending = it->pendingNotifications.begin(); it_pending != it->pendingNotifications.end(); it_pending++){
            cout << "    " << it_pending->profileId << ", " << it_pending->profileId << endl;
        }

        cout << endl;
    }
}

void MyDatabase::ReadDatabase(string file){

}

void MyDatabase::WriteProfile(string id){
  string dataToWrite(id);
  dataToWrite.append(" ");
  f_profiles << dataToWrite.c_str();
  f_profiles.flush();
}
void MyDatabase::initProfiles(){
  string profile;
  while(f_profiles >> profile){
    AddProfile(profile);
  }
}

void MyDatabase::initFollowers(){
  string follower;
  string followed;

  while(f_followers >> follower){
    f_followers >> followed;
    AddFollowing(follower,followed);
    AddFollower(followed,follower);
  }
}
void MyDatabase::WriteFollower(string follower, string followed){
  string dataToWrite(follower);
  dataToWrite.append(" ");
  dataToWrite.append(followed);
  dataToWrite.append(" ");
  f_followers << dataToWrite;
  f_followers.flush();
}

void MyDatabase::initDatabase(){
  this->init = true;
  f_profiles.open("profiles.txt",fstream::in);//opens for reading and creating database
  f_followers.open("followers.txt",fstream::in);
  initProfiles();
  initFollowers();
  f_profiles.close();
  f_followers.close();
  f_profiles.open("profiles.txt",fstream::app); //opens for appending at the end of file
  f_followers.open("followers.txt",fstream::app);
  this->init = false;
  WriteDatabase("");
}
