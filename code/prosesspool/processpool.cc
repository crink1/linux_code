#include<iostream>
#include<unistd.h>
#include<cassert>


const int num = 5;




int main()
{
    //1.创建管道
    for(int i = 0; i < num; i++)
    {
        int pipefd[2];
        int n = pipe(pipefd);
        assert(n == 0);
        (void)n;

        //2.创建进程

        pid_t id = fork();
        assert(id != -1);

        //3.构建半双工通讯
        if(id == 0)
        {
            close(pipefd[1]);
        }
    }
    return 0;
}