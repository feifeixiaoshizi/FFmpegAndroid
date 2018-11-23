//
// Created by ThinkPad on 2018/11/16.
//

#include "JavaAudioTrackWrapper.h"
#include <android/log.h>
#include <string.h>


#define TAG "JavaAudioTrackWrapper"
#define LOG(...) __android_log_print(ANDROID_LOG_VERBOSE,TAG,__VA_ARGS__);
/**
 * 初始化参数列表在方法实现实现时不在声明时。（*****）
 */
//初始化静态变量
const char *const JavaAudioTrackWrapper::DL_JAVA_AUDIO_CLASS_NAME = "com/ws/ffmpegandroidplayer/test/AudioTrackWrapper";

JavaAudioTrackWrapper::JavaAudioTrackWrapper() : javaAudioTrack(nullptr), env(nullptr) {
}

/**
 * 初始化列表的构造方法试下实现时定义的
 * @param env
 * @param rate
 * @param chanel
 */
JavaAudioTrackWrapper::JavaAudioTrackWrapper(JNIEnv *env, int rate, int chanel)
        : javaAudioTrack(nullptr), env(env), frequence(rate), channel(chanel) {
    j_AudioTrackClass = nullptr;
    j_AudioTrackClass = env->FindClass("android/media/AudioTrack");
    jclass j_AudioTrackWrapperClass = env->FindClass(DL_JAVA_AUDIO_CLASS_NAME);
    //查找构造方法，传入方法名字为<init>,直接传入类名找不到方法(****)
    jmethodID j_AudioTrackWrapperConstructMethod = env->GetMethodID(j_AudioTrackWrapperClass,
                                                                    "<init>", "()V");
    //调用构造方法构建java对象
    jobject j_AudioTrackWrapper = env->NewObject(j_AudioTrackWrapperClass,
                                                 j_AudioTrackWrapperConstructMethod);
    //找到java对象的方法对象
    jmethodID j_AudioTrackWrapper_createAudioTrackMethod = env->GetMethodID(
            j_AudioTrackWrapperClass, "createAudioTrack", "(II)Landroid/media/AudioTrack;");
    if (!j_AudioTrackWrapper_createAudioTrackMethod) {
        LOG("not found create AudioTrack method !");
    }
    //使用java对象调用java方法
    javaAudioTrack = env->CallObjectMethod(j_AudioTrackWrapper,
                                           j_AudioTrackWrapper_createAudioTrackMethod,
                                           frequence, channel);
}

JavaAudioTrackWrapper::JavaAudioTrackWrapper(const JavaAudioTrackWrapper &javaAudioTrack) {
    env= javaAudioTrack.env;
    this->javaAudioTrack = javaAudioTrack.javaAudioTrack;
    this->j_AudioTrackClass = j_AudioTrackClass;
    /*JNIEnv *env;
    jobject javaAudioTrack;
    jclass j_AudioTrackClass;
    //采样率
    int frequence;
    //声道
    int channel;*/
}

JavaAudioTrackWrapper &JavaAudioTrackWrapper::operator=(
        const JavaAudioTrackWrapper &javaAudioTrack) {
    if(this == &javaAudioTrack){
        return *this;
    }
}

void JavaAudioTrackWrapper::play() {
    if (!javaAudioTrack) {
        LOG("javaAudioTrack is null!");
        return;
    }
    //public void play()
    jmethodID j_AudioTrack_playMethod = env->GetMethodID(j_AudioTrackClass, "play", "()V");
    env->CallVoidMethod(javaAudioTrack, j_AudioTrack_playMethod);
    return;

}

void JavaAudioTrackWrapper::stop() {
    if (!javaAudioTrack) {
        LOG("javaAudioTrack is null!");
        return;
    }
    jmethodID j_AudioTrack_stopMethod = env->GetMethodID(j_AudioTrackClass, "stop", "()V");
    env->CallVoidMethod(javaAudioTrack, j_AudioTrack_stopMethod);
    return;
}

void JavaAudioTrackWrapper::write(uint8_t *buffer, int offset, int size) {
    if (!javaAudioTrack) {
        LOG("javaAudioTrack is null!");
        return;
    }
    //获取write（）方法
    //public int write(@NonNull short[] audioData, int offsetInShorts, int sizeInShorts)
    jmethodID j_AudioTrack_writeMethod = env->GetMethodID(j_AudioTrackClass, "write", "([BII)I");
    jbyteArray  audio_sample_array = env->NewByteArray(size);
    jbyte* sample_byte_array = env->GetByteArrayElements(audio_sample_array, nullptr);
    memcpy(sample_byte_array,buffer,size);
    env->ReleaseByteArrayElements(audio_sample_array,sample_byte_array,0);
    env->CallIntMethod(javaAudioTrack,j_AudioTrack_writeMethod,audio_sample_array,0,size);
    env->DeleteLocalRef(audio_sample_array);
    return;
}