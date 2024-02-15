#include<iostream>
#include<pthread.h>
#include<ctime>
#include<unistd.h>

using namespace std;

#include"Task.hpp"
#include"BlockQueue.hpp"




void* Productor(void* args)
{
    BlockQueue<Task> *bq = static_cast<BlockQueue<Task> *>(args);

    while(1)
    {
        int data1 = rand() % 10 + 1;
        int data2 = rand() % 10;
        Task t(data1, data2, operators[rand() % operators.size()]);
        
        bq->push(t);
        cout << "生产了一个任务: " << t.GetTask() << " thread id: " << pthread_self() << endl;
        sleep(1);
    }

}

void* Consumer(void* args)
{
    BlockQueue<Task> *bq = static_cast<BlockQueue<Task> *>(args);

    while(true)
    {
        Task t = bq->pop();
        t();
        cout << "处理任务: " << t.GetTask() << " 运算结果是： " << t.GetResult() << " thread id: " << pthread_self() << endl;
        sleep(1);

    }

}

int main()
{
    srand(time(nullptr));

    BlockQueue<Task> *bq = new BlockQueue<Task>;

    pthread_t c[3], p[5];
    for (int i = 0; i < 5; i++)
    {
        pthread_create(p + i, nullptr, Productor, bq);
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_create(c + i, nullptr, Consumer, bq);
    }

    

    for (int i = 0; i < 3; i++)
    {
        pthread_join(c[i], nullptr);
    }
    for (int i = 0; i < 5; i++)
    {
        pthread_join(p[i], nullptr);
    }

    // pthread_t c;
    // pthread_t p;

    // pthread_create(&c, nullptr, Consumer, bq);
    // pthread_create(&p, nullptr, Productor, bq);

    // pthread_join(c, nullptr);
    // pthread_join(p, nullptr);

    delete bq;


    return 0;
}