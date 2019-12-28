//
// Created by 叶亮 on 2019/2/12.
//
#include "GLVideoView.h"
#include "../opengl/XTexture.h"
#include "../XData.h"
#include "../Xlog.h"

void GLVideoView::SetRender(void *win)
{
    view = win;
}

void GLVideoView::Close()
{
    mux.lock();
    if(txt)
    {
        txt->Drop();
        txt = 0;
    }
    mux.unlock();
}

void GLVideoView::Render(XData data)
{
    if(!view){
        return;
    }
    if(!txt){
        txt = XTexture::Create();
        txt->Init(view, (XTextureType)data.format);
    }

    txt->Draw(data.datas, data.width, data.height);
}