#include<iostream>
#include<unistd.h>
#include<cstring>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main()
{
    int pipefd[2];
    int n = pipe(pipefd);
    if(n < 0)
    {
        cout << "errno: " << errno <<", str: " <<  strerror(errno) << endl;
        exit(1);
    }

    pid_t pid = fork();
    if(pid == 0)
    {
        close(pipefd[1]);

        char buffer[128];

        ssize_t s = read(pipefd[0], buffer, sizeof(buffer));
        if(s > 0)
        {
            cout << "my pid: "<< getpid() <<  " father say: " << buffer << endl;
        }
        else if(s == 0)
        {
           
        }
        close(pipefd[0]);
        exit(1);
    }
    
    cout << "father pid: " << getpid() << endl;
    close(pipefd[0]);

    char buffer[128] = "i am father";

    ssize_t s = write(pipefd[1], buffer, sizeof(buffer));

    close(pipefd[1]);

    pid_t rid = waitpid(pid, nullptr, 0);
    if(rid < 0)
    {
        cout << "wait fault" << endl;
    }






    return 0;
}