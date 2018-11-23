package com.ws.ffmpegandroidplayer.my;

/**
 * 负责渲染界面
 */
public class Render {
    /*加载so库*/
    static {
        System.loadLibrary("native-lib");
    }

    /**
     * @param surface SurfaceView中的surface对象，负责接收要显示的界面数据
     * @return
     */
    public static  native int render(Object surface);

    public static  native int renderAudio();
}
