#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
int main()
{
    int pid_t = fork();
    if(pid_t == 0)
    {
      execlp("ls","ls","-a","-l",NULL);
    }
    else
    {
       wait(NULL); 
    }
    return 0;
}
