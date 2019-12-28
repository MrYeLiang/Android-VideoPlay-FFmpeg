//
// Created by 叶亮 on 2019/3/4.
//

#include "IPlayer.h"
#include "Xlog.h"
#include "IPlayer.h"
#include "audio/IAudioPlay.h"

IPlayer *IPlayer::Get(unsigned char index)
{
    static IPlayer p[256];
    return &p[index];
}

void IPlayer::Main()
{
    while (!isExit)
    {
        mux.lock();
        if(!audioPlay || !vdecode)
        {
            mux.unlock();
            XSleep(2);
            continue;
        }

        //同步
        //获取音频的pts 告诉音频
        int apts = audioPlay->pts;
        vdecode->synPts = apts;

        mux.unlock();
        XSleep(2);
    }
}

void IPlayer::Close()
{
    mux.lock();

    //先关闭主题线程，再清理观察者
    //同步线程
    XThread::Stop();

    //解封装
    if(demux)
        demux->Stop();

    //解码
    if(vdecode)
        vdecode->Stop();
    if(adecode)
        adecode->Stop();
    if(vdecode)
        vdecode->Stop();

    //2 清理缓冲队列
    if(vdecode)
        vdecode->Clear();
    if(adecode)
        adecode->Clear();
    if(audioPlay)
        audioPlay->Clear();

    //3 清理资源
   if(audioPlay)
        audioPlay->Close();
    if(videoView)
        videoView->Close();
    if(adecode)
        adecode->Close();
    if(vdecode)
        vdecode->Close();
    if(demux)
        demux->Close();
    mux.unlock();

}

bool IPlayer::Open(const char *path)
{
    Close();
    //解封装
    if(!demux || !demux->Open(path)){
        XLOGE("demux->Open %s failed!", path);
        return false;
    }

    //解码 如果解封装之后是原始数据解码可能不需要
    if(!vdecode || !vdecode->Open(demux->GetVPara(),isHardDecode)){
        XLOGE("vdecode->Open %s failed!",path);
        //return false;
    }

    if(!vdecode || !adecode->Open(demux->GetAPara()))
    {
        XLOGE("adecode->Open %s failed!", path);
    }

    //重采样，有可能不需要  解码或者解封装后可能是直接能播放的数据
    if(outPara.sample_rate <= 0) outPara = demux->GetAPara();

    if(!resample || !resample->Open(demux->GetAPara(),outPara))
    {
        XLOGE("resample->Open %s failed!",path);
    }

    return true;
}

bool IPlayer::Start()
{

    mux.lock();
    if(vdecode){
        vdecode->Start();
    }

    if(!demux || !demux->Start())
    {
        XLOGE("demux->Start failed!");
        return false;
    }

    if(adecode)
    {
        adecode->Start();
    }

    if(audioPlay){
        audioPlay->StartPlay(outPara);
    }
    XThread::Start();
    mux.unlock();

    return true;
}

void IPlayer::InitView(void *win)
{
    if(videoView)
    {
        videoView->Close();
        videoView->SetRender(win);
    }
}

//获取当前的播放进度
double  IPlayer::PlayPos(){
    double pos = 0.0;
    mux.lock();
    int total = 0;
    if(demux)
    {
        total = demux->totalMs;
    }

    if(total > 0)
    {
        if(vdecode)
        {
            pos = (double)vdecode->pts/(double)total;
        }
    }

    mux.unlock();

    //XLOGI("IPlayer--> pos = %lf",pos);
    return pos;
}

void IPlayer::SetPause(bool isP)
{
    mux.lock();
    XThread::SetPause(isP);
    if(demux)
        demux->SetPause(isP);
    if(vdecode)
        vdecode->SetPause(isP);
    if(adecode)
        adecode->SetPause(isP);
    if(audioPlay)
        audioPlay->SetPause(isP);

    mux.unlock();
}

bool IPlayer::Seek(double pos)
{
    bool re;
    if(!demux) return false;
    //暂停所有线程
    SetPause(true);
    mux.lock();
    //清理缓冲
    //2 清理缓冲队列
    if(vdecode)
        vdecode->Clear();//清理缓冲队列，清理ffmepg的缓冲
    if(adecode)
        adecode->Clear();
    if(audioPlay)
        audioPlay->Clear();

    re = demux->Seek(pos);//seek跳转到关键帧
    if(!vdecode){
        mux.unlock();
        SetPause(false);
        return re;
    }
    //解码到实际需要显示的帧
    int seekPts = pos*demux->totalMs;
    while (!isExit)
    {
        XData pkt = demux->Read();
        if(pkt.size <= 0) break;
        if(pkt.isAudio){
            if(pkt.pts < seekPts){
                pkt.Drop();
                continue;
            }

            //写入缓冲队列
            demux->Notify(pkt);
            continue;
        }
        //解码需要显示的帧之前的数据
        vdecode->SendPacket(pkt);
        pkt.Drop();
        XData data = vdecode->RecvFrame();
        if(data.size <= 0)
        {
            continue;
        }
        if(data.pts >= seekPts)
        {
            break;
        }
    }
    mux.unlock();

    SetPause(false);
    return re;
}