#pragma once

#include <iostream>
#include <unistd.h>
#include <functional>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <strings.h>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unordered_map>

using func_t = std::function<std::string(const std::string&)>;

uint16_t defaule_port = 8080;
std::string default_ip = "0.0.0.0";
const int size = 1024;

enum
{
    SOCKET_ERR = 1,
    BIND_ERR
};

class udpserver
{
public:
    udpserver(uint16_t port, std::string ip = default_ip)
        : _ip(ip), _sockfd(0), _port(port), _isrunning(false)
    {
    }

    void Init()
    {
        // 1.create udp socket
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd < 0)
        {
            std::cerr << "socket create error, sockfd: " << _sockfd << std::endl;
            exit(1);
        }
        // 2.bind socket
        struct sockaddr_in local;
        bzero(&local, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);  //host to net short
        local.sin_addr.s_addr = inet_addr(_ip.c_str()); //string to net
        //local.sin_addr.s_addr = htonl(INADDR_ANY);

        if(bind(_sockfd, (const struct sockaddr*)&local, sizeof(local)) < 0)
        {
            std::cerr << "bind error, errno: " << errno <<", err string: " << strerror(errno) << std::endl;
            exit(1);
        }
        std::cout << "bind success" << std::endl;
    }

    void CheckUser(const struct sockaddr_in &client, const std::string clientip, uint16_t clientport)
    {
        auto iter = online_user_.find(clientip);
        if(iter == online_user_.end())
        {
            online_user_.insert({clientip, client});
            std::cout << "[" << clientip << ":" << clientport << "] add to online user." << std::endl;
        }
    }


    void Broadcast(const std::string &info, const std::string clientip, uint16_t clientport)
    {
        for(const auto &user : online_user_)
        {
            std::string message = "[";
            message += clientip;
            message += ":";
            message += std::to_string(clientport);
            message += "]# ";
            message += info;
            socklen_t len = sizeof(user.second);
            sendto(_sockfd, message.c_str(), message.size(), 0, (struct sockaddr*)(&user.second), len);
        }
    }

    void Run()
    {
        _isrunning = true;
        char buffer[size];
        
        while(_isrunning)
        {
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            ssize_t n = recvfrom(_sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&client, &len);
            if(n < 0)
            {
                std::cerr << "recv error, errno: " << errno <<", err string: " << strerror(errno) << std::endl;
                continue;
            }
            buffer[n] = 0;
            uint16_t clientport = ntohs(client.sin_port);
            std::string clientip = inet_ntoa(client.sin_addr);
            CheckUser(client, clientip, clientport);

            std::string info = buffer;
            Broadcast(info,clientip, clientport);
        
        }
    }
    ~udpserver()
    {
        if(_sockfd > 0)
        {
            close(_sockfd);
        }
    }

private:
    int _sockfd;
    std::string _ip;
    uint16_t _port;
    bool _isrunning;
    std::unordered_map<std::string, struct sockaddr_in> online_user_;
};