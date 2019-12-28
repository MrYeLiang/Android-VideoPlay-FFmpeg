//
// Created by 叶亮 on 2019/3/4.
//

#ifndef VIDEOPLAY_IPLAYERBUILDER_H
#define VIDEOPLAY_IPLAYERBUILDER_H

#include "../IPlayer.h"

class IPlayerBuilder{
public:
    virtual IPlayer *BuilderPlayer(unsigned char index = 0);

protected:
    virtual IDemux *CreateDemux() = 0;
    virtual IDecode *CreateDecode() = 0;
    virtual IResample *CreateResample() = 0;
    virtual IVideoView *CreateVideoView() = 0;
    virtual IAudioPlay *CreateAudioPlay() = 0;
    virtual IPlayer *CreatePlayer(unsigned char index = 0) = 0;
};
#endif //VIDEOPLAY_IPLAYERBUILDER_H
