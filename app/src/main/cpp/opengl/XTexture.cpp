//
// Created by 叶亮 on 2019/2/13.
//
#include "XTexture.h"
#include "XShader.h"
#include "../Xlog.h"
#include "XEGL.h"

class CXTexture:public XTexture
{
public:
    XShader sh;
    XTextureType type;
    std::mutex mux;

     virtual void Drop()
    {
        mux.lock();
        XEGL::Get()->Close();
        sh.Close();
        mux.unlock();
        delete this;
    }

    virtual bool Init(void *win, XTextureType type)
    {
        mux.lock();
        this->type = type;
        if(!win){
            XLOGE("XTexture Init failed win is NULL");
            mux.unlock();
            return false;
        }

        if(!XEGL::Get()->Init(win)){
            mux.unlock();
            return false;
        }
        sh.Init((XShaderType)type);
        mux.unlock();
        return true;
    }

    virtual void Draw(unsigned char *data[], int width, int height)
    {
        mux.lock();
        sh.GetTexture(0, width, height,data[0]);        //Y

        if(type == XTEXTURE_YUV420P)
        {
            sh.GetTexture(1, width/2, height/2, data[1]);   //U
            sh.GetTexture(2, width/2, height/2, data[2]);   //V
        }

        else{
            sh.GetTexture(1, width/2, height/2, data[1]);   //UV
        }

        sh.Draw();
        XEGL::Get()->Draw();
        mux.unlock();
    }


};

XTexture *XTexture::Create()
{
    return new CXTexture();
}
