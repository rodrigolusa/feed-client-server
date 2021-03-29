#include "usersession.hpp"

int Session::attemptLogin(){
  //pthread_mutex_lock(&sessionvector_mutex);
  packet* pkt = readMessage();
  if(pkt->type != LOGIN){
    cout << "Login ERROR" << endl;
    sendMessage(ERROR);
    return -1;
    }
  const char* name = new char[pkt->length];
  name = pkt->_payload;
  int count = 0;
  //Verifica se o perfil ja existe na nase de dados, senão o adiciona.
  if(!database.ExistsProfile(name)){
    Profile prof(name);
    database.AddProfile(prof);
    database.AddSessionCount(name); //Apos adicionar o perfil ja incrementa o numero de sessoes ativas
  }
  else{
    if(database.GetActiveSessionsNumber(name) < 2){ //verifica se existe sessao disponivel, senão desativa o cliente (não sei se esta correto)
      database.AddSessionCount(name);
    }
    else{
      setActive(false);
      sendMessage(ERROR);
      return -1;
      }
    }
      string s(name);
      this->username = s;
    //  clientsessions.push_back(this);
      //pthread_mutex_unlock(&sessionvector_mutex);
      delete pkt;
      sendMessage(SUCCESS);
      return 0;
    }

void Session::connectionInterrupted(){ //closes socket and removes session from vector
  close(this->sckt);
  setActive(false);
  //pthread_mutex_lock(&sessionvector_mutex);
  //auto it = find(clientsessions.begin(),clientsessions.end(),this);
  //if(it != clientsessions.end()) //only erase if not last element, otherwise pop
    //clientsessions.erase(it);
//  else
  //  clientsessions.pop_back();
    //pthread_mutex_unlock(&sessionvector_mutex);
}


string Session::getUsername(){
  return this->username;
}
