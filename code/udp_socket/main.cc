#include <iostream>
#include <memory>
#include "UdpServer.hpp"
using namespace std;

void Usage(std::string proc)
{
    std::cout << "\n\rUsage: " << proc << " port[1024+]\n" << std::endl;
}

std::string Handler(const std::string &str)
{
    std::string res = "Server get a message: ";
    res += str;
    std::cout << res << std::endl;
    return res;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
    }

    uint16_t port = stoi(argv[1]);

    unique_ptr<udpserver> svr(new udpserver(port));
    svr->Init();
    svr->Run();
    return 0;
}