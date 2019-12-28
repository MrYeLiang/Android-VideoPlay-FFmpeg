//
// Created by 叶亮 on 2019/1/28.
//

#ifndef VIDEOPLAY_XTHREAD_H
#define VIDEOPLAY_XTHREAD_H

//sleep毫秒
void XSleep(int mix);

//C++ 11 线程库
class XThread
{
public:
        //启动线程
    virtual bool Start();

    //通过控制isExit安全停止线程(不一定成功)
    virtual void Stop();

    virtual void SetPause(bool isP);

    virtual bool IsPause()
    {
        return isPause;
    }

    //入口主函数
    virtual void Main(){}

protected:
        bool isExit = false;
        bool isRuning = false;
        bool isPause = false;
        bool isPausing = false;
private:
        void ThreadMain();
};
#endif //VIDEOPLAY_XTHREAD_H
