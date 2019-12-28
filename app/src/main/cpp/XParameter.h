//
// Created by 叶亮 on 2019/2/12.
//

#ifndef VIDEOPLAY_XPARAMETER_H
#define VIDEOPLAY_XPARAMETER_H

struct AVCodecParameters;
struct AVRational;
class XParameter
{
public:
    AVCodecParameters *para = 0;

    int channels = 2;
    int sample_rate = 44100;

    AVRational *time_base;

};
#endif //VIDEOPLAY_XPARAMETER_H
