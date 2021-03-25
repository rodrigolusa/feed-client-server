#include "profile.hpp"

using namespace std;

Profile::Profile(string i, int s){
    id = i;
    activeSessions = s;
}

void Profile::AddFollowing(string p){
    list<string>::iterator it;
    it = following.end();
    following.insert(it, p);
}

list<string> Profile::GetFollowing(){
    return following;
}

void Profile::RemoveFollowing(string p){  
    list<string>::iterator it;
    it = following.end();
    for(it = following.begin(); it!= following.end(); it++){
        
        if(*it == p){
            following.erase(it);
        }
    } 
}
