//
// Created by ThinkPad on 2018/11/7.
//

#include "render_wrapper.h"
#include <jni.h>
#include <android/native_window_jni.h>
#include "Render.h"
#define  LOG_TAG    "render"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

#include "../test/Test.h"

/*负责和java沟通*/
extern "C"
JNIEXPORT jint JNICALL
Java_com_ws_ffmpegandroidplayer_my_Render_render(JNIEnv *env, jclass type, jobject surface) {
    //renderspace::Render::render(env,type,surface);
    /**
     * 忘记加return 在播放完毕后会崩溃：signal 4 (SIGILL), code 1 (ILL_ILLOPC), fault addr 0xe37ee0d8
     * （*****，调试的时候哪行收到的Singal就是哪行存在问题）
     */
    return testspace::Test::renderCore(env,type,surface);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_ws_ffmpegandroidplayer_my_Render_renderAudio(JNIEnv *env, jclass type){
    return testspace::Test::renderAudioCore1(env,type);
}




