//
// Created by 叶亮 on 2019/3/7.
//

#ifndef VIDEOPLAY_IPLAYERPROXY_H
#define VIDEOPLAY_IPLAYERPROXY_H

#include "IPlayer.h"

class IPlayerProxy: public IPlayer{
public:
    static IPlayerProxy*Get()
    {
        static IPlayerProxy px;
        return &px;
    }

    void Init(void *vm = 0);

    virtual bool Open(const char *path);
    virtual bool Start();
    virtual void InitView(void *win);
    virtual void Close();
    virtual bool Seek(double pos);

    virtual void SetPause(bool isP);
    virtual bool IsPause();

    //获取当前播放的进度
    virtual double PlayPos();

protected:
    IPlayerProxy(){}
    IPlayer *player = 0;
    std::mutex mux;

};
#endif //VIDEOPLAY_IPLAYERPROXY_H
