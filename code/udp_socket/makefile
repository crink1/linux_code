.PHONY:ALL
ALL:udpserver client

udpserver:main.cc
	g++ -o $@ $^ -lpthread -std=c++11

client:client.cc
	g++ -o $@ $^ -lpthread -std=c++11

.PHONY:clean
clean:
	rm -f udpserver client