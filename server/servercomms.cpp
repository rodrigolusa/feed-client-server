#include "servercomms.hpp"

using namespace std;

#define PORT 4000

MyDatabase database;
int notificationId = 0;

int serverComms::init(){
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
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);

	if (bind(this->sckt, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		cout << "ERROR on binding" << endl;
		return -1;
	}

	listen(this->sckt, 5);

return 0;
}

int serverComms::acceptConnections(){
		int newsockfd = 0;
		struct sockaddr_in cli_addr;
		socklen_t clilen;
		clilen = sizeof(struct sockaddr_in);
		if ((newsockfd = accept(this->sckt, (struct sockaddr *) &cli_addr, &clilen)) == -1)
			cout << "ERROR on accept";
		cout << "Connection accepted" << endl;
		return newsockfd;
}

void serverComms::closeSocket(){
			close(this->sckt);
}



void* ClientManagement(void* arg){
	Session *user = *(Session**) &arg;
	cout << "User " << user->getUsername() << " logged in\n";
	string name = user->getUsername();
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
				break;
			case SEND:
				rn.id = notificationId++;
				rn.timestamp = pkt->timestamp;
				rn.message = pkt->_payload;
				rn.size = pkt->length;
				rn.pendingFollowersToReceive = database.GetFollowersNumber(name);
				database.AddReceivedNotifications(name, rn);
				pn.profileId = name;
				pn.notificationId = notificationId;
				database.AddPendingNotifications(name, pn);
				cout << "mensagem recebida foi " << pkt->_payload << getDate(pkt->timestamp) << endl;
				break;
			default:
				user->sendMessage(SEND,(char*)pkt->_payload);
				break;
			}

		}
	}
	delete user;
	pthread_exit(NULL);
}
int main(int argc, char *argv[])
{

	serverComms commManager;
	int  newsockfd, n;
	if(commManager.init() != 0)
		exit(1);
	int count = 0;
	

	while(true){
		newsockfd = commManager.acceptConnections();
		Session* new_session = new Session(newsockfd);
		if(new_session->attemptLogin() != 0){
			cout << "User already logged in twice.\n";
		}
		else{
			pthread_t new_thread;
			pthread_create(&new_thread, NULL, ClientManagement, new_session);
			}

		}

		cout << "Terminando...\n";
	commManager.closeSocket();

	return 0;
}
