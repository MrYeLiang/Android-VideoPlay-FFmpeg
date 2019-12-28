//
// Created by 叶亮 on 2019/2/12.
//

#ifndef VIDEOPLAY_GLVIDEOVIEW_H
#define VIDEOPLAY_GLVIDEOVIEW_H

#include "../XData.h"
#include "IVideoView.h"

class XTexture;

class GLVideoView:public IVideoView{
public:
    virtual void SetRender(void *win);
    virtual void Render(XData data);
    virtual void Close();
protected:
    void *view = 0;
    XTexture *txt = 0;
    std::mutex mux;
};
#endif //VIDEOPLAY_GLVIDEOVIEW_H
