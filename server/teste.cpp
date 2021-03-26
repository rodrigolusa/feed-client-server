#include <iostream>
#include "dataProfile.hpp"
#include "profile.hpp"
#include "data.hpp"
#include <list>

int main(int argc, char *argv[])
{
    //data geral
    MyData dt;

    //perfil @dez
    Profile p10("@dez", 0);       
    DataProfile dp10(p10);
    dp10.AddFollower("@nove");
    dp10.AddFollower("@oito");
    ReceivedNotification rn(1,1613618078,"olá",3,2);
    dp10.AddReceivedNotification(rn);

    //adiciona estrutura do perfil @dez
    dt.Add(dp10);

    //perfil @onze
    Profile p11("@onze", 0);       
    DataProfile dp11(p11);
    PendingNotification pn11("@dez",1);
    dp11.AddPendingNotification(pn11);

    //adiciona estrutura do perfil @onze
    dt.Add(dp11);

    //perfil @doze
    Profile p12("@doze", 0);       
    DataProfile dp12(p12);
    PendingNotification pn12("@dez",1);
    dp12.AddPendingNotification(pn12);

    //adiciona estrutura do perfil @onze
    dt.Add(dp12);

    list<DataProfile>::iterator it;
    for(it = dt.data.begin(); it!= dt.data.end(); it++){
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