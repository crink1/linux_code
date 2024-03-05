#include <iostream>
#include <string>
#include <unistd.h>
#include <assert.h>
#include "Socket.hpp"
#include "ServerCal.hpp"

using namespace std;

static void Usage(const std::string &proc)
{
    std::cout << "\nUsage: " << proc << " serverip serverport\n"
              << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
    }

    string ip = argv[1];
    uint16_t port = stoi(argv[2]);


    Socket sockfd;
    sockfd.Initsocket();

    bool r = sockfd.Connect(ip, port);
    if(!r)
    {
        return 1;
    }

    srand(time(nullptr) ^ getpid());

    int cnt = 0;
    const string opers = "+-*/%&^";

    string buffer_stream;
    while(cnt <= 10)
    {
        cout << "================第" << cnt << "次测试....., " << "================" << endl;
        int x = rand() % 100 + 1;
        usleep(1000);
        int y = rand() % 100;
        usleep(1000);

        char oper = opers[rand() % opers.size()];
        Request req(x,y,oper);
        req.DebugPrint();

        string package;
        req.Serialize(&package);
        package = Encode(package);
        //cout << package << endl;
        write(sockfd.Fd(), package.c_str(), package.size());
        
        
        char buffer[128];
        ssize_t n = read(sockfd.Fd(), buffer, sizeof(buffer));
        if(n > 0)
        {
            buffer[n] = 0;
            buffer_stream += buffer;

            cout << buffer_stream << endl;

            string content;
            bool r = Decode(buffer_stream, &content);
            assert(r);

            Response res;
            r = res.Deserialize(content);
            assert(r);

            res.DebugPrint();


        }
        cout << "=================================================" << endl;
        sleep(1);

        cnt++;

    }
    sockfd.Close();
    
    return 0;
}
