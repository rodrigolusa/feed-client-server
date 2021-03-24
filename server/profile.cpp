#include "profile.hpp"

using namespace std;

class profile{
    string id;
    int activeSessions;

    public:
    profile(string i, int s){
        id = i;
        activeSessions = s;
    }
};
