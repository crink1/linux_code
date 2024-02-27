.PHONY:All
All:tcpclient tcpserver

tcpclient:TcpClient.cc
	g++ -o $@ $^ -std=c++11

tcpserver:Main.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f tcpclient tcpserver
