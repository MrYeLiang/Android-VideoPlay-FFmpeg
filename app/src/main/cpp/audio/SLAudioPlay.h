//
// Created by 叶亮 on 2019/2/28.
//

#ifndef VIDEOPLAY_SLAUDIOPLAY_H
#define VIDEOPLAY_SLAUDIOPLAY_H

#include "IAudioPlay.h"

class SLAudioPlay: public IAudioPlay
{
public:
    virtual bool StartPlay(XParameter out);
    void PlayCall(void *bufq);
    void Close();

    SLAudioPlay();
    virtual ~SLAudioPlay();

protected:
    unsigned char *buf = 0;
    std::mutex mux;
};
#endif //VIDEOPLAY_SLAUDIOPLAY_H
