#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include "comm.hpp"

using namespace std;


class Shm
{
public:
    Shm()
    {
        bool r = makefifo();
        if(!r)
        {
            return ;
        }
        key_t key = GetKey();
        shmid = createshm(key, IPC_CREAT | IPC_EXCL | 0644);
        s = (char*)shmat(shmid, nullptr, 0);
        fd = open(filename.c_str(), O_RDONLY);

        

    }

    ~Shm()
    {
        shmdt(s);
        cout << "开始将shm从进程的地址空间中移除" << endl;
        shmctl(shmid, IPC_RMID, nullptr);
        cout << "开始将shm从OS中删除" << endl;
        unlink(filename.c_str());
    }
public:
    int shmid;
    int fd;
    char *s;
};

int main()
{
    Shm shm;

    struct shmid_ds ds;
    shmctl(shm.shmid, IPC_STAT, &ds);

    std::cout << ds.shm_perm.__key << std::endl;
    std::cout << ds.shm_segsz << std::endl;
    std::cout << ds.shm_nattch << std::endl;


    while(true)
    {
        int code = 0;
        ssize_t n = read(shm.fd, &code, sizeof(code));
        if(n > 0)
        {
            cout <<"shm中的内容" << shm.s << endl;
            sleep(1);
        }
        else if(n == 0)
        {
            break;
        }
    }
    sleep(10);
    return 0;
}