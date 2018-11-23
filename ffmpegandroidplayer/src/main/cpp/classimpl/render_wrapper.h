//
// Created by ThinkPad on 2018/11/7.
//

#include <jni.h>

#ifndef RENDER_H
#define RENDER_H

/*负责和java沟通*/
extern "C"
JNIEXPORT jint JNICALL
Java_com_ws_ffmpegandroidplayer_my_Render_render(JNIEnv *env, jclass type, jobject surface);
extern "C"
JNIEXPORT jint JNICALL
Java_com_ws_ffmpegandroidplayer_my_Render_renderAudio(JNIEnv *env, jclass type);
#endif
