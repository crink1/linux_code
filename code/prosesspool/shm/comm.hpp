#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const std::string pathname = "/home/crin/linux_code/code/prosesspool/shm";
const int project_id = 0x11223344;
const std::string filename = "fifo";

const int size = 4096;

key_t GetKey()
{
    key_t key = ftok(pathname.c_str(), project_id);
    if(key < 0)
    {
        std::cerr << "key errno:" << errno << ", errstring:" << strerror(errno) << std::endl;
        exit(1);
    }
    return key;
}

int createshm(key_t key, int flag)
{
    int shmid = shmget(key, size, flag);
    if(shmid < 0)
    {
        std::cerr << "shm errno:" << errno << ", errstring:" << strerror(errno) << std::endl;
        exit(2);
    }

    return shmid;
}

int getshm(key_t key)
{
    return createshm(key, IPC_CREAT);
}

bool makefifo()
{
    int n = mkfifo(filename.c_str(), 0666);
    if(n < 0)
    {
        std::cerr << "errno:" << errno << ", errstring:" << strerror(errno) << std::endl;
       return false;
    }
    std::cout << "mkfifo success..." << std::endl;
    return true;

}


