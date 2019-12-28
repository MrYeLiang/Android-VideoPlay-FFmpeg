//
// Created by 叶亮 on 2019/1/28.
//

#include <vector>
#include "XThread.h"
#include "../XData.h"
#include <mutex>

#ifndef VIDEOPLAY_IOBSERVER_H
#define VIDEOPLAY_IOBSERVER_H

class IObserver:public XThread
{
public:
    //观察者接收数据函数
    virtual void Update(XData data){}

    //主体函数 添加观察者(线程安全)
    void AddObs(IObserver *obs);

    //通知所有观察者(线程安全)
    void Notify(XData data);

protected:
    std::vector<IObserver *> obss;
    std::mutex mux;
};

#endif //VIDEOPLAY_IOBSERVER_H