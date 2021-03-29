# feed-client-server

g++ -std=c11 servercomms.cpp myDatabase.cpp profile.cpp receivedNotification.cpp pendingNotification.cpp usersession.cpp ../common/basiccomm.cpp -o server -lpthreads