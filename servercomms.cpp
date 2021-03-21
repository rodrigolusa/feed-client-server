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
	char* readMessage(int socket){
		/* read from the socket */
		int n;
		char* buffer = new char[256];
		bzero(buffer,256);
		n = read(socket, buffer, 256);
		if (n < 0){
			printf("ERROR reading from socket");
			return "error";
		}
		return buffer;

	}

int writeMessage(int socket, char* msg){
		/* write in the socket */
		int n;
		n = write(socket,msg,256);
		if (n < 0)
			cout << "ERROR writing to socket";
		return 0;
	}

void 	serverComms::closeSocket(){
			close(sockfd);
	}

int serverComms::attemptLogin(session* user){
	char* username = readMessage(user->socket);
	int count = 0;
	for(auto it = begin(clientsessions); it != end(clientsessions); ++it)
			{
				if(it->username.compare(username) == 0)
				{
					count++;
					if(count == 2)
						return -1;
				}
			}
			string s(username);
			user->username = s;
			clientsessions.push_back(*user);
			return 0;
		}

void* ClientManagement(void* arg){
	session user = *(session*) arg;
	string username = user.username;
	int socket = user.socket;
	cout << "User " << username << " logged in\n";
	while(true){
		char*	msg =  readMessage(socket);
		cout << "new message received: " << msg;
		if(msg[0] == 'F'){
			close(socket);
			break;
		}
		else
			writeMessage(socket,"I received your message\n");
	}
}
int main(int argc, char *argv[])
{

	serverComms commManager;
	int  newsockfd, n;
	commManager.init();
	int count = 0;

	while(true){
		newsockfd = commManager.acceptConnections();
		session *clientsession = new session();
		clientsession->socket = newsockfd;

		if(commManager.attemptLogin(clientsession) != 0){
			cout << "User already logged in twice.\n";
			writeMessage(clientsession->socket, "-1");
		}
		else{
			writeMessage(clientsession->socket, "0");
			pthread_t new_client;
			pthread_create(&new_client, NULL, ClientManagement, clientsession);
			}
		}

		cout << "Terminando...\n";
	commManager.closeSocket();

	return 0;
}
