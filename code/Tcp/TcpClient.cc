#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

using namespace std;

void Usage(const std::string &proc)
{
    std::cout << "\n\rUsage: " << proc << " serverip serverport\n"
              << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }
    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(serverport);
    inet_pton(AF_INET, serverip.c_str(), &(server.sin_addr));

    while (true)
    {
        int linkcnt = 5;
        bool islink = false;
        int sockfd = 0;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            std::cerr << "socket error" << std::endl;
            return 1;
        }

        do
        {
            int n = connect(sockfd, (struct sockaddr *)(&server), sizeof(server));
            if (n < 0)
            {
                islink = true;
                linkcnt--;
                std::cerr << "connect error..." << std::endl;
                sleep(1);
            }
            else
            {
                break;
            }

        } while (linkcnt && islink);

        string msg;
        cout << "Enter :";
        getline(cin, msg);

        int n = write(sockfd, msg.c_str(), msg.size());
        if (n < 0)
        {
            std::cerr << "write error..." << std::endl;
            continue;
        }

        char buffer[1024];
        n = read(sockfd, buffer, sizeof(buffer));
        if (n > 0)
        {
            buffer[n] = 0;
            cout << buffer << endl;
        }

        close(sockfd);
    }
   

    return 0;
}