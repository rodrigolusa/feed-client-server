#include "primaryManager.hpp"

using namespace std;



int primaryManager::init(replicaManager* replica){
this->sckt = initListeningSocket(DEFAULT_PORT);
this->port = DEFAULT_PORT;
pthread_t new_thread;
Managers* managers = new Managers;
managers->primary = this;
managers->replica = replica;
pthread_create(&new_thread, NULL, acceptClients, managers);
return 0;
}

void* acceptClients(void* args){
		int newsockfd = 0;
		Managers* managers = (Managers*) args;

		primaryManager* primary = managers->primary;
		replicaManager* replica = managers->replica;


		while(replica->isPrimary()){
			struct sockaddr_in cli_addr;
			socklen_t clilen;
			clilen = sizeof(struct sockaddr_in);
			if ((newsockfd = accept(primary->sckt, (struct sockaddr *) &cli_addr, &clilen)) == -1){
				continue;
			}
			char *hostname = inet_ntoa(cli_addr.sin_addr);
			cout << "Connection accepted" << endl;
			Session* new_session = new Session(newsockfd,string(hostname));
			if(new_session->attemptLogin() != 0){
				cout << "User already logged in twice.\n";
				delete new_session;
			}
			else{
				pthread_t new_thread;
				Session_Replica* info = new Session_Replica();
				string backup(new_session->getUsername());
				backup.append(" ");
				backup.append(new_session->getHostname());
				backup.append(" ");
				backup.append(to_string(new_session->getPort()));

	      replica->sendmessagetoAllReplicas(LOGIN,(char*)backup.c_str());
				database.UpdateProfileInFile(new_session->getUsername(),to_string(-1),-1,new_session->getHostname(),new_session->getPort());
				info->replica = replica;
				info->session = new_session;
				pthread_create(&new_thread, NULL, NotificationProducer, info);
				pthread_detach(new_thread);
				}

			}
	}


void primaryManager::closeSocket(){
			close(this->sckt);
}

void primaryManager::sendKeepAlive(replicaManager* replica){
	replica->sendmessagetoAllReplicas(KEEP_ALIVE);
}
