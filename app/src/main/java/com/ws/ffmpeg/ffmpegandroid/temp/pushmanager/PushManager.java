package com.ws.ffmpeg.ffmpegandroid.temp.pushmanager;

import com.ws.ffmpeg.ffmpegandroid.temp.gtpush.GTPushInit;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by ThinkPad on 2018/11/13.
 */

public class PushManager {
    private List<IPushInit> pushInits = new ArrayList<>();
    private static volatile PushManager mPushManager;
    public static PushManager getPushManager(){
        if(mPushManager==null){
            synchronized (PushManager.class){
                if(mPushManager==null){
                    mPushManager = new PushManager();
                }
            }
        }
        return mPushManager;
    }

    private PushManager() {
        pushInits.add(new GTPushInit());
    }

    /**
     * 初始化推送
     */
    public void initPush(){
        for(IPushInit iPushInit:pushInits){
            if(iPushInit.handlePhone()){
                iPushInit.init();
                return;
            }
        }
    }

}
