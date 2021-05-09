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


  packet* pkt = readMessage();
  if(pkt->type != BACKUP_PORT){
    cout << "Login ERROR, PACKET SHOULD BE PORT" << endl;
    sendMessage(ERROR);
    return -1;
    }

  //Verifica se o perfil ja existe na nase de dados, senão o adiciona.
  if(!database.ExistsProfile(name)){
    Profile prof(name);
    database.AddProfile(prof);
    database.AddSessionCount(name); //Apos adicionar o perfil ja incrementa o numero de sessoes ativas
  }
  else{
    Profile* prof = database.getProfile(name);
    pthread_mutex_lock(&(prof->logincontrol_mutex));//locks critical session so 2 clients can't login with same user simultaneously
    if(database.GetActiveSessionsNumber(name) < 2){ //verifica se existe sessao disponivel, senão desativa o cliente (não sei se esta correto)
      database.AddSessionCount(name);
    pthread_mutex_unlock(&(prof->logincontrol_mutex));
    }
    else{
      sendMessage(ERROR);
      pthread_mutex_unlock(&(prof->logincontrol_mutex));
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

void Session::flushsendingQueue(){
  list<QueuedMessage>::iterator it;
  for(it = sendingQueue.begin(); it!= sendingQueue.end(); it++){
    cout << "cheguei no flush de sending";
    sendMessage(SEND_NAME,it->username, it->timestamp);
    sendMessage(SEND_DATA,it->message, it->timestamp);
  }
  sendingQueue.clear();
}


void Session::connectionInterrupted(){ //closes socket and removes session from vector
  close(this->sckt);
  setActive(false);
  Profile* prof = database.getProfile(this->username);
  pthread_mutex_lock(&(prof->logincontrol_mutex));//locks critical session so 2 clients can't login with same user simultaneously
  database.SubtractSessionCount(this->username);
  pthread_mutex_unlock(&(prof->logincontrol_mutex));
}


string Session::getUsername(){
  return this->username;
}

int Session:getPort(){
  return this->port;
}

void Session::setPort(int port){
  this->port = port;
}
