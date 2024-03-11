#pragma pnce

#include <iostream>
#include <string>
#include <pthread.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <sys/types.h>
#include <sys/socket.h>

#include "Socket.hpp"
#include "Log.hpp"

const std::string webroot = "./webroot/sign";
const std::string sep = "\r\n";
const std::string homepage = "index.html";

static const int dftport = 8080;

class HttpServer;

class ThreadData
{
public:
    ThreadData(int fd, HttpServer *s)
        : sockfd(fd), ser(s)
    {
    }

public:
    int sockfd;
    HttpServer *ser;
};

class HttpRequest
{
public:
    void Deserialize(std::string req)
    {
        while (true)
        {
            size_t pos = req.find(sep);
            if (pos == std::string::npos)
            {
                break;
            }
            std::string temp = req.substr(0, pos);
            if (temp.empty())
            {
                break;
            }
            _req_head.push_back(temp);
            req.erase(0, pos + sep.size());
        }
        text = req;
    }

    void Parse()
    {
        std::stringstream ss(_req_head[0]);
        ss >> method >> url >> httpv;
        file_path = webroot;
        if (url == "/" || url == "/index.html")
        {
            file_path += "/";
            file_path += homepage;
        }
        else
        {
            file_path += url;
        }

        size_t pos = file_path.rfind(".");
        if (pos == std::string::npos)
        {
            suffix = ".html";
        }
        else
        {
            suffix = file_path.substr(pos);
        }

        
    }

    void printinfo()
        {
            for (auto &line : _req_head)
            {
                std::cout << "--------------------------------" << std::endl;
                std::cout << line << "\n\n";
            }

            std::cout << "method: " << method << std::endl;
            std::cout << "url: " << url << std::endl;
            std::cout << "http_version: " << httpv << std::endl;
            std::cout << "file_path: " << file_path << std::endl;
            std::cout << text << std::endl;
        }

public:
    std::vector<std::string> _req_head;
    std::string text;

    std::string url;
    std::string method;
    std::string httpv;
    std::string file_path;
    std::string suffix;
};



class HttpServer
{
public:
    HttpServer(uint16_t port = dftport)
    :_port(port)
    {
        content_type[".html"] = "text/html";
        content_type[".png"] = "timage/png";
    }

    bool Launch()
    {
        _listensock.Initsocket();
        _listensock.Bind(_port);
        _listensock.Listen();

        while(true)
        {
            std::string clientip;
            uint16_t clientport;

            int sockfd = _listensock.Accept(&clientip, &clientport);
            if(sockfd < 0)
            {
                continue;
            }
            lg(Info, "get a new connect, sockfd: %d", sockfd);
            pthread_t tid;
            ThreadData *td = new ThreadData(sockfd, this);

            pthread_create(&tid, nullptr, ThreadRun, td);
        }
    }


    static std::string ReadHtmlContent(const std::string &htmlpath)
    {
        std::ifstream in(htmlpath, std::ios::binary);
        if(!in.is_open())
        {
            return "";
        }
        in.seekg(0,std::ios_base::end);

        auto len = in.tellg();
        in.seekg(0, std::ios_base::beg);

        std::string content;
        content.resize(len);

        in.read((char*)content.c_str(), content.size());

        in.close();
        return content;

    }

    std::string SuffixToDesc(const std::string &suffix)
    {
        auto it = content_type.find(suffix);
        if(it == content_type.end())
        {
            return content_type[".html"];
        }

        else
        {
            return content_type[suffix];
        }
    }

    void HttpHead(int sockfd)
    {
        char buff[1024];
        ssize_t n = recv(sockfd, buff, sizeof(buff) - 1, 0);
        if(n > 0)
        {
            buff[n] = '\0';
            std::cout << buff << std::endl;

            HttpRequest hreq;

            hreq.Deserialize(buff);
            hreq.Parse();


            std::string str;
            bool f = true;
            str = ReadHtmlContent(hreq.file_path);
            
            if(str.empty())
            {
                f = false;
                std::string err = webroot;
                err += "/";
                err += "err.html";
                str = ReadHtmlContent(err);

            }
            
            std::string res_line;
            if(f)
            {
                res_line += "HTTP/1.0 200 OK\r\n";
            }
            else
            {
                res_line += "HTTP/1.0 404 Not Found\r\n";
            }

            std::string res_header;
            res_header += "Content-Length";
            res_header += std::to_string(str.size());
            res_header += "\r\n";
            res_header += "Content-Type";
            res_header += SuffixToDesc(hreq.suffix);
            res_header += "\r\n";
            res_header += "Set-Cookie: name==xiyy&&passwd=123";
            res_header += "\r\n";

            std::string blank_line = "\r\n";

            std::string res = res_line;
            res += res_header;
            res += blank_line;
            res += str;
            send(sockfd, res.c_str(), res.size(), 0);
        }
        close(sockfd);

    }

    static void *ThreadRun(void *args)
    {
        pthread_detach(pthread_self());
        ThreadData *td = static_cast<ThreadData *>(args);
        td->ser->HttpHead(td->sockfd);
        delete td;
        return nullptr;
    }

    ~HttpServer()
    {}

public:

    uint16_t _port;
    Socket _listensock;
    std::unordered_map<std::string, std::string> content_type;
};
