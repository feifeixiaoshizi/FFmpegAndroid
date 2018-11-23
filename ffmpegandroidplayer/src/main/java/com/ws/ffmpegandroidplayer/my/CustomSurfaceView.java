package com.ws.ffmpegandroidplayer.my;

import android.content.Context;
import android.graphics.PixelFormat;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.util.AttributeSet;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * 负责接收视频画面
 */
public class CustomSurfaceView extends SurfaceView implements SurfaceHolder.Callback {
    private static  final  String TAG = "CustomSurfaceView" ;
    private RenderThread renderThread;
    private Handler handler;
    //共享变量
    private volatile SurfaceHolder surfaceHolder;

    public CustomSurfaceView(Context context) {
        super(context);
        initSuface();
    }

    public CustomSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        initSuface();
    }

    public CustomSurfaceView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initSuface();
    }

    /**
     * 初始化SurfaceView
     */
    private void initSuface(){
         surfaceHolder = getHolder();
         surfaceHolder.addCallback(this);
         surfaceHolder.setFormat(PixelFormat.TRANSPARENT);
         setZOrderOnTop(true);
     }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        renderThread = new RenderThread(TAG);
        renderThread.start();
        handler = new Handler(renderThread.getLooper(), renderThread);
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        handler =null;
        renderThread.quit();
        renderThread =null;
    }

    public void play(){
        if(handler==null|| renderThread ==null){
            return;
        }
        Message message = Message.obtain();
        message.what = 1;
        handler.sendMessage(message);
    }

    /**
     * 渲染线程
     */
    class RenderThread extends HandlerThread implements Handler.Callback{

        public RenderThread(String name) {
            super(name);
        }

        public RenderThread(String name, int priority) {
            super(name, priority);
        }

        @Override
        public boolean handleMessage(Message msg) {
            switch (msg.what){
                case 1:
                    render();
                    break;
            }
            return false;
        }

        /**
         * 渲染界面
         */
        private void render(){
            if(surfaceHolder==null){
                return;
            }
            Surface surface = surfaceHolder.getSurface();
            if(surface!=null){
                Render.render(surface);
            }
        }

    }


}
