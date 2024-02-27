#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>

const int defaultfd = -1;
const std::string defaultip = "0.0.0.0";
const int backlog = 10;

enum
{
    UsageErr = 1,
    SocketError,
    BindErr,
    ListenErr,


};

class TcpServer
{
public:
    TcpServer(const uint16_t &port, const std::string &ip = defaultip)
    :_port(port)
    ,_ip(ip)
    ,_listensock(defaultfd)
    {}

    void Init()
    {
        _listensock = socket(AF_INET, SOCK_STREAM, 0);
        if(_listensock < 0)
        {
            exit(SocketError);
        }

        int opt = 1;
        setsockopt(_listensock, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &opt, sizeof(opt));
        
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        inet_aton(_ip.c_str(), &(local.sin_addr));

        if(bind(_listensock, (struct sockaddr*)&local, sizeof(local)) < 0)
        {
            exit(BindErr);
        }
        std::cout << "bind succes" << std::endl;
        if(listen(_listensock, backlog) < 0)
        {
            exit(ListenErr);
        }
        std::cout << "listen succes" << std::endl;




    }

    void launch()
    {
        
        std::cout << "tcpserver launch" << std::endl;

        while(true)
        {
            struct sockaddr_in client;
            memset(&client, 0, sizeof(client));
            socklen_t len =  sizeof(client);
            int sockfd = accept(_listensock, (struct sockaddr*)&client, &len);
            if(sockfd < 0)
            {
                std::cout << "accept error" << std::endl;
                continue;
            }

            uint16_t cport = ntohs(client.sin_port);
            char cip[32];
            inet_ntop(AF_INET, &(client.sin_addr), cip, sizeof(cip));
            printf("get a new link sockfd: %d, client ip: %s, client port%d\n", sockfd, cip, cport);
            test(sockfd, cip, cport);
            //close(sockfd);
        }
    }

    void test(int sockfd, const std::string &clientip, const uint16_t &clientport)
    {
        char buffer[4096];
        while(true)
        {
            ssize_t n = read(sockfd, buffer, sizeof(buffer));
            if(n > 0)
            {
                buffer[n] = 0;
                std::cout << "client say# " << buffer << std::endl;
                std::string str = "TcpServer echo: ";
                str += buffer;
                write(sockfd, str.c_str(), str.size());
            }
            else if(n == 0)
            {
                std::cout << "server link close" << std:: endl;
                break;
            }
            else
            {
                std::cout << "error sockfd" << std::endl;
                break;
            }
        }
    }

    ~TcpServer()
    {}

private:
    int _listensock;
    uint16_t _port;
    std::string _ip;

};