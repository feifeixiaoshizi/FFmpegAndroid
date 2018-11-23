//
// Created by ThinkPad on 2018/11/16.
//

#ifndef FFMPEGANDROID_JAVAAUDIOTRACK_H
#define FFMPEGANDROID_JAVAAUDIOTRACK_H

#include <cstdint>
#include <jni.h>
/**
 * 类的定义，仅仅负责了声明变量和函数，类在定义的时候是不分配其变量的内存空间的。（*****）
 */
class JavaAudioTrackWrapper{
private:
    //定义静态不可变变量
    static const char *const DL_JAVA_AUDIO_CLASS_NAME;
    JNIEnv *env;
    jobject javaAudioTrack;
    jclass j_AudioTrackClass;
    //采样率
    int frequence;
    //声道
    int channel;
protected:
public:
    //无参构造函数
    JavaAudioTrackWrapper();
    //带参数构造方法（默认参数在声明的时候，初始化列表在定义的时候）
    JavaAudioTrackWrapper(JNIEnv *env,int frequence=44100, int channels=1);
    //拷贝函数
    JavaAudioTrackWrapper(const JavaAudioTrackWrapper& javaAudioTrack);
    //赋值重载
    JavaAudioTrackWrapper& operator=(const JavaAudioTrackWrapper& javaAudioTrack);
    //析构函数
    ~JavaAudioTrackWrapper();

    void  play();
    void  stop();

    /**
     * @param buffer  缓冲区
     * @param offset 偏移量
     * @param size  缓冲区size大小
     */
    void  write(uint8_t* buffer,int offset,int size);
};

#endif //FFMPEGANDROID_JAVAAUDIOTRACK_H
