#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <ctime>
#include <unistd.h>
#include <cassert>
#include <sys/types.h>
#include <wait.h>

using namespace std;
#include "Task.hpp"

static int number = 1;

const int num = 5;

class channel
{
public:
    channel(int fd, pid_t id)
    :ctrlfd(fd)
    ,workerid(id)
    {
        name = "channel-" + to_string(number++);
    }

public:
    int ctrlfd;
    pid_t workerid;
    string name;

};

void run()
{
    while(true)
    {
        int code = 0;

        ssize_t n = read(0, &code, sizeof(code));

        if(n == sizeof(code))
        {
            tasks.RunTask(code);
        } 
        else if(n == 0)
        {
            break;
        }
    }

    cout << getpid() << "quit" <<endl;
}

void Inintchannels(vector<channel>* c)
{
    for(int i = 0; i < num; i++)
    {
        vector<int> closefd;
        int pipefd[2];
        int n = pipe(pipefd);
        assert(n == 0);
        (void)n;

        pid_t id = fork();
        assert(id != -1);

        if(id == 0)
        {
            if(!closefd.empty())
            {
                for(auto& i : closefd)
                {
                    close(i);
                }
            }
            close(pipefd[1]);
            dup2(pipefd[0], 0);
            run();
            exit(0);
        }

        close(pipefd[0]);
        c->push_back(channel(pipefd[1], id));
        closefd.push_back(pipefd[1]);

    }
}

void SendCommand(const vector<channel>& c, bool flag, int num = -1)
{
  int pos = 0;

  while(true)
  {
    int command = tasks.SelectTask();


    const auto &channel = c[pos++];
    pos = pos % c.size();
    
    cout << "任务" << tasks.ToSrt(command) << "由进程" << channel.name << "执行" << endl;

    if(!flag)
    {
        num--;
        if(num < 0)
        {
          break;
        }
    }
    sleep(1);
    
  }
  cout <<"执行完毕"<< endl;
}

void releasechannels(vector<channel>& c)
{
  for(auto& i : c)
  {
    close(i.ctrlfd);
    waitpid(i.workerid, nullptr, 0);
  }
}


int main()
{

    vector<channel> channels;

    Inintchannels(&channels);

    const bool loop = true;
    SendCommand(channels, loop);

    releasechannels(channels);
    return 0;
}
