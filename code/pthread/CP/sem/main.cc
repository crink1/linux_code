#include<iostream>
#include<pthread.h>
#include<ctime>
#include<unistd.h>

using namespace std;

#include"Task.hpp"
#include"RingQueue.hpp"



void* Productor(void* args)
{
    RingQueue<Task> *rq = static_cast<RingQueue<Task> *>(args);

    while(1)
    {
       
        int data1 = rand() % 10 + 1;
        int data2 = rand() % 10;
        Task t(data1, data2, operators[rand() % operators.size()]);
        
        rq->push(t);
        cout << "生产了一个任务: " << t.GetTask() << " thread id: " << pthread_self() << endl;
        sleep(1);
        

    }

}

void* Consumer(void* args)
{
    RingQueue<Task> *rq = static_cast<RingQueue<Task> *>(args);

    while(true)
    {
        Task t;
        rq->pop(&t);
        t();
        cout << "处理任务: " << t.GetTask() << " 运算结果是： " << t.GetResult() << " thread id: " << pthread_self() << endl;
        sleep(1);
    }

}

int main()
{
    srand(time(nullptr));

    RingQueue<Task> *rq = new RingQueue<Task>;

    pthread_t c[3], p[5];
    for (int i = 0; i < 5; i++)
    {
        pthread_create(p + i, nullptr, Productor, rq);
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_create(c + i, nullptr, Consumer, rq);
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

    // pthread_create(&c, nullptr, Consumer, rq);
    // pthread_create(&p, nullptr, Productor, rq);

    // pthread_join(c, nullptr);
    // pthread_join(p, nullptr);

    delete rq;


    return 0;
}