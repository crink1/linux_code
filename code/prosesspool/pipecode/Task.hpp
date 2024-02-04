#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include <ctime>
#include <unistd.h>

// 包装器把函数封装一下，都存到vector里
using task_t = function<void()>;

void Download()
{
    cout << getpid() << ":下载任务" << endl;
}
void PrintLog()
{
    cout << getpid() << ":打印日志" << endl;
}
void PushVideoStream()
{
    cout << getpid() << ":推送视频流" << endl;
}

class Task
{
public:
    const static int download_code = 0;
    const static int printlog_code = 1;
    const static int pushvideostream_code = 2;
    vector<task_t> tasks;

private:
    bool CheckSafe(const int &code)
    {
        if (code >= 0 && code < tasks.size())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

public:
    Task()
    {
        tasks.push_back(Download);
        tasks.push_back(PrintLog);
        tasks.push_back(PushVideoStream);

        srand(time(nullptr) ^ getpid());
    }

    void RunTask(const int &code)
    {
        if (!CheckSafe(code))
        {
            cout << "任务编号不合法!!!" << endl;
            return;
        }

        return tasks[code]();
    }

    int SelectTask()
    {
        return rand() % tasks.size();
    }


    string ToSrt(const int& code)
    {
        switch (code)
        {
        case download_code:
            return "Download";
        case printlog_code:
            return "PrintLog";
        case pushvideostream_code:
            return "PushVideoStream";
        default:
            return "Unknow";
        }
    }
    
};

Task tasks;
