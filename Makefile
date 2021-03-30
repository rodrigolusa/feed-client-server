run-server: servidor
	cd server ; ./servidor

servidor:
	cd server ; g++ -std=c++11 servercomms.cpp myDatabase.cpp profile.cpp receivedNotification.cpp pendingNotification.cpp usersession.cpp ../common/basiccomm.cpp -o servidor -lpthread

cliente:
	cd bin ; make

run-client: cliente
	cd bin ; ./cliente fulaninho localhost 4000

clean:
	cd bin ; make clean ; rm ./cliente ; cd ../server ; rm ./servidor servercomms.o myDatabase.o profile.o receivedNotification.o pendingNotification.o usersession.o basiccomm.o
