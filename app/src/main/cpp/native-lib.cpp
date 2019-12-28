//
// Created by 叶亮 on 2019/1/25.
//

#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "builder/FFPlayerBuilder.h"
#include "IPlayerProxy.h"
#include "Xlog.h"


extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res) {
    FFPlayerBuilder::InitHard(vm);

    IPlayerProxy::Get()->Init(vm);

    return JNI_VERSION_1_4;
}
//===================   初始化View   ===================
extern "C"
JNIEXPORT void JNICALL
Java_com_play_ffmpeg_XPlayView_InitView(JNIEnv *env, jobject instance, jobject surface) {
    //视频显示
    ANativeWindow *win = ANativeWindow_fromSurface(env, surface);
    IPlayerProxy::Get()->InitView(win);
}

//===================   打开播放地址   ===================
extern "C"
JNIEXPORT void JNICALL
Java_com_example_videoplay_PlayListActivity_OpenUrl(JNIEnv *env, jobject instance, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, 0);

    IPlayerProxy::Get()->Open(url);
    IPlayerProxy::Get()->Start();

    env->ReleaseStringUTFChars(url_, url);
}

//===================   获取当前播放位置   ===================

extern "C"
JNIEXPORT jdouble JNICALL
Java_com_play_ffmpeg_MainActivity_getPlayPos(JNIEnv *env, jobject instance) {
    jdouble pos = IPlayerProxy::Get()->PlayPos();
    return pos;
}

//===================         滑动       ===================
extern "C"
JNIEXPORT void JNICALL
Java_com_play_ffmpeg_MainActivity_seekTo(JNIEnv *env, jobject instance, jdouble pos) {
    IPlayerProxy::Get()->Seek(pos);
}

//=========================    暂停  =========================
extern "C"
JNIEXPORT void JNICALL
Java_com_play_ffmpeg_XPlayView_playOnPause(JNIEnv *env, jobject instance) {
    IPlayerProxy::Get()->SetPause(!IPlayerProxy::Get()->IsPause());
}

//=========================    视频采集封装  =========================

extern "C"
JNIEXPORT void JNICALL
Java_com_play_ffmpeg_MainActivity_play(JNIEnv *env, jobject instance, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, 0);

    IPlayerProxy::Get()->Open(url);
    IPlayerProxy::Get()->Start();

    env->ReleaseStringUTFChars(url_, url);
}