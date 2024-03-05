#pragma once

#include <functional>
#include <string>
#include <signal.h>
#include "Log.hpp"
#include "Socket.hpp"

using func_t = std::function<std::string(std::string &packge)>;

class TcpServer
{
public:
    TcpServer(uint16_t port, func_t cb)
    :_port(port)
    ,_cb(cb)
    {
    }

    bool Initserver()
    {
        _listensock.Initsocket();
        _listensock.Bind(_port);
        _listensock.Listen();
        lg(Info, "server init");
        return true;
    }

    void Launch()
    {
        signal(SIGCHLD, SIG_IGN);
        signal(SIGPIPE, SIG_IGN);

        while(true)
        {
            uint16_t cport;
            std::string cip;
            int sockfd = _listensock.Accept(&cip, &cport);
            if(sockfd < 0)
            {
                continue;
            }
            lg(Info, "accept a new link, sockfd: %d, clientip: %s, clientport: %d", sockfd, cip.c_str(), cport);

            if(fork() == 0)
            {
                _listensock.Close();
                std::string buffer_stream;

                while(true)
                {
                    char buffer[1024];
                    ssize_t n = read(sockfd, buffer, sizeof(buffer));
                    if(n > 0)
                    {
                        buffer[n] = 0;
                        buffer_stream += buffer;
                        lg(Debug, "debug:\n%s", buffer_stream.c_str());

                        while(true)
                        {
                            std::string info = _cb(buffer_stream);
                            
                            if(info.empty())
                            {
                                break;
                            }
                            
                            lg(Debug, "debug, response:\n%s", info.c_str());
                            lg(Debug, "debug:\n%s", buffer_stream.c_str());

                            write(sockfd, info.c_str(), info.size());
                        }

                    }
                    else if(n == 0)
                    {
                        break;
                    }
                    else
                    {
                        break;
                    }

                }
                exit(0);
            }
            close(sockfd);
        }
    }

    ~TcpServer()
    {
    }

private:
    uint16_t _port;
    Socket _listensock;
    func_t _cb;
};