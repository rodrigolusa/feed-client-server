#include "usersession.hpp"

int Session::attemptLogin(){
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
    cout << "passei aqui" << endl;
    Profile prof(name);
    database.AddProfile(prof);
    database.AddSessionCount(name); //Apos adicionar o perfil ja incrementa o numero de sessoes ativas
  }
  else{
    this->profile = database.getProfile(name);
    pthread_mutex_lock(&(this->profile->logincontrol_mutex));//locks critical session so 2 clients can't login with same user simultaneously
    if(database.GetActiveSessionsNumber(name) < 2){ //verifica se existe sessao disponivel, senão desativa o cliente (não sei se esta correto)
      database.AddSessionCount(name);
    cout << database.GetActiveSessionsNumber(name)  << endl;
    pthread_mutex_unlock(&(this->profile->logincontrol_mutex));
    }
    else{
      sendMessage(ERROR);
      cout << "passei aqui 3" << endl;
      pthread_mutex_unlock(&(this->profile->logincontrol_mutex));
      setActive(false);
      return -1;
      }
    }

      string s(name);
      this->username = s;
      delete pkt;
      sendMessage(SUCCESS);
      return 0;
    }

void Session::connectionInterrupted(){ //closes socket and removes session from vector
  close(this->sckt);
  setActive(false);
  pthread_mutex_lock(&(this->profile->logincontrol_mutex));//locks critical session so 2 clients can't login with same user simultaneously
  database.SubtractSessionCount(this->username);
  pthread_mutex_unlock(&(this->profile->logincontrol_mutex));
}


string Session::getUsername(){
  return this->username;
}
