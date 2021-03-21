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
		if (n < 0)
			printf("ERROR reading from socket");
		return buffer;

	}

int writeMessage(int socket){
		/* write in the socket */
		int n;
		char buffer[256];
		 bzero(buffer,256);
		n = write(socket,"I got your message", 18);
		if (n < 0)
			printf("ERROR writing to socket");
	}

void 	serverComms::closeSocket(){
			close(sockfd);
	}

void* ClientManagement(void* arg){
	int socket = *(int*) arg;
	while(true){
		cout << "new message received: " << readMessage(socket);
		writeMessage(socket);

	}
}
int main(int argc, char *argv[])
{

	serverComms commManager;
	int  newsockfd, n;
	commManager.init();
	while(true){
		newsockfd = commManager.acceptConnections();
		pthread_t new_client;
		pthread_create(&new_client, NULL, ClientManagement, &newsockfd);

	}
	commManager.closeSocket();

	return 0;
}
