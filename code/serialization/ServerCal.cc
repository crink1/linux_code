#include "TcpServer.hpp"
#include "ServerCal.hpp"
#include <unistd.h>

using namespace std;
static void Usage(const std::string &proc)
{
    std::cout << "\nUsage: " << proc << " port\n" << std::endl; 
}

int main(int argc, char *argv[])
{

    if(argc != 2)
    {
        Usage(argv[0]);
    }

    uint16_t port = stoi(argv[1]);
    ServerCal cal;
    TcpServer *tcp = new TcpServer(port, bind(&ServerCal::Cal, &cal, placeholders::_1));
    tcp->Initserver();
    tcp->Launch();

    // Request res(10086 ,432, '+');
    // string s;
    // cout << "序列化" << endl;
    // res.Serialize(&s);
    // cout << s << endl;
    // s = Encode(s);
    // cout << "添加报头" << endl;
    // cout << s;
    // string content;
    // Decode(s, &content);
    // cout << "去除报头" <<endl;
    // cout << content << endl;
    // Request temp;
    // temp.Deserialize(content);
    // std::cout << temp.x << std::endl;
    // std::cout << temp.op << std::endl;
    // std::cout << temp.y << std::endl;

    // Response res(678, 0);
    // string s;

    // cout << "序列化" <<endl;
    // res.Serialize(&s);
    // cout << s << endl;
    // cout << "添加报头" << endl;
    // s = Encode(s);
    // cout << s;
    // cout << "去除报头" << endl;
    // string content;
    // Decode(s, &content);
    // cout << content << endl;
    // cout << "反序列化" << endl;

    // Response temp;
    // temp.Deserialize(content);

    // std::cout << temp.result << std::endl;
    // std::cout << temp.code << std::endl;




    return 0;
}

