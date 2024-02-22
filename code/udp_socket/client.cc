#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <strings.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

void Usage(std::string proc)
{
    std::cout << "\n\rUsage: " << proc << " serverip serverport\n"
              << std::endl;
}

struct ThreadData
{
    struct sockaddr_in server;
    int sockfd;
    std::string serverip;
};

void *recvmsg(void *args)
{
    ThreadData *td = static_cast<ThreadData *>(args);
    char buffer[1024];
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        struct sockaddr_in tmp;
        socklen_t tlen = sizeof(tmp);

        ssize_t s = recvfrom(td->sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&tmp, &tlen);
        if (s > 0)
        {
            buffer[s] = 0;
            cout << buffer << endl;
        }
    }
}
void *sendmsg(void *args)
{
    ThreadData *td = static_cast<ThreadData *>(args);
    string msg;
    socklen_t len = sizeof(td->server);
    std::string welcome = td->serverip;
    welcome += " comming...";
    sendto(td->sockfd, welcome.c_str(), welcome.size(), 0, (const struct sockaddr *)&td->server, len);
    while (true)
    {

        cout << "enter: ";
        getline(cin, msg);
        sendto(td->sockfd, msg.c_str(), msg.size(), 0, (const struct sockaddr *)&td->server, len);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(0);
    }
    string server_ip = argv[1];
    uint16_t server_port = stoi(argv[2]);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        cout << "socker error" << endl;
        return 1;
    }

    struct ThreadData td;
    td.sockfd = sockfd;
    td.serverip = server_ip;

    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    td.server.sin_family = AF_INET;
    td.server.sin_port = htons(server_port);
    td.server.sin_addr.s_addr = inet_addr(server_ip.c_str());

    pthread_t recvr, sender;
    pthread_create(&recvr, nullptr, recvmsg, &td);
    pthread_create(&sender, nullptr, sendmsg, &td);

    pthread_join(recvr, nullptr);
    pthread_join(sender, nullptr);



    close(sockfd);

    return 0;
}