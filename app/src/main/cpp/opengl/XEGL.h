//
// Created by 叶亮 on 2019/2/12.
//

#ifndef VIDEOPLAY_EGL_H
#define VIDEOPLAY_EGL_H

class XEGL
{
public:
    virtual bool Init(void *win) = 0;
    virtual void Draw() = 0;
    virtual void Close() = 0;
    static XEGL *Get();

protected:
    XEGL(){}
};
#endif //VIDEOPLAY_EGL_H
