#include <jni.h>
#include <string>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>

jstring
Java_com_ws_ffmpeg_ffmpegandroid_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

jstring
Java_com_ws_ffmpeg_ffmpegandroid_MainActivity_urlprotocolinfo(
        JNIEnv *env, jobject) {
    char info[40000] = {0};
    av_register_all();

    struct URLProtocol *pup = NULL;

    //二级指针作为函数参数的作用:在函数外部定义一个指针p，在函数内给指针赋值，函数结束后对指针p生效，
    // 那么我们就需要二级指针。
    struct URLProtocol **p_temp = &pup;
    //直接在方法参数中传入指针实际上是指针的变量的副本。
    avio_enum_protocols((void **) p_temp, 0);

    while ((*p_temp) != NULL) {
        //avio_enum_protocols((void **) p_temp, 0) 0表示遍历输入类型协议
        /**
         * sprintf()往指定的空间内，存放格式化后的字符串
         * 把格式化化的"%sInput: %s\n"，其中前一个%s是为了原来已经有的字符串内容，后一个%s
         * 是要追加的内容。
         */
        sprintf(info, "%sInput: %s\n", info, avio_enum_protocols((void **) p_temp, 0));
    }
    pup = NULL;
    avio_enum_protocols((void **) p_temp, 1);
    while ((*p_temp) != NULL) {
        //avio_enum_protocols((void **) p_temp, 1) 1表示遍历输出类型协议
        sprintf(info, "%sOutput: %s\n", info, avio_enum_protocols((void **) p_temp, 1));
    }
    return env->NewStringUTF(info);
}

jstring
Java_com_ws_ffmpeg_ffmpegandroid_MainActivity_avformatinfo(
        JNIEnv *env, jobject) {
    char info[40000] = {0};

    //注册ffmpeg所有的组件
    av_register_all();

    //输入格式
    AVInputFormat *if_temp = av_iformat_next(NULL);
    //输出格式
    AVOutputFormat *of_temp = av_oformat_next(NULL);
    while (if_temp != NULL) {
        sprintf(info, "%sInput: %s\n", info, if_temp->name);
        if_temp = if_temp->next;
    }
    while (of_temp != NULL) {
        sprintf(info, "%sOutput: %s\n", info, of_temp->name);
        of_temp = of_temp->next;
    }
    return env->NewStringUTF(info);
}

jstring
Java_com_ws_ffmpeg_ffmpegandroid_MainActivity_avcodecinfo(
        JNIEnv *env, jobject) {
    char info[40000] = {0};

    av_register_all();

    //解码类型
    AVCodec *c_temp = av_codec_next(NULL);

    while (c_temp != NULL) {
        if (c_temp->decode != NULL) {
            sprintf(info, "%sdecode:", info);
        } else {
            sprintf(info, "%sencode:", info);
        }
        switch (c_temp->type) {
            case AVMEDIA_TYPE_VIDEO:
                sprintf(info, "%s(video):", info);
                break;
            case AVMEDIA_TYPE_AUDIO:
                sprintf(info, "%s(audio):", info);
                break;
            default:
                sprintf(info, "%s(other):", info);
                break;
        }
        sprintf(info, "%s[%10s]\n", info, c_temp->name);
        c_temp = c_temp->next;
    }

    return env->NewStringUTF(info);
}

jstring
Java_com_ws_ffmpeg_ffmpegandroid_MainActivity_avfilterinfo(
        JNIEnv *env, jobject) {
    char info[40000] = {0};
    avfilter_register_all();

    //滤镜类型
    AVFilter *f_temp = (AVFilter *) avfilter_next(NULL);
    while (f_temp != NULL) {
        sprintf(info, "%s%s\n", info, f_temp->name);
        f_temp = f_temp->next;
    }
    return env->NewStringUTF(info);
}

}

