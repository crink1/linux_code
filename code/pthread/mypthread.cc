#include <iostream>
#include <string>
#include <cstdlib>
#include <pthread.h>
#include <thread>
#include <unistd.h>
#include <vector>
using namespace std; 

#define NUM 4

int tickets = 1000;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


class threadData
{
public:
    threadData(int number)
    {
        threadname = "thread-" + to_string(number);
    }

public:
    string threadname;
};


class Mutex
{
public:
    Mutex(pthread_mutex_t *lock):lock_(lock)
    {}
    void Lock()
    {
        pthread_mutex_lock(lock_);
    }
    void Unlock()
    {
        pthread_mutex_unlock(lock_);
    }
    ~Mutex()
    {}
private:
    pthread_mutex_t *lock_;
};

class LockGuard
{
public:
    LockGuard(pthread_mutex_t *lock):mutex_(lock)
    {
        mutex_.Lock();
    }
    ~LockGuard()
    {
        mutex_.Unlock();
    }
private:
    Mutex mutex_;
};


void *GetTicket(void *args)
{
    threadData *td = static_cast<threadData *>(args);
    const char *name = td->threadname.c_str();

    while(1)
    {
        //pthread_mutex_lock(&lock);
        LockGuard lockg(&lock);//RAII
        if(tickets > 0)
        {
            cout << "thread-" << name << ", t: " << tickets << endl;
            tickets--;
            //pthread_mutex_unlock(&lock);
        }
        else
        {
            //pthread_mutex_unlock(&lock);
            break;
        }
        

    }
    printf("%s ... quit\n", name);
    return nullptr;
}

int main()
{
    vector<pthread_t> tids;
    vector<threadData *> thread_datas;
    for(int i = 1; i <= NUM; i++)
    {
        pthread_t tid;
        threadData *td = new threadData(i);
        thread_datas.push_back(td);
        pthread_create(&tid, nullptr, GetTicket, td);
        tids.push_back(tid);

    }

    for (auto thread : tids)
    {
        pthread_join(thread, nullptr);
    }

    for (auto td : thread_datas)
    {
        delete td;
    }
    //pthread_mutex_destroy(&lock);

    return 0;
}







// void* run(void *args)
// {
//     while(true)
//     {
//         cout << "new thread : " << getpid() << endl;
//         sleep(1);
//     }
//     return nullptr;
    
// }

// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid, nullptr, run, nullptr);
//     while(true)
//     {
//         cout << "main thread : " << getpid() << endl;
//         sleep(1);

//     }
//     pthread_join(tid,nullptr);
//     return 0;
// }

//c++线程库
// void run()
// {
//     while(true)
//     {
//         cout << "new thread : " << getpid() << endl;
//         sleep(1);
//     }
    
// }

// int main()
// {
//     thread t(run);
//     t.join();
//     return 0;
// }



// class Request
// {
// public:
//     Request(int s, int e, string name)
//     :_start(s)
//     ,_end(e)
//     ,_name(name)
//     {}
// public:
//     int _start;
//     int _end;
//     string _name;
// };

// class Response
// {
// public:
//     Response(int r, int e)
//     :_ret(r)
//     ,_exitcode(e)
//     {}
    
// public:
//     int _ret;
//     int _exitcode;
// };


// void* sum(void* args)
// {
//     Request* req = static_cast<Request*>(args);
//     Response* res = new Response(0, 0);

//     for(int i = req->_start; i<= req->_end; i++)
//     {
//         res->_ret += i;
//     }

//     delete req;
//     return res;
// }

// int main()
// {
//     Request* req = new Request(1, 100, "thread 1");
//     pthread_t tid;
//     pthread_create(&tid, nullptr, sum, req);


//     void* t;
//     pthread_join(tid, &t);

//     Response* res = static_cast<Response*>(t);
//     cout << res->_ret << "--" << res->_exitcode << endl;

//     delete res;
//     return 0;
// }


