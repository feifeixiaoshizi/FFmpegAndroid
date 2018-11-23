package com.ws.ffmpeg.ffmpegandroid.temp;

/**
 * 推送消息的基类
 */
public abstract class BaseMessage{
    protected SocialNetworkSystem socialNetworkSystem;
    //原始消息对象
    protected Object originMessage;

    /**
     * 消息来源（推送来源）
     * @return
     */
    public abstract int getMessageFromType();

    /**
     * 消息内容
     * @return
     */
    public abstract String getMessageContent();

    /**
     * 消息类型（用户动态，聊天，运营，登录，直播下发...）
     * @return
     */
    public abstract int getMessageContentType();

    public abstract void onDealMessage();

    public SocialNetworkSystem getSocialNetworkSystem() {
        return socialNetworkSystem;
    }

    public final void dealMessage(SocialNetworkSystem SocialNetworkSystem) {
        this.socialNetworkSystem = SocialNetworkSystem;
        onDealMessage();
    }

    public Object getOriginMessage() {
        return originMessage;
    }

    public <T> T setOriginMessage(Object originMessage) {
        this.originMessage = originMessage;
        return (T)this;
    }
}
