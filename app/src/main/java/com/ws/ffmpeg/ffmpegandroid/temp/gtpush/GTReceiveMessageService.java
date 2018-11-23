package com.ws.ffmpeg.ffmpegandroid.temp.gtpush;

import android.content.Context;

import com.ws.ffmpeg.ffmpegandroid.temp.PushLinker;
import com.ws.ffmpeg.ffmpegandroid.temp.pushmanager.IPushInit;

/**
 * 模拟个推接收消息的服务
 */
public class GTReceiveMessageService {
    public void onReceiveServicePid(Context context, int i) {
    }

    public void onReceiveClientId(Context context, String s) {

    }

    public void onReceiveMessageData(Context context, GTTransmitMessage gtTransmitMessage) {
        PushLinker.getInstance().onReceiveMessage(GTMessage.create()
                .setPushMessageType(GTMessage.PUSH_MESSAGE_TYPE_MESSAGE_DATA)
                .<GTMessage>setOriginMessage(gtTransmitMessage)
                .setContext(context));
    }

    public void onReceiveOnlineState(Context context, boolean b) {

    }

    public void onReceiveCommandResult(Context context, GTCmdMessage gtCmdMessage) {

    }


    //模拟变量
    static class GTTransmitMessage{

    }

    //模拟变量
    static class GTCmdMessage{

    }
}
