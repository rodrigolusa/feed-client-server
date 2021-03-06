#include "notificationManager.hpp"

using namespace std;
void print(list<ReceivedNotification> const &list)
{
    for (auto const& i: list) {
        cout << i.id << endl;
    }
}


void findNotificationsToRemove(string profile, int port,replicaManager* replica){

  Profile* prof = database.getProfile(profile);
  list<PendingNotification>::iterator it;
  list<PendingNotification> pending_list;
  int removeSize = 0;
  int rmidx = 0;
  int updateSize = 0;
  int upidx = 0;
  PendingNotification* remove_vec;
  PendingNotification* update_vec;

  pending_list = prof->pendingNotifications;
  int sessions = database.GetActiveSessionsNumber(prof->id);

  if(sessions == 1){//if we have only one session, solution is trivial, we can just delete all pending notifs for user
    removeSize = prof->pendingNotifications.size();
    remove_vec = new PendingNotification[removeSize];

    for(it = pending_list.begin();it != pending_list.end();it++){
          prof = database.getProfile(it->profileId);
          string data = "";
          data.clear();
          data = profile + " " + it->profileId + " " + to_string(it->notificationId);
          replica->sendmessagetoAllReplicas(REMOVE_PENDING,(char*)data.c_str());
          pthread_mutex_lock(&(prof->receivenotification_mutex));
          ReceivedNotification* notif = database.GetReceivedNotification(it->profileId,it->notificationId);
          pthread_mutex_unlock(&(prof->receivenotification_mutex));
          if(notif->pendingFollowersToReceive == 0){
            data.clear();
            data += it->profileId;
            data += " ";
            data += to_string(notif->id);
            replica->sendmessagetoAllReplicas(REMOVE_RECEIVED,(char*)data.c_str());
            database.RemoveReceivedFromFile(notif->id);
          }
          remove_vec[rmidx] = *it;
          rmidx++;
        }

      database.RemovePendingsFile(remove_vec,removeSize,profile);
      delete[] remove_vec;
    }
    else{
      for(it = pending_list.begin();it != pending_list.end();it++){
        if(it->last_read_by == -2)
          updateSize++;
        else
          if(it->last_read_by != port)
            removeSize++;
          }

        remove_vec = new PendingNotification[removeSize];
        update_vec = new PendingNotification[updateSize];

        for(it = pending_list.begin();it != pending_list.end();it++){
          if(it->last_read_by == -2){
            update_vec[upidx] = *it;
            upidx++;
            prof = database.getProfile(it->profileId);
            string data = "";
            data.clear();
            data = profile + " " + it->profileId + " " + to_string(it->notificationId) + " " + to_string(port);
            replica->sendmessagetoAllReplicas(UPDATE_PENDING,(char*)data.c_str());
          }
          else
            if(it->last_read_by != port){
              remove_vec[rmidx] = *it;
              rmidx++;
              prof = database.getProfile(it->profileId);
              string data = "";
              data.clear();
              data = profile + " " + it->profileId + " " + to_string(it->notificationId);
              replica->sendmessagetoAllReplicas(REMOVE_PENDING,(char*)data.c_str());
            }
            pthread_mutex_lock(&(prof->receivenotification_mutex));
            ReceivedNotification* notif = database.GetReceivedNotification(it->profileId,it->notificationId);
            pthread_mutex_unlock(&(prof->receivenotification_mutex));
            if(notif->pendingFollowersToReceive == 0){
              string data;
              data.clear();
              data += it->profileId;
              data += " ";
              data += to_string(notif->id);
              replica->sendmessagetoAllReplicas(REMOVE_RECEIVED,(char*)data.c_str());
              database.RemoveReceivedFromFile(notif->id);
            }

          }

          database.RemovePendingsFile(remove_vec,removeSize,profile,update_vec,updateSize,port);
          delete[] remove_vec;
          delete[] update_vec;

    }

  }




