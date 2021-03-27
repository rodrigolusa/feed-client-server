#include <iostream>
#include "profile.hpp"
#include "myDatabase.hpp"
#include <list>

int main(int argc, char *argv[])
{
    //data geral
    MyDatabase db;

    //perfil @dez
    Profile p10("@dez");       
    p10.AddFollower("@nove");
    p10.AddFollower("@oito");
    ReceivedNotification rn(1,1613618078,"olá",3,2);
    p10.AddReceivedNotification(rn);

    //adiciona estrutura do perfil @dez
    db.AddProfile(p10);

    //perfil @onze
    Profile p11("@onze");       
    PendingNotification pn11("@dez",1);
    p11.AddPendingNotification(pn11);

    //adiciona estrutura do perfil @onze
    db.AddProfile(p11);

    //perfil @doze
    Profile p12("@doze");       
    PendingNotification pn12("@dez",1);
    p12.AddPendingNotification(pn12);

    //adiciona estrutura do perfil @onze
    db.AddProfile(p12);

    db.AddFollower("@doze", "@onze");

    list<Profile>::iterator it;
    for(it = db.data.begin(); it!= db.data.end(); it++){
        cout << "Perfil: " << it->id << endl;
        
        cout << "  Seguidores: " << endl;
        list<string>::iterator it_seguidores;
        for(it_seguidores = it->followers.begin(); it_seguidores != it->followers.end(); it_seguidores++){
            cout << "    " << *it_seguidores << endl;
        }        
        
        cout << "  Notificações recebidas: " << endl;
        list<ReceivedNotification>::iterator it_notifications;
        for(it_notifications = it->receivedNotifications.begin(); it_notifications != it->receivedNotifications.end(); it_notifications++){
            cout << "    " << it_notifications->id << ", " << it_notifications->timestamp << ", " << it_notifications->message << ", " << it_notifications->size << ", " << it_notifications->pendingFollowersToReceive << endl;
        }

        cout << "  Notificações pendentes para: " << endl;
        list<PendingNotification>::iterator it_pending;
        for(it_pending = it->pendingNotifications.begin(); it_pending != it->pendingNotifications.end(); it_pending++){
            cout << "    " << it_pending->profileId << ", " << it_pending->profileId << endl;
        }       

        cout << endl; 
    }    
    
    return 0;
}