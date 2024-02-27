#include "TcpServer.hpp"
#include <iostream>
#include <memory>

using namespace std;

void Usage(std::string proc)
{
    std::cout << "\n\rUsage: " << proc << " port[1024+]\n" << std::endl;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        exit(UsageErr);
    }
    uint16_t port = std::stoi(argv[1]);

    unique_ptr<TcpServer> tcp(new TcpServer(port));
    tcp->Init();
    tcp->launch();
    

    return 0;

}