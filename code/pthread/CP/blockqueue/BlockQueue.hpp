#pragma once

#include<queue>

template<class T>
class BlockQueue
{
    static const int defaultnum = 20;
public:
    BlockQueue(int max_capcity = defaultnum)
    :_max_capcity(max_capcity)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_c_cond, nullptr);
        pthread_cond_init(&_p_cond, nullptr);
    }

    T pop()
    {
        pthread_mutex_lock(&_mutex);

        while(_q.size() == 0)
        {
            pthread_cond_wait(&_c_cond, &_mutex);
        }
        T out = _q.front();
        _q.pop();

        pthread_cond_signal(&_p_cond);
        pthread_mutex_unlock(&_mutex);

        return out;

    }

    void push(const T& in)
    {
        pthread_mutex_lock(&_mutex);

        while(_q.size() == _max_capcity)
        {
            pthread_cond_wait(&_p_cond, &_mutex);
        }
        _q.push(in);

        pthread_cond_signal(&_c_cond);
        pthread_mutex_unlock(&_mutex);

    }

    ~BlockQueue()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_mutex_destroy(&_mutex);
        pthread_mutex_destroy(&_mutex);
    }
private:
    queue<T> _q;
    int _max_capcity;
    pthread_mutex_t _mutex;
    pthread_cond_t _c_cond;
    pthread_cond_t _p_cond;

};