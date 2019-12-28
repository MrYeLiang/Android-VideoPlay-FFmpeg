//
// Created by 叶亮 on 2019/1/27.
//

#ifndef VIDEOPLAY_IDEMUX_H
#define VIDEOPLAY_IDEMUX_H

#include "../XData.h"
#include "../thread/IObserver.h"
#include "../XParameter.h"


//解封装接口
class IDemux : public IObserver{
public:
        //打开文件， 或者流媒体 rtmp http rtsp
    virtual bool Open(const char *url) = 0;
    //seek位置pos 0.0~1.0
    virtual bool Seek(double pos) = 0;

    virtual void Close() = 0;

    //读取视频参数
    virtual XParameter GetVPara() = 0;

    //获取音频参数
    virtual XParameter GetAPara() = 0;

    //读取一帧数据，数据由调用者清理
    virtual XData Read() = 0;

    //总时长(毫秒)
    int totalMs = 0;

protected:
    virtual void Main();
};
#endif //VIDEOPLAY_IDEMUX_H
