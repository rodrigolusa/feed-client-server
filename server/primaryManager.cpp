#include "primaryManager.hpp"

using namespace std;



int primaryManager::init(replicaManager* replica){

int sckt = 0;
int port = DEFAULT_PORT;

sckt = initListeningSocket(port);
while(sckt == -1){
	port++;
	cout << "Trying to connect on port " << port << endl;
	sckt = initListeningSocket(port);
	}
cout<< "Sucessful connection on port " << port << endl;
this->sckt = sckt;
this->port = port;
replica->primary_socket = this->sckt;
pthread_t new_thread;
Managers* managers = new Managers;
managers->primary = this;
managers->replica = replica;
pthread_create(&new_thread, NULL, acceptClients, managers);
connecttoOlderClients(replica);
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

void primaryManager::connecttoOlderClients(replicaManager* replica){

list<Profile>::iterator it;
Session_Replica* info;
Session* new_session;

for(it = database.data.begin(); it != database.data.end(); it++){

	for(int i = 0; i < 2; i++){
			int newsockfd = createSocket();
		if(it->backup_ports[i] != -1){//if port is different than -1, then we have an active session.
				if(connectToSv(it->backup_hosts[i],it->backup_ports[i],newsockfd)){
					info = new Session_Replica();
				 	new_session = new Session(newsockfd,it->backup_hosts[i]);
					new_session->setPort(it->backup_ports[i]);
					new_session->setUsername(it->id);
					info->replica = replica;
					info->session = new_session;
					pthread_t new_thread;
					pthread_create(&new_thread, NULL, NotificationProducer, info);
					pthread_detach(new_thread);
				}

			}
		}

	}
}
