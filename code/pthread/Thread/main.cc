#include <iostream>
#include <unistd.h>
#include "Thread.hpp"

void Printf()
{
    
    printf("haha, 我是一个封装的线程...\n");
    sleep(2);   
    
}

int main()
{
    Thread t(Printf);
    t.join();
    return 0;
}