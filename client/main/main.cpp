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
    //w->news();
}

void* SendMessages(void* arg)
{
    MainWindow* w = static_cast<MainWindow *>(arg);
    w->show();
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

    cout << "Conectou";

    QApplication app(argc, argv);
    MainWindow w;
    w.setUserName(argv[1]);

    w.show();

    //start thread to receive messages.
    pthread_t receive_messages_thread;
    pthread_create(&receive_messages_thread, NULL, ReceiveMessages, &w);

    return app.exec();
}
