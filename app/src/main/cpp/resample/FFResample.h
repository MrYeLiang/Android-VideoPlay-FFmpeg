//
// Created by 叶亮 on 2019/2/14.
//

#ifndef VIDEOPLAY_FFRESAMPLE_H
#define VIDEOPLAY_FFRESAMPLE_H

#include "IResample.h"

struct SwrContext;
class FFResample:public IResample
{
public:
    virtual bool Open(XParameter in, XParameter out = XParameter());
    virtual void Close();
    virtual XData Resample(XData indata);

protected:
    SwrContext *actx = 0;
    std::mutex mux;
};
#endif //VIDEOPLAY_FFRESAMPLE_H
