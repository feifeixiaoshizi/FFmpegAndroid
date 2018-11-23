package com.ws.ffmpeg.ffmpegandroid.temp.gtpush;

import android.content.Context;
import com.ws.ffmpeg.ffmpegandroid.temp.BaseMessage;

/**
 * Created by ThinkPad on 2018/11/6.
 */

public class GTMessage extends BaseMessage {

    public static final int PUSH_MESSAGE_TYPE_PID = 0;
    public static final int PUSH_MESSAGE_TYPE_CLIENTID = 1;
    public static final int PUSH_MESSAGE_TYPE_MESSAGE_DATA = 2;
    /**
     * 推送消息类型
     */
    private int pushMessageType;
    private int pid;
    private String cid;
    private Context context;
    private boolean state;

    public static GTMessage create() {
        return new GTMessage();
    }

    @Override
    public int getMessageFromType() {
        return 0;
    }

    @Override
    public String getMessageContent() {
        return null;
    }

    @Override
    public int getMessageContentType() {
        return 0;
    }

    @Override
    public void onDealMessage() {
        switch (pushMessageType) {
            case PUSH_MESSAGE_TYPE_MESSAGE_DATA:
                socialNetworkSystem.resolveMessage(this);
                break;
        }
    }

    public int getPushMessageType() {
        return pushMessageType;
    }

    public GTMessage setPushMessageType(int pushMessageType) {
        this.pushMessageType = pushMessageType;
        return this;
    }

    public int getPid() {
        return pid;
    }

    public GTMessage setPid(int pid) {
        this.pid = pid;
        return this;
    }

    public String getCid() {
        return cid;
    }

    public GTMessage setCid(String cid) {
        this.cid = cid;
        return this;
    }

    public Context getContext() {
        return context;
    }

    public GTMessage setContext(Context context) {
        this.context = context;
        return this;
    }

    public boolean isState() {
        return state;
    }

    public GTMessage setState(boolean state) {
        this.state = state;
        return this;
    }


}
