#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "Log.hpp"

enum
{
    SocketError = 2,
    BindError,
    ListenError,
};

const int backlog = 10;

class Socket
{
public:
    Socket()
    {
    }
    ~Socket()
    {
    }

public:
    void Initsocket()
    {
        _sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (_sockfd < 0)
        {
            lg(Fatal, "socker error, %s: %d", strerror(errno), errno);
            exit(SocketError);
        }
    }

    void Bind(uint16_t port)
    {
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(port);
        local.sin_addr.s_addr = INADDR_ANY;

        if (bind(_sockfd, (struct sockaddr *)&local, sizeof(local)) < 0)
        {
            lg(Fatal, "bind error, %s: %d", strerror(errno), errno);
            exit(BindError);
        }
    }

    void Listen()
    {
        if (listen(_sockfd, backlog) < 0)
        {
            lg(Fatal, "listen error, %s: %d", strerror(errno), errno);
            exit(ListenError);
        }
    }

    int Accept(std::string *outip, uint16_t *outport)
    {
        struct sockaddr_in c;
        socklen_t len = sizeof(c);

        int newfd = accept(_sockfd, (struct sockaddr *)&c, &len);
        if (newfd < 0)
        {
            lg(Warning, "accept error, %s: %d", strerror(errno), errno);
            return -1;
        }

        char ip[64];
        inet_ntop(AF_INET, &c.sin_addr, ip, sizeof(ip));

        *outip = ip;
        *outport = ntohs(c.sin_port);
        return newfd;
    }

    bool Connect(const std::string &ip, const uint16_t &port)
    {
        struct sockaddr_in c;
        memset(&c, 0, sizeof(c));
        c.sin_family = AF_INET;
        c.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &(c.sin_addr));

        int n = connect(_sockfd, (struct sockaddr *)&c, sizeof(c));
        if (n == -1)
        {
            std::cerr << "connect to " << ip << ":" << port << " error" << std::endl;
            return false;
        }
        return true;
    }

    void Close()
    {
        close(_sockfd);
    }

    int Fd()
    {
        return _sockfd;
    }

private:
    int _sockfd;
};