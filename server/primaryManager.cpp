#include "primaryManager.hpp"

using namespace std;



int primaryManager::init(){
this->sckt = initListeningSocket(DEFAULT_PORT);
this->port = DEFAULT_PORT;
return 0;
}

int primaryManager::acceptConnections(){
		int newsockfd = 0;
		struct sockaddr_in cli_addr;
		socklen_t clilen;
		clilen = sizeof(struct sockaddr_in);
		if ((newsockfd = accept(this->sckt, (struct sockaddr *) &cli_addr, &clilen)) == -1){
			cout << "ERROR on accept";
			return -1;
		}

		char *hostname = inet_ntoa(cli_addr.sin_addr);

		cout << "Connection accepted" << endl;
		Session* new_session = new Session(newsockfd,string(hostname));
		if(new_session->attemptLogin() != 0){
			cout << "User already logged in twice.\n";
		}
		else{
			pthread_t new_thread;
			pthread_create(&new_thread, NULL, NotificationProducer, new_session);
			pthread_detach(new_thread);
			}

		}


void primaryManager::closeSocket(){
			close(this->sckt);
}
