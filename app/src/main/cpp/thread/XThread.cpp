//
// Created by 叶亮 on 2019/1/28.
//

#include "XThread.h"
#include "../Xlog.h"
#include <thread>
#include <sys/syscall.h>
#include <unistd.h>

using namespace std;
void XSleep(int mis)
{
    chrono::milliseconds du(mis);
    this_thread::sleep_for(du);
}

//启动线程
bool XThread::Start()
{
    isExit = false;
    isPause = false;
    thread th(&XThread::ThreadMain, this);
    th.detach();
    return true;
}

void XThread::SetPause(bool isP)
{
    isPause = isP;
    //等待100毫秒
    for(int i = 0; i <10; i++){
        if(isPause == isP)
        {
            break;
        }
        XSleep(10);
    }
}

void XThread::ThreadMain()
{
    isRuning = true;
    Main();
    isRuning = false;
}

void XThread::Stop()
{
    isExit = true;
    for(int i = 0; i <200; i++){
        if(!isRuning){
            return;
        }
        XSleep(1);
    }
}
