#include "usersession.hpp"
int Session::attemptLogin(){
  pthread_mutex_lock(&sessionvector_mutex);
  char* name = readMessage();
  int count = 0;
  for(auto it = begin(clientsessions); it != end(clientsessions); ++it)
      {
        if((*it)->username.compare(name) == 0)
        {
          count++;
          if(count == 2)
            return -1;
        }
      }
      string s(name);
      this->username = s;
      clientsessions.push_back(this);
      pthread_mutex_unlock(&sessionvector_mutex);
      return 0;
    }

char* Session::readMessage(){
    /* read from the socket */
  int n;
  char* buffer = new char[256];
  bzero(buffer,256);
  n = read(this->socket, buffer, 256);
  if (n < 0){
    printf("ERROR reading from socket");
    return "error";
   }
  return buffer;
}
int Session::sendMessage(char* msg){
  /* write in the socket */
  int n;
  n = write(this->socket,msg,256);
  if (n < 0)
    cout << "ERROR writing to socket";
  return 0;
}
void Session::terminateSession(){ //closes socket and removes session from vector
  close(this->socket);
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
