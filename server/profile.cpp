#include "profile.hpp"

using namespace std;

class profile{
    string id;
    int activeSessions;
    list<string> following;


    public:
    profile(string i, int s){
        id = i;
        activeSessions = s;
    }

    void addFollowing(string p){
        list<string>::iterator it;
        it = following.end();
        following.insert(it, p);
    }

    list<string> getFollowing(){
        return following;
    }

    void removeFollowing(string p){  
        list<string>::iterator it;
        it = following.end();
        for(it = following.begin(); it!= following.end(); it++){
            
            if(*it == p){
                following.erase(it);
            }
        } 
    }
};
