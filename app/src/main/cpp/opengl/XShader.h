//
// Created by 叶亮 on 2019/2/12.
//

#ifndef VIDEOPLAY_XSHADER_H
#define VIDEOPLAY_XSHADER_H

#include <mutex>

enum XShaderType
{
    XSHADER_YUV420P = 0,
    XSHADER_NV12 = 25,
    XSHADER_NV21 = 26
};

class XShader
{
public:
    virtual void Close();
    virtual bool Init(XShaderType type = XSHADER_YUV420P);

    //获取材质并映射到内存
    virtual void GetTexture(unsigned int index, int width, int height, unsigned char *buf, bool isa = false);
    virtual void Draw();

protected:
    unsigned  int vsh = 0;
    unsigned  int fsh = 0;
    unsigned  int program = 0;
    unsigned  int texts[100] = {0};
    std::mutex mux;
};
#endif //VIDEOPLAY_XSHADER_H
