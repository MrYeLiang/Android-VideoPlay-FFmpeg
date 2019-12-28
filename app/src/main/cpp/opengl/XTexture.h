//
// Created by 叶亮 on 2019/2/13.
//

#ifndef VIDEOPLAY_XTEXTURE_H
#define VIDEOPLAY_XTEXTURE_H

enum XTextureType
{
    XTEXTURE_YUV420P = 0,   //软解码和虚拟机
    XTEXTURE_NV12 = 25,     //手机
    XTEXTURE_NV21 = 26
};


class XTexture
{
public:
    static XTexture *Create();
    virtual bool Init(void *win, XTextureType type = XTEXTURE_YUV420P) = 0;
    virtual void Draw(unsigned char *data[], int width, int heigth) = 0;
    virtual void Drop() = 0;

    virtual ~XTexture(){};

protected:
    XTexture(){};
};
#endif //VIDEOPLAY_XTEXTURE_H
