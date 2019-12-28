//
// Created by 叶亮 on 2019/2/12.
//

#ifndef VIDEOPLAY_IVIDEOVIEW_H
#define VIDEOPLAY_IVIDEOVIEW_H

#include "../thread/IObserver.h"

class IVideoView: public IObserver
{
public:
    virtual void SetRender(void *win) = 0;
    virtual void Render(XData data) = 0;
    virtual void Update(XData data);
    virtual void Close() = 0;
};
#endif //VIDEOPLAY_IVIDEOVIEW_H
