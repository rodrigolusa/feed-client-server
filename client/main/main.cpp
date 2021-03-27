#include "mainwindow.h"
#include "../clientcomms.hpp"
#include <iostream>
#include <pthread.h>
#include <QApplication>

using namespace std;

//client communication object
ClientComms client;

void* ReceiveMessages(void* arg)
{
    MainWindow* w = static_cast<MainWindow *>(arg);
    w->news();
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
    client.init(argv[1],argv[2],atoi(argv[3]));
    //try to connect
    int isConnected = client.connectToServer();
    if (isConnected != 0)
    {
        cout << "Error connecting to server\n";
        return 0;
    }

    //try to login
    int isLogged = client.login();
    if (isLogged != 0)
    {
        cout << "Login error, disconnect one device to continue";
        client.closeSocket();
        return 0;
    }

    cout << "Conectou";

    QApplication app(argc, argv);
    MainWindow w;
    w.setComm(&client);
    w.setUserName(argv[1]);

    w.show();

    //start thread to receive messages.
    pthread_t receive_messages_thread;
    pthread_create(&receive_messages_thread, NULL, ReceiveMessages, &w);

    return app.exec();
}