void commitChanges(ReceivedNotification rn, list<PendingDelayed> pending, string username, replicaManager* replica){

    database.WriteReceivedFile(username,rn);
    string header(username);
    header.append(" ");
    header.append(to_string(rn.id));
    replica->sendmessagetoAllReplicas(SEND_HEADER,(char*)header.c_str(),rn.timestamp);
    replica->sendmessagetoAllReplicas(SEND_DATA,(char*)rn.message.c_str());
    for (auto const& i: pending) {
        database.WritePendingFile(i.follower,i.pn);
    }

}



void ClearNotifications(string profile, int port,replicaManager* replica){
  list<Profile>::iterator it;
  for(it = database.data.begin(); it != database.data.end(); it ++){
    if(it->id == profile){
      list<PendingNotification>::iterator it_p;

      pthread_mutex_lock(&(replica->isprimary_mutex));
      if(replica->isPrimary()){//if it's primary, we have to delete notifs from file.
        findNotificationsToRemove( profile, port,replica);
      }
        pthread_mutex_unlock(&(replica->isprimary_mutex));

    //  for(it_p = it->pendingNotifications.begin(); it_p != it->pendingNotifications.end(); it_p++){
        if(database.GetActiveSessionsNumber(profile) == 2){ //if we only have one session active, we can remove the notification
          //if(it_p->last_read_by != -1 && it_p->last_read_by != socket){
            it->pendingNotifications.erase(std::remove_if(it->pendingNotifications.begin(), it->pendingNotifications.end(), [&port](PendingNotification notif){ return (notif.last_read_by != port && notif.last_read_by != -2); }), it->pendingNotifications.end());
          //  it->RemovePendingNotification(it_p->profileId, it_p->notificationId);

            for(it_p = it->pendingNotifications.begin(); it_p != it->pendingNotifications.end(); it_p++){
              it_p->last_read_by = port;
              }
            }
        else
          it->pendingNotifications.clear();//remove if only 1 profile

      Profile* notif_prof = database.getProfile(profile);
      pthread_mutex_lock(&(notif_prof->receivenotification_mutex));
      //next line removes any receiving notification with 0 followers to receive(clean-up process)
      it->receivedNotifications.erase(std::remove_if(it->receivedNotifications.begin(), it->receivedNotifications.end(), [&](ReceivedNotification notif){ return (notif.pendingFollowersToReceive == 0); }), it->receivedNotifications.end());
      pthread_mutex_unlock(&(notif_prof->receivenotification_mutex));
      break;

}
}
}

bool islistEmptyForClient(list<PendingNotification> notf_list, int port){
  bool empty = true;
  list<PendingNotification>::iterator it_p;
  if(notf_list.empty())
    return true;
  for(it_p = notf_list.begin(); it_p != notf_list.end(); it_p++){
    if(it_p->last_read_by != port){
      empty = false;
      break;
      }
    }
    return empty;
  }

void* NotificationConsumer(void* arg){
    Session_Replica* info = (Session_Replica*) arg;
    Session* user = info->session;
    replicaManager* replica = info->replica;
    Profile* prof = database.getProfile(user->getUsername());
    while(user->isActive() && replica->isPrimary()){
      pthread_mutex_lock(&(prof->pendingnotification_mutex));//blocking behavior for consuming notification while list is empty
      while(islistEmptyForClient(prof->pendingNotifications,user->getPort())){
        pthread_cond_wait(&(prof->not_empty),&(prof->pendingnotification_mutex));
      }
      list<PendingNotification>::iterator it_p;
      for(it_p = prof->pendingNotifications.begin(); it_p != prof->pendingNotifications.end(); it_p++){
        if(it_p->last_read_by != user->getPort()){
        QueuedMessage msg;
        Profile* notif_prof = database.getProfile(it_p->profileId);
        pthread_mutex_lock(&(notif_prof->receivenotification_mutex));
        ReceivedNotification* notif = database.GetReceivedNotification(it_p->profileId,it_p->notificationId);
        cout << database.GetActiveSessionsNumber(user->getUsername()) << endl;
        if(it_p->last_read_by != -2 || (database.GetActiveSessionsNumber(user->getUsername()) == 1)){//if it is not -2 nor our socket, then another socket already read it and we can decrease it
          notif->pendingFollowersToReceive--;
          string data;
          data += it_p->profileId;
          data += " ";
          data += to_string(notif->id);
          data += " " + to_string(notif->pendingFollowersToReceive);
          replica->sendmessagetoAllReplicas(UPDATE_RECEIVED,(char*)data.c_str());
          database.UpdateReceivedNotificationInFile(notif->id,notif->pendingFollowersToReceive,it_p->profileId);
        }
        pthread_mutex_unlock(&(notif_prof->receivenotification_mutex));
        msg.username = (char*)it_p->profileId.c_str();
        msg.message = (char*)notif->message.c_str();
        msg.timestamp = notif->timestamp;
        user->sendingQueue.push_back(msg);
        }
      }
      if(user->isActive() && replica->isPrimary())
        ClearNotifications(user->getUsername(),user->getPort(),replica);
      pthread_mutex_unlock(&(prof->pendingnotification_mutex));
      if(user->isActive() && replica->isPrimary())
        user->flushsendingQueue();
    }
    pthread_exit(NULL);
}

