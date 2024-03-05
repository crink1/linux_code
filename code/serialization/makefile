.PHONY:all
all:server client

Flag=#-DMS=1
Lib=-ljsoncpp


server:ServerCal.cc
	g++ -o $@ $^ -std=c++11 $(Lib) $(Flag)

client:ClientCal.cc
	g++ -o $@ $^ -std=c++11 -g $(Lib) $(Flag)


.PHONY:clean
clean:
	rm -f server client
