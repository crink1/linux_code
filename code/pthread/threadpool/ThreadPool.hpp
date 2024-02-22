#pragma once

#include<iostream>
#include <vector>
#include <string>
#include <queue>
#include <pthread.h>
#include <unistd.h>
#include "Task.hpp"

struct threadinfo
{
    pthread_t tid;
    std::string name;
};

template<class T>
class threadpool
{
public:
    void Lock()
    {
        pthread_mutex_lock(&_mutex);
    }

    void Unlock()
    {
        pthread_mutex_unlock(&_mutex);
    }

    void Thread_wakeup()
    {
        pthread_cond_signal(&_cond);
    }

    void Thread_sleep()
    {
        pthread_cond_wait(&_cond, &_mutex);
    }

    bool IsQueueEmpty()
    {
        return _tasks.empty();
    }

    std::string GetThreadName(pthread_t tid)
    {
        for (const auto &ti : _threads)
        {
            if (ti.tid == tid)
                return ti.name;
        }
        return "None";
    }
public:
    static void *handle(void *args)
    {
        threadpool<T> *tp = static_cast<threadpool<T> *>(args);
        std::string name = tp->GetThreadName(pthread_self());
        while(true)
        {
            tp->Lock();

            while(tp->IsQueueEmpty())
            {
                tp->Thread_sleep();
            }
            T temp = tp->Pop();

            tp->Unlock();

            temp();

            std::cout << name << " run, " << "result: " << temp.GetResult() << std::endl;
            
        }
    }

    void launch()
    {
        int n = _threads.size();
        for(int i = 0; i < n; i++)
        {
            _threads[i].name = "thread-" + std::to_string(i + 1);
            pthread_create(&(_threads[i].tid), nullptr, handle, this);
        }
    }

    T Pop()
    {
        T t = _tasks.front();
        _tasks.pop();
        return t;
    }

    void Push(const T &t)
    {
        Lock();

        _tasks.push(t);
        Thread_wakeup();

        Unlock();
    }

    threadpool(int n = 5)
    :_threads(n)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);
    }
    ~threadpool()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
    }

private:
    std::vector<threadinfo> _threads;
    std::queue<Task> _tasks;

    pthread_mutex_t _mutex;
    pthread_cond_t _cond;



};