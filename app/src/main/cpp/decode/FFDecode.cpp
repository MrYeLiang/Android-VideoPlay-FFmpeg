//
// Created by 叶亮 on 2019/2/12.
//

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavcodec/jni.h>

#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>

}

#include "FFDecode.h"
#include "../Xlog.h"

void FFDecode::InitHard(void *vm)
{
    av_jni_set_java_vm(vm,0);
}

void FFDecode::Clear()
{
    IDecode::Clear();
    mux.lock();
    if(codec)
        avcodec_flush_buffers(codec);
    mux.unlock();
}

void FFDecode::Close()
{
    IDecode::Clear();
    mux.lock();
    pts = 0;
    if(frame)
    {
        av_frame_free(&frame);
    }

    if(codec)
    {
        avcodec_close(codec);
        avcodec_free_context(&codec);
    }
    mux.unlock();
}


bool FFDecode::Open(XParameter para, bool isHard)
{
    Close();
    if(!para.para){
        return false;
    }
    AVCodecParameters *p = para.para;

    //1查找解码器
    AVCodec *cd = avcodec_find_decoder(p->codec_id);

    if(false)
    {
        cd = avcodec_find_decoder_by_name("h264_mediacodec");
    }

    if(!cd){
        XLOGE("avcodec_find_decoder %d failed", p->codec_id);
        return false;
    }

    XLOGI("avcodec_find_decoder success!");

    mux.lock();
    //2 创建解码上下文 并复制参数
    codec = avcodec_alloc_context3(cd);
    avcodec_parameters_to_context(codec, p);

    codec->thread_count = 8;

    //3 打开解码器
    int re = avcodec_open2(codec, 0, 0);
    if(re != 0){
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf)-1);
        XLOGE("%s", buf);
        return false;
    }

    if(codec->codec_type == AVMEDIA_TYPE_VIDEO){
        this->isAudio = false;
        //initFilter(para);
    }else{
        this->isAudio = true;
        ///initFilter(para);
    }

    mux.unlock();

    XLOGI("avcodec_open2 success!");
    return true;
}

bool FFDecode::SendPacket(XData pkt)
{
    if(pkt.size <= 0 || !pkt.data)
    {
        return false;
    }
    mux.lock();

    if(!codec)
    {
        return false;
    }

    int re = avcodec_send_packet(codec, (AVPacket *)pkt.data);
    mux.unlock();
    if(re != 0){
        return false;
    }

    return true;
}

//从线程中获取解码结果
XData FFDecode::RecvFrame()
{
    mux.lock();

    if(!codec)
    {
        mux.unlock();
        return XData();
    }

    if(!frame){
        frame = av_frame_alloc();
    }

    int re = avcodec_receive_frame(codec,frame);
    if(re != 0)
    {
        mux.unlock();
        return XData();
    }
    XData d;

    if(codec->codec_type == AVMEDIA_TYPE_VIDEO){
        d.size = (frame->linesize[0] + frame->linesize[1] + frame->linesize[2])*frame->height;
        d.width = frame->width;
        d.height = frame->height;

        //filterFrame(frame);
    }else
    {
        //样本字节数 * 单通道样本数 * 通道数
        d.size = av_get_bytes_per_sample((AVSampleFormat)frame->format)*frame->nb_samples*2;
        //filterFrame(frame);
    }
    d.data = (unsigned char *)frame;
    d.format = frame->format;
    memcpy(d.datas, frame->data, sizeof(d.datas));
    d.pts = frame->pts;
    pts = d.pts;

    mux.unlock();
    return d;
}

int FFDecode::initFilter(XParameter parameter){
    //1 注册滤波器组件
    avfilter_register_all();

    //2 初始化图状结构
    filter_graph = avfilter_graph_alloc();

    //3.1 获取filter
    AVFilter *buffersrc  = avfilter_get_by_name("abuffer"); /* 输入buffer filter */

    int ret;
    char args[512];
    AVRational time_base = *(parameter.time_base);/* 时间基数 */
    snprintf(args, sizeof(args),
             "time_base=%d/%d:sample_rate=%d:sample_fmt=%s:channel_layout=0x%" PRIx64,
             time_base.num, time_base.den, codec->sample_rate,
             av_get_sample_fmt_name(codec->sample_fmt), codec->channel_layout);

    ret = avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in", args, NULL, filter_graph);
    if (ret < 0) {
        XLOGE("Cannot create buffer source === %d\n",ret);
        return ret;
    }

    //3.2 获取filter
    AVFilter *buffersink = avfilter_get_by_name("abuffersink"); /* 输出buffer filter */

    ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out", NULL, NULL, filter_graph);
    if (ret < 0) {
        XLOGE("Cannot create buffer sink\n");
        return ret;
    }

    XLOGI("avfilter_graph_create_filter suc");

    //4.1 标识Filter在哪个节点后面
    AVFilterInOut *outputs = avfilter_inout_alloc();
    outputs->name       = av_strdup("in");
    outputs->filter_ctx = buffersrc_ctx;
    outputs->pad_idx    = 0;
    outputs->next       = NULL;

    AVFilterInOut *inputs  = avfilter_inout_alloc();
    inputs->name       = av_strdup("out");
    inputs->filter_ctx = buffersink_ctx;
    inputs->pad_idx    = 0;
    inputs->next       = NULL;

    char *filters_descr;

    //改变颜色水印
    char *filters_color = "lutyuv='u=128:v=128'";

    //文字水印，暂不支持可能编译时没有添加文字选项
    char *filters_text = "drawtext=fontsize=20:fontfile=Kaiti.ttf:text='中文水印':fontcolor=green:box=1:boxcolor=yellow";

    //添加图片水印
    char *filters_img = "movie=/storage/emulated/0/filter_logo.png[wm];[in][wm]overlay=100:100[out]";

    //视频二倍速
    char *filters_speed_video = "setpts=0.5*PTS";

    //音频二倍速
    char *filters_speed_audio = "atempo=tempo=1.1";

    //音视频二倍速
    char * filter_spped_both = "[0:v]setpts=0.5*PTS[v];[0:a]atempo=2.0[a]";

    filters_descr = filters_speed_audio;

    //5 将这个filter加入图中
//    ret = avfilter_graph_parse_ptr(filter_graph, filters_descr, &inputs, &outputs, NULL);
//    if (ret < 0) {
//        XLOGE("Cannot avfilter_graph_parse_ptr\n");
//        return ret;
//    }

    avfilter_inout_free(&outputs);
    avfilter_inout_free(&inputs);

    //6 配置构建的图
    ret = avfilter_graph_config(filter_graph, NULL);

    if (ret < 0) {//检查当前所构造的FilterGraph的完整性与可用性
        XLOGE("Cannot avfilter_graph_config eror = %s",av_err2str(ret));
        return ret;
    }
    XLOGI("avfilter_graph_config suc");

    return ret;
}

int FFDecode::filterFrame(AVFrame *frame) {
    int ret;
    //把解码后视频帧添加到filter_graph
    ret = av_buffersrc_add_frame_flags(buffersrc_ctx, frame, AV_BUFFERSRC_FLAG_KEEP_REF);
    if (ret < 0) {
        XLOGE("Could not av_buffersrc_add_frame");
        return ret;
    }

    XLOGI("11111111");
    //把滤波后的视频帧从filter graph取出来
    while ((ret = av_buffersink_get_frame(buffersink_ctx, frame)) >= 0) {
        /* now do something with our filtered frame */
        XLOGI("22222222------------------");
        if (ret < 0) {
            XLOGE("Could not av_buffersink_get_frame");
        }
        av_frame_unref(frame);
    }
    return ret;
}

