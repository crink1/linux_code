#include"httpserver.hpp"
#include <memory>

using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        exit(1);
    }

    uint16_t port = stoi(argv[1]);

    unique_ptr<HttpServer> ser(new HttpServer(port));
    ser->Launch();
    return 0;
}