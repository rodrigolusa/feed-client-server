#include <string>
#include <list>

using namespace std;

class Profile{
    public:
    string id;
    int activeSessions;
    list<string> following;

    Profile();
    Profile(string i, int s);
    void AddFollowing(string p);
    list<string> GetFollowing();
    void RemoveFollowing(string p);
};

