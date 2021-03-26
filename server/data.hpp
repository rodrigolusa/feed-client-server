#pragma once
#include <string>
#include <list>
#include "dataProfile.hpp"

using namespace std;

class MyData{
public:
    list<DataProfile> data;

    MyData();
    void Add(DataProfile d);
};