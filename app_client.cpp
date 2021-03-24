#include <iostream>
#include "clientcomms.hpp"
#include <pthread.h>

using namespace std;

//client communication object
ClientComms client;

void* ReceiveMessages(void* arg)
{

}

void* SendMessages(void* arg)
{

}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        cout << "missing arguments." << std::endl;
        return 0;
    }

    if (argc > 4)
    {
        cout << "too many arguments." << std::endl;
        return 0;
    }

    //try to connect
    int isConnected = client.connectToServer(argv[2]);
    if (isConnected != 0)
    {
        cout << "Error connecting to server\n";
        return 0;
    }
    
    //try to login
    int isLogged = client.login(argv[1]);
    if (isLogged != 0)
    {
        cout << "Login error, disconnect one device to continue";
        client.closeConnection();
        return 0;
    }

    //start thread to send messages.
    pthread_t send_messages_thread;
	pthread_create(&send_messages_thread, NULL, SendMessages, NULL);

    //start thread to receive messages.
    pthread_t receive_messages_thread;
	pthread_create(&receive_messages_thread, NULL, ReceiveMessages, NULL);

	return 0;
}

