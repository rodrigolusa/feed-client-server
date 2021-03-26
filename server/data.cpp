#include "data.hpp"


using namespace std;

MyData::MyData(){
};

void MyData::Add(DataProfile d){
    list<DataProfile>::iterator it;
    it = data.end();
    data.insert(it, d);
}