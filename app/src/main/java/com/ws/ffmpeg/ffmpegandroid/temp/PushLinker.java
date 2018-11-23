package com.ws.ffmpeg.ffmpegandroid.temp;

/**
 * 1.实现接收消息接口
 * 2.封装NetWork来具体干活
 */
public class PushLinker implements PushServiceCallBack {
    private SocialNetworkSystem socialNetworkSystem;
    private static  volatile PushLinker mInstance;

    private PushLinker() {
    }

    public static PushLinker getInstance(){
        if(mInstance==null){
            synchronized (PushLinker.class){
                if(mInstance==null){
                    mInstance=new PushLinker();
                }
            }
        }
        return mInstance;
    }

    public void setSocialNetworkSystem(SocialNetworkSystem socialNetworkSystem) {
        this.socialNetworkSystem = socialNetworkSystem;
    }

    @Override
    public void onReceiveMessage(BaseMessage message) {
        socialNetworkSystem.resolveMessage(message);
    }

    @Override
    public void onReceiveClientId(String cid) {
        socialNetworkSystem.resolveClientId(cid);
    }
}
