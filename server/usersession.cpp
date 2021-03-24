#include "usersession.hpp"
int Session::attemptLogin(){
  pthread_mutex_lock(&sessionvector_mutex);
  packet* pkt = readMessage();
  if(pkt->type != LOGIN){
    cout << "Login ERROR" << endl;
    sendMessage(ERROR);
    return -1;
    }
  const char* name = new char[pkt->length];
  name = pkt->_payload;
  int count = 0;
  for(auto it = begin(clientsessions); it != end(clientsessions); ++it)
      {
        if((*it)->username.compare(name) == 0)
        {
          count++;
          if(count == 2){
            sendMessage(ERROR);
            return -1;
          }
        }
      }
      string s(name);
      this->username = s;
      clientsessions.push_back(this);
      pthread_mutex_unlock(&sessionvector_mutex);
      delete pkt;
      sendMessage(SUCCESS);
      return 0;
    }

void Session::terminateSession(){ //closes socket and removes session from vector
  close(this->sckt);
  this->active = false;
  pthread_mutex_lock(&sessionvector_mutex);
  auto it = find(clientsessions.begin(),clientsessions.end(),this);
  if(it != clientsessions.end()) //only erase if not last element, otherwise pop
    clientsessions.erase(it);
  else
    clientsessions.pop_back();
    pthread_mutex_unlock(&sessionvector_mutex);
}

bool Session::isActive(){
  return this->active;
}

string Session::getUsername(){
  return this->username;
}
