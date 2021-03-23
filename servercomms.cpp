#include "servercomms.hpp"

using namespace std;

#define PORT 4000

	int 	serverComms::init(){
		char buffer[256];
		struct sockaddr_in serv_addr, cli_addr;

		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
					cout << "ERROR opening socket";
					return -1;
				}
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(PORT);
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		bzero(&(serv_addr.sin_zero), 8);

		if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
			printf("ERROR on binding");
			return -1;
		}

		listen(sockfd, 5);

		return 0;
	}
int 	serverComms::acceptConnections(){
		int newsockfd = 0;
		struct sockaddr_in cli_addr;
		socklen_t clilen;
		clilen = sizeof(struct sockaddr_in);
		if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) == -1)
			cout << "ERROR on accept";
		cout << "Connection accepted" << endl;
		return newsockfd;
	}

void 	serverComms::closeSocket(){
			close(sockfd);
	}



void* ClientManagement(void* arg){
	Session *user = *(Session**) &arg;
	cout << "User " << user->getUsername() << " logged in\n";
	while(user->isActive()){
		char* msg = user->readMessage();
		cout << "new message received: " << msg;
		if(msg[0] == 'F'){
			user->terminateSession();
		}
		else
			user->sendMessage("I got your message");
	}
	delete user;
	pthread_exit(NULL);
}
int main(int argc, char *argv[])
{

	serverComms commManager;
	int  newsockfd, n;
	commManager.init();
	int count = 0;
	pthread_mutex_init(&sessionvector_mutex,NULL);

	while(true){
		newsockfd = commManager.acceptConnections();

		Session* new_session = new Session(newsockfd);


		if(new_session->attemptLogin() != 0){
			cout << "User already logged in twice.\n";
			new_session->sendMessage("-1");
		}
		else{
			new_session->sendMessage("0");
			pthread_t new_thread;
			pthread_create(&new_thread, NULL, ClientManagement, new_session);
			}

		}

		cout << "Terminando...\n";
	commManager.closeSocket();

	return 0;
}
