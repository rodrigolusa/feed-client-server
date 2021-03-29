#include "utils.hpp"
#include <cstdio>
#include <ctime>

using namespace std;
char* getTimestamp(char* timestamp){
  time_t t = time(0);
  struct tm timestruct;
  timestruct = *localtime(&t);
  strftime(timestamp,13,"%H%M%S%d%m%y",&timestruct);
  return timestamp;
}
char* getDate(char* timestamp){
  getTimestamp(timestamp);
  char* date = new char[19];
  sprintf(date,"%c%c:%c%c:%c%c - %c%c/%c%c/%c%c",timestamp[0],timestamp[1],timestamp[2],timestamp[3],timestamp[4],timestamp[5],
timestamp[6],timestamp[7],timestamp[8],timestamp[9],timestamp[10],timestamp[11]);
  return date;
}
