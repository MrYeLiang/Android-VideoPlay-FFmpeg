package com.play.ffmpeg;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.View;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Author: yeliang(yeliang@tv365.net)
 * Date: 2019/2/12
 * Time: 6:50 PM
 * Description:
 */

public class XPlayView extends GLSurfaceView implements SurfaceHolder.Callback, View.OnClickListener, GLSurfaceView.Renderer{

    public XPlayView(Context context, AttributeSet attrs) {
        super(context, attrs);
        setOnClickListener(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        InitView(holder.getSurface());
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.O){
            setRenderer(this);
        }

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    public native void InitView(Object surface);

    public native void playOnPause();

    @Override
    public void onClick(View v) {
        playOnPause();
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {

    }

    @Override
    public void onDrawFrame(GL10 gl) {

    }
}