list<PendingDelayed> getPendingForFollowers(string profile, PendingNotification pn){
  list<Profile>::iterator it;
  list<PendingDelayed> still_pending;
  for(it = database.data.begin(); it!= database.data.end(); it++){
      if(it->id == profile){
          list<string>::iterator it_f;
          for(it_f = it->followers.begin(); it_f != it->followers.end(); it_f++){
              PendingDelayed pd;
              pd.follower = *it_f;
              pd.pn = pn;
              still_pending.push_back(pd);
      }
  }
}
  return still_pending;
}


void* NotificationProducer(void* arg){

  Session_Replica* info = (Session_Replica*) arg;
	Session* user = info->session;
  replicaManager* replica = info->replica;
	cout << "User " << user->getUsername() << " logged in\n";
	string name = user->getUsername();
  string follow_data;
  pthread_t new_thread2;
  pthread_create(&new_thread2, NULL, NotificationConsumer, info);
  pthread_detach(new_thread2);
  list<PendingDelayed> pending_list;
  Profile* userprofile = database.getProfile(name);
  //make socket non-blocking
  fcntl(user->getSocket(), F_SETFL, fcntl(user->getSocket(), F_GETFL, 0) | O_NONBLOCK);

	while(user->isActive() && replica->isPrimary()){

		packet* pkt = user->readMessage();
		if(user->isActive() == true){//if user crashed, connection is no longer active

			ReceivedNotification rn;
			PendingNotification pn;
      if(pkt == NULL){
        usleep(10);
      }
      else{
			switch (pkt->type)
			{
			case LOGOUT:
				user->connectionInterrupted();
				break;
			case FOLLOW:
				database.AddFollowing(name, pkt->_payload);
				database.AddFollower(pkt->_payload, name);
        follow_data.append(string(pkt->_payload));
        follow_data.append(" ");
        follow_data.append(name);
        replica->sendmessagetoAllReplicas(FOLLOW,(char*)follow_data.c_str());
        database.WriteFollower(name,pkt->_payload);
        follow_data.clear();
				cout << "pedido de follow " << pkt->_payload << getDate(pkt->timestamp) << endl;
				break;
			case SEND_UNNAMED:
				rn.id = ++notificationId;
				rn.timestamp = pkt->timestamp;
				rn.message = pkt->_payload;
				rn.size = pkt->length;
				rn.pendingFollowersToReceive = database.GetFollowersNumber(name);
				pn.profileId = name;
				pn.notificationId = notificationId;
        pending_list.splice(pending_list.end(),getPendingForFollowers(name,pn)); //adds new pending notifications that we need to insert
        commitChanges(rn,pending_list,name,replica);
        database.AddReceivedNotifications(user->getUsername(),rn);
        database.AddPendingNotifications(user->getUsername(),pn);

        pending_list.clear();
        cout << "mensagem recebida foi " << pkt->_payload << getDate(pkt->timestamp) << endl;

				break;
			default:
        user->sendMessage(ERROR);
				break;
			}
      if (pkt != NULL)
        delete pkt;
      }
    }
	}

  if(replica->isPrimary())
    replica->sendLogoutToReplicas(user);

	pthread_exit(NULL);
}
