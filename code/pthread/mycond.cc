#include <iostream>
#include <unistd.h>
#include <pthread.h>

int cnt = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* count(void* args)
{
    pthread_detach(pthread_self());
    uint64_t i = (uint64_t)args;
    std::cout << "pthread: " << i << " create success" << std::endl;

    while(1)
    {
        pthread_mutex_lock(&mutex);

        pthread_cond_wait(&cond, &mutex);
        std::cout << "pthread: " << i << " ， cnt: " << cnt++ << std::endl;

        pthread_mutex_unlock(&mutex);
    }

}

int main()
{
    for(uint64_t i; i < 6; i++)
    {
        pthread_t tid;
        pthread_create(&tid, nullptr, count, (void*)i);
        sleep(1);

    }

    std::cout << "主线程开始控制" << std::endl;

    while (true)
    {
        sleep(1);
        pthread_cond_signal(&cond);//唤醒一个等待线程，默认是第一个
        //pthread_cond_broadcast(&cond); // 唤醒全部等待线程
        std::cout << "signal one thread..." << std::endl;
    }
    

    return 0;
}