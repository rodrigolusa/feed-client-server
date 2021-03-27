run-server: servidor
	cd server ; ./servidor

servidor:
	cd server ; g++ -std=c++11 servercomms.cpp -o servidor -lpthread

cliente:
	cd bin ; make

run-client: cliente
	cd bin ; ./cliente fulaninho localhost 4000

clean:
	cd bin ; make clean ; rm ./cliente ; cd ../server ; rm ./servidor
