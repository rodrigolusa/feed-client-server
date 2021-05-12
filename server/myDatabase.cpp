#include "myDatabase.hpp"


using namespace std;

MyDatabase database;
int notificationId = 0;
MyDatabase::MyDatabase(){
};

void MyDatabase::AddProfile(Profile d){
    list<Profile>::iterator it;
    it = data.end();
    data.insert(it, d);
}


void MyDatabase::UpdateProfileInFile(string profile, string oldhost, int oldport, string newhost ,int newport){
  f_profiles.open("profiles.txt",fstream::in); //opens for appending at the end of file
  string id,port,host, dataToWrite,line;
  fstream temp;
  temp.open("temp_profiles.txt",fstream::out);
  dataToWrite.append(profile);
  dataToWrite.append(" ");
  while(getline(f_profiles,line)){
  stringstream lineStream(line);
  lineStream >> id;
    if(id == profile){
        lineStream >> host;
        lineStream >> port;
        if(host == oldhost && stoi(port) == oldport)
          {
              dataToWrite.append(newhost);
              dataToWrite.append(" ");
              dataToWrite.append(to_string(newport)); //add new information
              dataToWrite.append(" ");
              host.clear();
              port.clear();
              lineStream >> host;
              lineStream >> port;
              dataToWrite.append(host);//append info about other session
              dataToWrite.append(" ");
              dataToWrite.append(port);
              dataToWrite.append("\n");
              temp << dataToWrite; //write to file.

          }
          else{//if it is not the first host-port pair, it is the last.
            dataToWrite.append(host);//append info about other session
            dataToWrite.append(" ");
            dataToWrite.append(port);
            dataToWrite.append(" ");
            dataToWrite.append(newhost);
            dataToWrite.append(" ");
            dataToWrite.append(to_string(newport)); //add new information
            dataToWrite.append("\n");
            temp << dataToWrite; //write to file.
          }
    }

    else
    {
      line.append("\n");
      temp << line;
    continue;
    }
  }
  f_profiles.close();
  temp.close();
  remove("profiles.txt");
  rename("temp_profiles.txt","profiles.txt");
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

void MyDatabase::AddSessionCount(string id,string host, int port){
    list<Profile>::iterator it;
    for(it = data.begin(); it!= data.end(); it++){
        if(it->id == id){
          if(port != -1)
            it->activeSessions++;
            for(int i = 0; i < 2;i++){
              if(it->backup_ports[i] == -1){
                it->backup_ports[i] = port;
                it->backup_hosts[i] = host;
                break;
              }
            }
            break;
        }
    }
}

void MyDatabase::SubtractSessionCount(string id,string host,int port){
    list<Profile>::iterator it;
    for(it = data.begin(); it!= data.end(); it++){
        if(it->id == id){
            it->activeSessions--;
            for(int i = 0; i < 2;i++){
              if(it->backup_ports[i] == port){
                it->backup_ports[i] = -1;
              }
            }
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

int MyDatabase::GetActiveFollowersNumber(string profile){
    list<string> followers;
    list<Profile>::iterator it;
    list<string>::iterator it2;
    int count = 0;
    int activeNum;
    for(it = data.begin(); it!= data.end(); it++){
        if(it->id == profile){
            followers = it->GetFollowers();
            break;
        }
    }
    for(it2 = followers.begin(); it2!= followers.end(); it2++){
      activeNum  = GetActiveSessionsNumber(*it2);
      if(activeNum > 0)
        count++;
      }
    return count;
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
    return;
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

ReceivedNotification* MyDatabase::GetReceivedNotification(string profile, int id){
    ReceivedNotification* ret;
    list<Profile>::iterator it;
    for(it = data.begin(); it!= data.end(); it++){
        if(it->id == profile){
            list<ReceivedNotification>::iterator it_r;
            for(it_r = it->receivedNotifications.begin(); it_r != it->receivedNotifications.end(); it_r++){
                if(it_r->id == id){
                    ret = &(*it_r);
                    break;
                }
            }
            break;
        }
    }
    return ret;
}

PendingNotification* MyDatabase::GetPendingNotification(string follower,string profile, int id){
    PendingNotification* ret;
    list<Profile>::iterator it;
    for(it = data.begin(); it!= data.end(); it++){
        if(it->id == follower){
            list<PendingNotification>::iterator it_r;
            for(it_r = it->pendingNotifications.begin(); it_r != it->pendingNotifications.end(); it_r++){
                if(it_r->notificationId == id && it_r->profileId == profile){
                    ret = &(*it_r);
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

bool MyDatabase::AddPendingNotificationInFollower(string follower, PendingNotification pn){
    list<Profile>::iterator it;
    for(it = data.begin(); it!= data.end(); it++){
        if(it->id == follower){
            if(pthread_mutex_lock(&(it->pendingnotification_mutex)) == 0){
              it->AddPendingNotification(pn);
              pthread_mutex_unlock(&(it->pendingnotification_mutex));
              pthread_cond_broadcast(&(it->not_empty));
              return true;
            }
            return false;
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
void MyDatabase::PrintDatabase(){
    list<Profile>::iterator it;
    for(it = data.begin(); it!= data.end(); it++){
        cout << "Perfil: " << it->id << endl;
        cout << "Host1: " << it->backup_hosts[0] << " ";
        cout << "Port1: " << it->backup_ports[0] << endl;
        cout << "Host2: " << it->backup_hosts[1] << " ";
        cout << "Port2: " << it->backup_ports[1] << endl;


        cout << "  Seguidores: " << endl;
        list<string>::iterator it_seguidores;
        for(it_seguidores = it->followers.begin(); it_seguidores != it->followers.end(); it_seguidores++){
            cout << "    " << *it_seguidores << endl;
        }

        cout << "  Notificações recebidas: " << endl;
        list<ReceivedNotification>::iterator it_notifications;
        for(it_notifications = it->receivedNotifications.begin(); it_notifications != it->receivedNotifications.end(); it_notifications++){
            cout << "    " << it_notifications->id << ", " << getDate(it_notifications->timestamp) << ", " << it_notifications->message << ", " << it_notifications->size << ", " << it_notifications->pendingFollowersToReceive << endl;
        }

        cout << "  Notificações pendentes para: " << endl;
        list<PendingNotification>::iterator it_pending;
        for(it_pending = it->pendingNotifications.begin(); it_pending != it->pendingNotifications.end(); it_pending++){
            cout << "    " << it_pending->profileId << endl;
        }

        cout << endl;
    }
}

void MyDatabase::ReadDatabase(string file){

}

void MyDatabase::WriteProfile(string id,string hostname, int port){
  f_profiles.open("profiles.txt",fstream::app); //opens for appending at the end of file
  string dataToWrite(id);
  dataToWrite.append(" ");
  dataToWrite.append(hostname);
  dataToWrite.append(" ");
  dataToWrite.append(to_string(port));
  dataToWrite.append(" ");
  dataToWrite.append(to_string(-1));
  dataToWrite.append(" ");
  dataToWrite.append(to_string(-1));
  dataToWrite.append("\n");
  f_profiles << dataToWrite.c_str();
  f_profiles.close();
}
void MyDatabase::initProfiles(){
  string profile,host1,host2, port1,port2, line;

  while(getline(f_profiles,line)){
  stringstream lineStream(line);
  lineStream >> profile;
  lineStream >> host1;
  lineStream >> port1;
  lineStream >> host2;
  lineStream >> port2;
  cout << host2 << endl;
  AddProfile(profile);
  AddSessionCount(profile,host1,stoi(port1));
  AddSessionCount(profile,host2,stoi(port2));
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
  f_followers.open("followers.txt",fstream::app);
  string dataToWrite(follower);
  dataToWrite.append(" ");
  dataToWrite.append(followed);
  dataToWrite.append(" ");
  f_followers << dataToWrite;
  f_followers.close();
}

void MyDatabase::WritePendingFile(string follower, PendingNotification pn){
  pending_notifs_f.open("pending-notif.txt",fstream::app);
  string dataToWrite(follower);
  dataToWrite.append(" ");
  dataToWrite.append(pn.profileId);
  dataToWrite.append(" ");
  dataToWrite.append(to_string(pn.notificationId));
  dataToWrite.append(" ");
  dataToWrite.append(to_string(pn.last_read_by));
  dataToWrite.append("\n");
  pending_notifs_f << dataToWrite;
  pending_notifs_f.close();
}

void MyDatabase::WriteReceivedFile(string username, ReceivedNotification rn){ //AQUIII
  received_notifs_f.open("received-notif.txt",fstream::app);
  string dataToWrite(to_string(rn.id));
  dataToWrite.append(" ");
  dataToWrite.append(rn.timestamp);
  dataToWrite.append(" ");
  dataToWrite.append(";");
  dataToWrite.append(" ");
  dataToWrite.append(rn.message);
  dataToWrite.append(" ");
  dataToWrite.append(";");
  dataToWrite.append(" ");
  dataToWrite.append(to_string(rn.size));
  dataToWrite.append(" ");
  dataToWrite.append(to_string(rn.pendingFollowersToReceive));
  dataToWrite.append(" ");
  dataToWrite.append(username);
  dataToWrite.append("\n");
  received_notifs_f << dataToWrite;
  received_notifs_f.close();
}


void MyDatabase::UpdateReceivedNotificationInFile(int notificationId, int count, string profile){
  fstream temp;
  string line,id, timestamp, token, size, pendingFollowers;
  string message = "";
  string dataToWrite = "";
  received_notifs_f.open("received-notif.txt",fstream::in);
  temp.open("temp-received.txt",fstream::out);

  while(getline(received_notifs_f,line)){
  stringstream lineStream(line);
  lineStream >> id;
    if(stoi(id) == notificationId)
      {
        lineStream >>timestamp;
        lineStream >> token;
        do{
          lineStream >> token;
          message += token;
          lineStream >> token;
        }while(token != ";");
        lineStream >> size;
        lineStream >> pendingFollowers;
        dataToWrite = id + " " + timestamp + " " + ";" + " " + message + " " + ";" + " " + size + " " + to_string(count) + " " + profile + "\n";
        temp << dataToWrite;
      }
    else
    {
      line.append("\n");
      temp << line;
    continue;
    }
  }
  received_notifs_f.close();
  temp.close();
  remove("received-notif.txt");
  rename("temp-received.txt","received-notif.txt");
}



void MyDatabase::RemoveReceivedFromFile(int idToRemove){
  fstream temp;
  string line,id, dataToWrite;

  received_notifs_f.open("received-notif.txt",fstream::in);
  temp.open("temp-received.txt",fstream::out);

  while(getline(received_notifs_f,line)){
  stringstream lineStream(line);
  lineStream >> id;
    if(stoi(id) == idToRemove)
      continue;
    else
    {
      line.append("\n");
      temp << line;
    continue;
    }
  }
  received_notifs_f.close();
  temp.close();
  remove("received-notif.txt");
  rename("temp-received.txt","received-notif.txt");
}

void MyDatabase::RemovePendingsFile(PendingNotification* pendingstoRemove, int removeSize, string profile, PendingNotification* pendingstoUpdate,int updateSize,int port){
  fstream temp;
  string line,id, dataToWrite, username,follower;
  bool removeBool = false;
  bool update = false;
  pending_notifs_f.open("pending-notif.txt",fstream::in);
  temp.open("temp-notif.txt",fstream::out);

  while(getline(pending_notifs_f,line)){
  stringstream lineStream(line);
  lineStream >> follower;
  lineStream >> username;
  lineStream >> id;
  for(int i = 0; i < removeSize; i ++){
      if(pendingstoRemove[i].profileId == username && pendingstoRemove[i].notificationId == stoi(id) && profile == follower){
        removeBool = true;
        break;
      }
    }

    for(int i = 0; i < updateSize; i ++){
        if(pendingstoUpdate[i].profileId == username && pendingstoUpdate[i].notificationId == stoi(id) && profile == follower){
          dataToWrite.clear();
          dataToWrite = follower + " " + username + " " + id + " " + to_string(port) + "\n";
          temp << dataToWrite;
          break;
        }
      }
    if(removeBool || update){
      removeBool = false;
      update = false;
      continue;
    }
    else
    {
      line.append("\n");
      temp << line;
    continue;
  }
}
  pending_notifs_f.close();
  temp.close();
  remove("pending-notif.txt");
  rename("temp-notif.txt","pending-notif.txt");
}



void MyDatabase::initPendingNotif(){
  string profile;
  string follower;
  string notificationId;
  string last_read_by;

  while(pending_notifs_f >> follower){
    pending_notifs_f >> profile;
    pending_notifs_f >> notificationId;
    pending_notifs_f >> last_read_by;
    PendingNotification pn;
    pn.profileId = profile;
    pn.notificationId = stoi(notificationId);
    pn.last_read_by = stoi(last_read_by);
    AddPendingNotificationInFollower(follower,pn);
  }
}

void MyDatabase::initReceivedNotif(){//AQUIII
  string id;
  string timestamp;
  string message = "";
  string size;
  string pendingFollowersToReceive;
  string username;
  string token ="";
  string discard;
  int idNum = 0;
  while(received_notifs_f >> id){
    received_notifs_f >> timestamp;
    received_notifs_f >> discard;
    do{
      message+= token;
      received_notifs_f >> token;
      if(token != ";")
        message += " ";
    }while(token != ";");
    received_notifs_f >> size;
    received_notifs_f >> pendingFollowersToReceive;
    received_notifs_f >> username;
    ReceivedNotification rn;
    rn.id = stoi(id);
    rn.timestamp = (char*)timestamp.c_str();
    rn.message = message;
    rn.size=  stoi(size);
    rn.pendingFollowersToReceive = stoi(pendingFollowersToReceive);
    idNum = rn.id;

    AddReceivedNotifications(username,rn);
    message.clear();
    token.clear();
  }
  notificationId = idNum; //last notification id is the highest.
}
void MyDatabase::initDatabase(){
  this->init = true;
  f_profiles.open("profiles.txt",fstream::in);//opens for reading and loading database
  f_followers.open("followers.txt",fstream::in);
  received_notifs_f.open("received-notif.txt",fstream::in);
  pending_notifs_f.open("pending-notif.txt",fstream::in);
  initProfiles();
  initFollowers();
  initPendingNotif();
  initReceivedNotif();
  f_profiles.close();
  f_followers.close();
  received_notifs_f.close();
  pending_notifs_f.close();
  this->init = false;
  PrintDatabase();
}
