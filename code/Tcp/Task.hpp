#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include "Log.hpp"
#include "TL.hpp"

extern Log lg;
Tl tl;
class Task
{
public:
    Task(int sockfd, const std::string &clientip, const uint16_t &clientport)
        : _sockfd(sockfd), _clientip(clientip), _clientport(clientport)
    {
    }

    Task()
    {
    }

    void run()
    {
        char buffer[4096];

        ssize_t n = read(_sockfd, buffer, sizeof(buffer));
        if (n > 0)
        {
            buffer[n] = 0;
            std::cout << "client key#" << buffer << std::endl;
            std::string ret = tl.translation(buffer);

            n = write(_sockfd, ret.c_str(), sizeof(ret));
            if (n < 0)
            {
                lg(Warning, "write error, errno : %d, errstring: %s", errno, strerror(errno));
            }
        }
        else if (n == 0)
        {
            lg(Info, "%s:%d quit, server close sockfd: %d", _clientip.c_str(), _clientport, _sockfd);
        }
        else
        {
            lg(Warning, "read error, sockfd: %d, client ip: %s, client port: %d", _sockfd, _clientip.c_str(), _clientport);
        }
        close(_sockfd);
    }

    void operator()()
    {
        run();
    }
    
    ~Task()
    {}

private:
    int _sockfd;
    std::string _clientip;
    uint16_t _clientport;
};