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
				pthread_create(&new_thread, NULL, NotificationProducer, new_session);
				pthread_detach(new_thread);
				}

			}
	}


void primaryManager::closeSocket(){
			close(this->sckt);
}
