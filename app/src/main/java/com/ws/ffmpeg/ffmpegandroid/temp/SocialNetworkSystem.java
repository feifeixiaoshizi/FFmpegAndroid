package com.ws.ffmpeg.ffmpegandroid.temp;

import android.util.Log;

/**
 * 具体的处理业务逻辑
 */
public class SocialNetworkSystem {

    public SocialNetworkSystem() {
        PushLinker.getInstance().setSocialNetworkSystem(this);
    }

    public void resolveMessage(BaseMessage baseMessage){
        //TODO 具体解析消息的工作
        Log.d("",baseMessage.getMessageContent());
    }


    public void resolveClientId(String cid){

    }





}
