#include "primaryManager.hpp"

using namespace std;


MyDatabase database;

int primaryManager::init(int port){
	struct sockaddr_in serv_addr, cli_addr;

	if ((this->sckt = socket(AF_INET, SOCK_STREAM, 0)) == -1){
				cout << "ERROR opening socket";
				return -1;
			}
			int value=1;

	if (setsockopt(this->sckt, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) == -1) {
	    perror("setsockopt");
	    exit(1);
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);

	if (::bind(this->sckt, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		cout << "ERROR on binding" << endl;
		return -1;
	}
	this->port = port;
	database.initDatabase();

	listen(this->sckt, 5);

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

		cout << "Connection accepted" << endl;
		Session* new_session = new Session(newsockfd);
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
