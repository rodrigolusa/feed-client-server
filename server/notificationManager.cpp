#include "NotificationManager.hpp"

int notificationId = 0;
void ClearNotifications(string profile, int socket){
  list<Profile>::iterator it;
  for(it = database.data.begin(); it != database.data.end(); it ++){
    if(it->id == profile){
      list<PendingNotification>::iterator it_p;
      for(it_p = it->pendingNotifications.begin(); it_p != it->pendingNotifications.end(); it_p++){
        if(it_p->last_read_by != -1 && it_p->last_read_by != socket){
          it->RemovePendingNotification(it_p->profileId, it_p->notificationId);
        }
        else
          it_p->last_read_by = socket;
      }
      break;
    }
  }
}

void* NotificationConsumer(void* arg){

    Session *user = *(Session**) &arg;
    Profile* prof = database.getProfile(user->getUsername());
    while(user->isActive()){
      pthread_mutex_lock(&(prof->pendingnotification_mutex));//blocking behavior for consuming notification while list is empty
      while(prof->pendingNotifications.empty())
        pthread_cond_wait(&(prof->not_empty),&(prof->pendingnotification_mutex));
      list<PendingNotification>::iterator it_p;
      for(it_p = prof->pendingNotifications.begin(); it_p != prof->pendingNotifications.end(); it_p++){
        QueuedMessage msg;
        ReceivedNotification notif = database.GetReceivedNotification(it_p->profileId,it_p->notificationId);
        msg.username = (char*)it_p->profileId.c_str();
        msg.message = (char*)notif.message.c_str();
        msg.timestamp = notif.timestamp;
        user->sendingQueue.push_back(msg);
      }
      ClearNotifications(user->getUsername(),user->getSocket());
      pthread_mutex_unlock(&(prof->pendingnotification_mutex));
      user->flushsendingQueue();
    }
    pthread_exit(NULL);
}


void* NotificationProducer(void* arg){
	Session *user = *(Session**) &arg;
	cout << "User " << user->getUsername() << " logged in\n";
	string name = user->getUsername();
  pthread_t new_thread;
  pthread_create(&new_thread, NULL, NotificationConsumer, user);

	while(user->isActive()){

		packet* pkt  = new packet;
		pkt = user->readMessage();
		if(user->isActive() == true){//if user crashed, connection is no longer active

			ReceivedNotification rn;
			PendingNotification pn;
			switch (pkt->type)
			{
			case LOGOUT:
				user->connectionInterrupted();
				break;
			case FOLLOW:
				database.AddFollowing(name, pkt->_payload);
				database.AddFollower(pkt->_payload, name);
				cout << "pedido de follow " << pkt->_payload << getDate(pkt->timestamp) << endl;
				break;
			case SEND_UNNAMED:
				rn.id = ++notificationId;
				rn.timestamp = pkt->timestamp;
				rn.message = pkt->_payload;
				rn.size = pkt->length;
				rn.pendingFollowersToReceive = database.GetFollowersNumber(name);
				database.AddReceivedNotifications(name, rn);
				pn.profileId = name;
				pn.notificationId = notificationId;
				database.AddPendingNotifications(name, pn);
				cout << "mensagem recebida foi " << pkt->_payload << getDate(pkt->timestamp) << endl;
				//user->sendMessage(SEND_NAME,(char*)name.c_str());
				//user->sendMessage(SEND_DATA,(char*)pkt->_payload);
				break;
			default:
        cout << "entrei no default" << endl;
        user->sendMessage(ERROR);
				break;
			}

		}
	}
	delete user;
	pthread_exit(NULL);
}
