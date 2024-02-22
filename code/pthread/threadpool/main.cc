#include <iostream>
#include <ctime>
#include "ThreadPool.hpp"
#include "Task.hpp"

using namespace std;

int main()
{
    std::cout << "process run" << std::endl;
    srand(time(nullptr) ^ getpid());
    threadpool<Task> tp;
    tp.launch();


    while (true)
    {
        int x = rand() % 10 + 1;
        usleep(10);
        int y = rand() % 5;
        char op = operators[rand() % operators.size()];

        Task t(x, y, op);
        tp.Push(t);
        std::cout << "main thread get task: " << t.GetTask() << std::endl;

        
    }
    
    return 0;
}