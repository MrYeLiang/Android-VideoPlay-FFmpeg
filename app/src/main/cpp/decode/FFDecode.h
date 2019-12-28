//
// Created by 叶亮 on 2019/2/12.
//

#ifndef VIDEOPLAY_FFDECODE_H
#define VIDEOPLAY_FFDECODE_H

#include "IDecode.h"

struct AVCodecContext;
struct AVFrame;
struct AVFilterContext;
struct AVFilterGraph;

class FFDecode:public IDecode
{
public:
    static void InitHard(void *vm);

    virtual bool Open(XParameter para, bool isHard = false);
    virtual void Close();
    virtual void Clear();

    //future模型 发送数据到线程解码
    virtual bool SendPacket(XData pkt);

    //从线程中获取解码结果
    virtual XData RecvFrame();

    virtual int initFilter(XParameter parameter);
    virtual int filterFrame(AVFrame *frame);
protected:
    AVCodecContext *codec = 0;
    AVFrame *frame = 0;
    std::mutex mux;

    AVFilterContext *buffersink_ctx;
    AVFilterContext *buffersrc_ctx;//认为它是AVFilter的一个实例就OK了
    AVFilterGraph *filter_graph;
};
#endif //VIDEOPLAY_FFDECODE_H
