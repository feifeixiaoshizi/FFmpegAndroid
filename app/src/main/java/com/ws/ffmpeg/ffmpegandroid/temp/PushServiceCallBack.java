package com.ws.ffmpeg.ffmpegandroid.temp;


/**
 * 负责接收消息
 */
public interface PushServiceCallBack {
    /**
     * 接收到消息
     * @param message
     */
    void onReceiveMessage(BaseMessage message);

    /**
     *
     * @param cid
     */
    void onReceiveClientId(String cid);

}
