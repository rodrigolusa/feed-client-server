#include "notificationManager.hpp"

using namespace std;
void print(list<ReceivedNotification> const &list)
{
    for (auto const& i: list) {
        cout << i.id << endl;
    }
}


void commitChanges(ReceivedNotification rn, list<PendingDelayed> pending, string username, replicaManager* replica){

    database.WriteReceivedFile(username,rn);
    string header(username);
    header.append(" ");
    header.append(to_string(rn.id));
    replica->sendmessagetoAllReplicas(SEND_HEADER,(char*)header.c_str(),rn.timestamp);
    replica->sendmessagetoAllReplicas(SEND_DATA,(char*)rn.message.c_str());

}



void ClearNotifications(string profile, int port){
  list<Profile>::iterator it;
  for(it = database.data.begin(); it != database.data.end(); it ++){
    if(it->id == profile){
      list<PendingNotification>::iterator it_p;
    //  for(it_p = it->pendingNotifications.begin(); it_p != it->pendingNotifications.end(); it_p++){
        if(database.GetActiveSessionsNumber(profile) == 2){ //if we only have one session active, we can remove the notification
          //if(it_p->last_read_by != -1 && it_p->last_read_by != socket){
            it->pendingNotifications.erase(std::remove_if(it->pendingNotifications.begin(), it->pendingNotifications.end(), [&port](PendingNotification notif){ return (notif.last_read_by != port && notif.last_read_by != -1); }), it->pendingNotifications.end());
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
    while(user->isActive()){
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
        ReceivedNotification notif = database.GetReceivedNotification(it_p->profileId,it_p->notificationId);
        if(it_p->last_read_by != -1)//if it is not -1 nor our socket, then another socket already read it and we can decrease it
          notif.pendingFollowersToReceive--;
        pthread_mutex_unlock(&(notif_prof->receivenotification_mutex));
        msg.username = (char*)it_p->profileId.c_str();
        msg.message = (char*)notif.message.c_str();
        msg.timestamp = notif.timestamp;
        user->sendingQueue.push_back(msg);
        }
      }
      if(user->isActive())
        ClearNotifications(user->getUsername(),user->getPort());
      pthread_mutex_unlock(&(prof->pendingnotification_mutex));
      if(user->isActive())
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
  list<ReceivedNotification> received_list;
  list<ReceivedNotification> finished_inserting;
    for(int i = 0; i < MAX_NUM_REPLICAS-1; i++)
      cout << replica->connection_ports[i] << endl;
  Profile* userprofile = database.getProfile(name);
  //make socket non-blocking
  fcntl(user->getSocket(), F_SETFL, fcntl(user->getSocket(), F_GETFL, 0) | O_NONBLOCK);

	while(user->isActive()){

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
        commitChanges(rn,pending_list,name,replica);
				//pending_list.splice(pending_list.end(),getPendingForFollowers(name,pn)); //adds new pending notifications that we need to insert
        cout << "mensagem recebida foi " << pkt->_payload << getDate(pkt->timestamp) << endl;

				//user->sendMessage(SEND_NAME,(char*)name.c_str());
				//user->sendMessage(SEND_DATA,(char*)pkt->_payload);
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

  string backup(user->getUsername());
  backup.append(" ");
  backup.append(user->getHostname());
  backup.append(" ");
  backup.append(to_string(user->getPort()));
  replica->sendmessagetoAllReplicas(LOGOUT,(char*)backup.c_str());
  database.UpdateProfileInFile(user->getUsername(),user->getHostname(),user->getPort());
	pthread_exit(NULL);
}
