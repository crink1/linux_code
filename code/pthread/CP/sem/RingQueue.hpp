#pragma once

#include<iostream>
#include<pthread.h>
#include<vector>
#include<semaphore.h>

template<class T>
class RingQueue
{
public:
    RingQueue(int capacty = 5)
    :_ringqueue(_capacity)
    ,_capacity(capacty)
    ,_c_step(0)
    ,_p_step(0)
    {
        sem_init(&_c_data_sem, 0, 0);
        sem_init(&_p_space_sem, 0, _capacity);

        pthread_mutex_init(&_c_mutex, nullptr);
        pthread_mutex_init(&_p_mutex, nullptr);

    }

    void push(const T& in)
    {
        sem_wait(&_p_space_sem);

        pthread_mutex_lock(&_p_mutex);
        _ringqueue[_p_step] = in;
        _p_step++;
        _p_step = _p_step % _capacity;
        pthread_mutex_unlock(&_p_mutex);

        sem_post(&_c_data_sem);

    }

    void pop(T *out)
    {
        sem_wait(&_c_data_sem);

        pthread_mutex_lock(&_c_mutex);
        *out = _ringqueue[_c_step]; 
        _c_step++;
        _c_step %= _capacity;

        pthread_mutex_unlock(&_c_mutex);


        sem_post(&_p_space_sem);
    }

    ~RingQueue()
    {
        sem_destroy(&_c_data_sem);
        sem_destroy(&_p_space_sem);

        pthread_mutex_destroy(&_c_mutex);
        pthread_mutex_destroy(&_p_mutex);
    }
private:
    int _capacity;
    vector<T> _ringqueue;
    
    sem_t _c_data_sem;
    sem_t _p_space_sem;

    int _c_step;
    int _p_step;

    pthread_mutex_t _c_mutex;
    pthread_mutex_t _p_mutex;
};