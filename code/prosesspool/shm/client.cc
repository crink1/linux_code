#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include "comm.hpp"

using namespace std;

int main()
{
    key_t key = GetKey();
    int shmid = getshm(key);
    char *s = (char*)shmat(shmid, nullptr, 0);
    int fd = open(filename.c_str(), O_WRONLY);

    for(char i = 'a'; i <= 'z'; i++)
    {
        s[i - 'a'] = i;
        sleep(1);

        //用命名管道保持同步
        int code = 1;
        write(fd, &code, sizeof(code));
    }

    shmdt(s);
    cout << "detach shm done" << endl;
    close(fd);
    return 0;
}