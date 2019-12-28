//
// Created by 叶亮 on 2019/3/4.
//

#ifndef VIDEOPLAY_FFPLAYERBUILDER_H
#define VIDEOPLAY_FFPLAYERBUILDER_H

#include "IPlayerBuilder.h"

class FFPlayerBuilder:public IPlayerBuilder
{
public:
    static void InitHard(void *vm);
    static FFPlayerBuilder *Get()
    {
         static FFPlayerBuilder ff;
        return &ff;
    }

protected:
    FFPlayerBuilder(){};
    virtual IDemux *CreateDemux();
    virtual IDecode *CreateDecode();
    virtual IResample *CreateResample();
    virtual IVideoView *CreateVideoView();
    virtual IAudioPlay *CreateAudioPlay();
    virtual IPlayer *CreatePlayer(unsigned char index = 0);
    
};
#endif //VIDEOPLAY_FFPLAYERBUILDER_H
