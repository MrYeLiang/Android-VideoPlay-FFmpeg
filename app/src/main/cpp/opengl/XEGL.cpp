//
// Created by 叶亮 on 2019/2/12.
//
#include <EGL/egl.h>
#include <mutex>
#include "XEGL.h"
#include "../Xlog.h"

class CXEGL:public XEGL
{
public:
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext context = EGL_NO_CONTEXT;
    std::mutex mux;

    virtual void Draw()
    {
        mux.lock();
        if(display == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE){
            mux.unlock();
            return;
        }
        eglSwapBuffers(display, surface);
        mux.unlock();
    }


    virtual void Close()
    {
        mux.lock();
        if(display == EGL_NO_DISPLAY)
        {
            mux.unlock();
            return;
        }

        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

        if(surface != EGL_NO_SURFACE)
            eglDestroySurface(display, surface);
        if(context != EGL_NO_CONTEXT)
            eglDestroyContext(display,context);

        eglTerminate(display);

        display = EGL_NO_DISPLAY;
        surface = EGL_NO_SURFACE;
        context = EGL_NO_CONTEXT;
        mux.unlock();
    }

    virtual bool Init(void *win)
    {

        ANativeWindow *nwin = (ANativeWindow *)win;
        Close();
        mux.lock();
        //初始化EGL
        //1 获取EGLDisPlay对象 显示设备
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

        if(display == EGL_NO_DISPLAY){
            mux.unlock();
            XLOGE("eglGetDisplay failed!");
            return false;
        }

        XLOGE("eglGetDisplay success!");

        //2初始化Display
        if(EGL_TRUE != eglInitialize(display, 0, 0)){
            mux.unlock();
            XLOGE("eglInitialize failed!");
            return false;
        }
        XLOGE("eglInitialize success!");

        //3 获取配置并创建surface
        EGLint configSpec [] = {
                EGL_RED_SIZE,8,
                EGL_GREEN_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_NONE
        };
        EGLConfig config = 0;
        EGLint numConfig = 0;
        if(EGL_TRUE != eglChooseConfig(display, configSpec, &config, 1, &numConfig)){
            mux.unlock();
            XLOGE("eglChooseConfig failed!");
            return false;
        }
        surface = eglCreateWindowSurface(display, config, nwin, NULL);

        //4创建并打开EGL上下文
        const EGLint ctxAttr[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
        context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxAttr);

        if(context == EGL_NO_CONTEXT){
            mux.unlock();
            XLOGE("eglChooseContext failed!");
            return false;
        }
        XLOGE("eglChooseContext success!");

        if(EGL_TRUE != eglMakeCurrent(display, surface, surface, context)){
            mux.unlock();
            XLOGE("eglMakeCurrent failed!");
            return false;
        }

        XLOGE("eglMakeCurrent success!");
        mux.unlock();
        return true;
    }
};
XEGL *XEGL::Get()
{
    static CXEGL egl;
    return &egl;
}