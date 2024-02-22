#pragma once

#include<iostream>
#include<string>
#include<pthread.h>
#include<functional>

using callback = std::function<void(void)>;
static int num = 1;

class Thread
{
public:
    Thread(callback cb)
    :_tid(0)
    ,_tname("")
    ,_isrunning(false)
    ,_cb(cb)
    {
        _tname = "thread-" + std::to_string(num++);
        pthread_create(&_tid, nullptr, Routine, this);
        _isrunning = true;
        
    }
    static void* Routine(void* args)
    {
        Thread *t = static_cast<Thread*>(args);
        t->Run();
        return nullptr;
        
    }

    void Run()
    {
        _cb();
    }

    void join()
    {
        pthread_join(_tid, nullptr);
        _isrunning = false;
    }

    std::string Name()
    {
        return _tname;
    }

    bool Isrunning()
    {
        return _isrunning;
    }

    ~Thread()
    {}



private:
    pthread_t _tid;

    std::string _tname;

    bool _isrunning;

    callback _cb;


};