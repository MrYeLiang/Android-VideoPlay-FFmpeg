//
// Created by 叶亮 on 2019/2/14.
//

#ifndef VIDEOPLAY_IRESAMPLE_H
#define VIDEOPLAY_IRESAMPLE_H

#include "../thread/IObserver.h"
#include "../XParameter.h"

class IResample: public IObserver
{
public:
    virtual bool Open(XParameter in, XParameter out = XParameter()) = 0;
    virtual XData Resample(XData indata) = 0;
    virtual void Update(XData data);
    int outChannels = 2;
    int outFormat = 1;
};
#endif //VIDEOPLAY_IRESAMPLE_H
