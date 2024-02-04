#include <iostream>
#include <unistd.h>
#include <cassert>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>


using namespace std;

#define MAX 1024

int main()
{
    // 1.创建管道

    int pipefd[2];
    int n = pipe(pipefd);
    assert(n == 0);
    (void)n;

    // 2.创建进程

    pid_t id = fork();
    assert(id != -1);

    // 3.构建半双工通讯
    if (id == 0)
    {
        close(pipefd[0]);

        int cnt = 10;

        while (cnt--)
        {
            char message[MAX];
            snprintf(message, sizeof(message), "child pid: %d, cht: %d", getpid(), cnt);
            write(pipefd[1], message, strlen(message));
            sleep(1);
        }
        cout << "child over" << endl;
        close(pipefd[1]);
        exit(0);
    }

    close(pipefd[1]);
    char buffer[MAX];
    int count = 10;
    while(true)
    {
        ssize_t n = read(pipefd[0], buffer, sizeof(buffer) - 1);
        if(n > 0)
        {
            buffer[n] = '\0';
            cout << getpid() << ":" << buffer <<endl;
        }
        else if(n == 0)
        {
            cout << "child quit" <<endl;
            break;
        }
       
        
    }
    close(pipefd[0]);

    int status = 0;
    pid_t rid  = waitpid(id, &status, 0);

    if(rid == id)
    {
        cout << "等待成功" << endl;
    }

    return 0;
}