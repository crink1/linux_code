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
#include "ThreadPool.hpp"
#include "Task.hpp"
#include "Daemon.hpp"

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

class TcpServer;
class ThreadData
{
public:
    ThreadData(int fd, const std::string &ip, const uint16_t &port, TcpServer *t)
        :sockfd(fd)
        ,clientip(ip)
        ,clientport(port)
        ,tser(t)
    {
    }

public:
    int sockfd;
    std::string clientip;
    uint16_t clientport;
    TcpServer *tser;
};

class TcpServer
{
public:
    TcpServer(const uint16_t &port, const std::string &ip = defaultip)
        : _port(port), _ip(ip), _listensock(defaultfd)
    {
    }

    void Init()
    {
        _listensock = socket(AF_INET, SOCK_STREAM, 0);
        if (_listensock < 0)
        {
            exit(SocketError);
        }

        int opt = 1;
        setsockopt(_listensock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        inet_aton(_ip.c_str(), &(local.sin_addr));

        if (bind(_listensock, (struct sockaddr *)&local, sizeof(local)) < 0)
        {
            exit(BindErr);
        }
        std::cout << "bind succes" << std::endl;
        if (listen(_listensock, backlog) < 0)
        {
            exit(ListenErr);
        }
        std::cout << "listen succes" << std::endl;
    }

    static void* run(void *args)
    {
        pthread_detach(pthread_self());
        ThreadData *td = static_cast<ThreadData *>(args);
        td->tser->test(td->sockfd, td->clientip, td->clientport);
        delete td;
        return nullptr;

    }

    void launch()
    {

        Daemon();
        std::cout << "tcpserver launch" << std::endl;
        threadpool<Task>::getinstance()->launch();

        while (true)
        {
            struct sockaddr_in client;
            memset(&client, 0, sizeof(client));
            socklen_t len = sizeof(client);
            int sockfd = accept(_listensock, (struct sockaddr *)&client, &len);
            if (sockfd < 0)
            {
                std::cout << "accept error" << std::endl;
                continue;
            }
            uint16_t cport = ntohs(client.sin_port);
            char cip[32];
            inet_ntop(AF_INET, &(client.sin_addr), cip, sizeof(cip));

            printf("get a new link sockfd: %d, client ip: %s, client port%d\n", sockfd, cip, cport);
            // test v1
            //  test(sockfd, cip, cport);
            //  close(sockfd);

            // test v2 多进程
            //  pid_t id = fork();
            //  if(id == 0)
            //  {
            //      close(_listensock);
            //      if(fork() > 0)
            //      {
            //          exit(0);
            //      }
            //      test(sockfd, cip, cport);
            //      close(sockfd);
            //      exit(0);
            //  }

            // close(sockfd);
            // pid_t rid = waitpid(id, nullptr, 0);
            // (void)rid;

            // test  v3 多线程
            // ThreadData *td = new ThreadData(sockfd, cip, cport, this);
            // pthread_t tid;
            // pthread_create(&tid, nullptr, run, td);

            //test v4 进程池
            Task t(sockfd, cip, cport);
            threadpool<Task>::getinstance()->Push(t); 
        }
    }

    void test(int sockfd, const std::string &clientip, const uint16_t &clientport)
    {
        char buffer[4096];
        while (true)
        {
            ssize_t n = read(sockfd, buffer, sizeof(buffer));
            if (n > 0)
            {
                buffer[n] = 0;
                std::cout << "client say# " << buffer << std::endl;
                std::string str = "TcpServer echo: ";
                str += buffer;
                write(sockfd, str.c_str(), str.size());
            }
            else if (n == 0)
            {
                std::cout << "client offline" << std::endl;
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
    {
    }

private:
    int _listensock;
    uint16_t _port;
    std::string _ip;
};