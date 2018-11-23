//
// Created by ThinkPad on 2018/11/8.
//

#include <jni.h>

#ifndef FFMPEGANDROID_TEST_H
#define FFMPEGANDROID_TEST_H

namespace testspace {
    class Test {
    private:
    protected:
    public:
        Test();

        ~Test();

        static int renderCore(JNIEnv *env, jclass clazz, jobject surface);

        static int renderAudioCore(JNIEnv *env, jclass clazz);
        static int renderAudioCore1(JNIEnv *env, jclass clazz);

    };
}

#endif //FFMPEGANDROID_TEST_H
