//
// Created by ThinkPad on 2018/11/8.
//
#include "Test.h"
#include <jni.h>
#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "JavaAudioTrackWrapper.h"
#include <vector>
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libswresample/swresample.h"
}


/*可变参数宏 …和__VA_ARGS__
__VA_ARGS__ 是一个可变参数的宏，很少人知道这个宏，这个可变参数的宏是新的C99规范中新增的，目前似乎只有gcc支持（VC6.0的编译器不支持）。
实现思想就是宏定义中参数列表的最后一个参数为省略号（也就是三个点）。这样预定义宏__VA_ARGS__就可以被用在替换部分中，替换省略号所代表的字符串。
*/
#define TAG "c_test"
#define LOG(...) __android_log_print(ANDROID_LOG_VERBOSE,TAG,__VA_ARGS__);

namespace testspace {
    const int SUCCESS = 0;
    const int ERROR = 1;


    int test();

    Test::Test() {

    }

    Test::~Test() {

    }

    int Test::renderCore(JNIEnv *env, jclass clazz, jobject surface) {
        LOG("renderCore start ");
        //注册所有的组件
        av_register_all();
        //打开输入
        //int avformat_open_input(AVFormatContext **ps, const char *url, AVInputFormat *fmt, AVDictionary **options);
        AVFormatContext *avFormatContext = nullptr;
        //文件路径
        char *fileName = "/storage/emulated/0/51vv/mvbox/2.mp4";
        AVInputFormat *avInputFormat = nullptr;
        AVDictionary *avDictionary = nullptr;
        int result = avformat_open_input(&avFormatContext, fileName, avInputFormat, &avDictionary);
        if (result != SUCCESS) {
            LOG("Couldn't open file:%s\n", fileName);
            return ERROR;
        }
        LOG("open file success");
        //查找视频流
        // int avformat_find_stream_info(AVFormatContext *ic, AVDictionary **options);
        result = avformat_find_stream_info(avFormatContext, nullptr);
        if (result != SUCCESS) {
            LOG("find stream fail :\n");
            return ERROR;
        }

        AVCodecContext *avCodecContext = nullptr;
        int videoStreamIndex = -1;
        for (int i = 0; i < avFormatContext->nb_streams; i++) {
            if( avFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO && videoStreamIndex<0){
                videoStreamIndex = i;
                //视频流中的解码上下文环境指针变量
                avCodecContext = avFormatContext->streams[i]->codec;
            }

        }
        if (videoStreamIndex == -1 || avCodecContext == nullptr) {
            LOG("no video stream  :\n");
            return ERROR;
        }

        //获取解码器对象
        //AVCodec *avcodec_find_decoder(enum AVCodecID id);
        AVCodec *avCodec = nullptr;
        avCodec = avcodec_find_decoder(avCodecContext->codec_id);
        if (avCodec == nullptr) {
            LOG("no video stream decoder  :\n");
            return ERROR;
        }

        //int avcodec_open2(AVCodecContext *avctx, const AVCodec *codec, AVDictionary **options);
        result = avcodec_open2(avCodecContext, avCodec, nullptr);
        if (result != SUCCESS) {
            LOG("open fail  :\n");
            return ERROR;
        }
        //读取数据
        //int av_read_frame(AVFormatContext *s, AVPacket *pkt);
        //c中定义的结构体，在c++中可以直接new
        AVPacket *avPacket = new AVPacket();
        AVFrame *avFrame = av_frame_alloc();
        AVFrame *avFrameRGBA = av_frame_alloc();
        int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, avCodecContext->width,
                                                avCodecContext->height, 1);
        //buffer分配的内存地址为"{uint8_t|0xcf580000}"
        uint8_t *buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
        //数组作为方法参数时，不需要考虑数组size（avFrameRGBA->data指针数组）
        av_image_fill_arrays(avFrameRGBA->data, avFrameRGBA->linesize, buffer, AV_PIX_FMT_RGBA,
                             avCodecContext->width, avCodecContext->height, 1);
        /*av_image_fill_arrays()方法执行完毕后的结果，该方法主要是为了给结构体avFrameRGBA中的data数组中指针变量赋值，
        以及给linesize通过像素格式和视频宽度来给linesize赋值，该方法执行完毕后的结果：
          //linesize[0]={int}1440(360*4)
        //data[0]={uint8_t* |0xcf580000}
         可以看出把buffer赋值给了data数组中的第一个位置变量。
         根据宽度360*一个像素（RGBA）4=1440得出一行占据的byte数。
         */

        int got_picture_ptr = 0;
        //根据java层surface得到对应的natice层Surface
        ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
        //通过解码对象获取视频宽和高
        int videoWidth = avCodecContext->width;
        int videoHeight = avCodecContext->height;
        ANativeWindow_setBuffersGeometry(window, videoWidth, videoHeight, WINDOW_FORMAT_RGBA_8888);
        ANativeWindow_Buffer *aNativeWindow_buffer = new ANativeWindow_Buffer();

        struct SwsContext *swsContext = sws_getContext(
                avCodecContext->width,
                avCodecContext->height,
                avCodecContext->pix_fmt,
                avCodecContext->width,
                avCodecContext->height,
                AV_PIX_FMT_RGBA,
                SWS_BILINEAR, nullptr, nullptr, nullptr

        );
        // result =  av_read_frame(avFormatContext,avPacket);
        //循环读取数据
        while (av_read_frame(avFormatContext, avPacket) >= 0) {
            //int avcodec_decode_video2(AVCodecContext *avctx, AVFrame *picture,int *got_picture_ptr,
            // const AVPacket *avpkt);
            if (avPacket->stream_index == videoStreamIndex) {
                //解码数据到avframe
                avcodec_decode_video2(avCodecContext, avFrame, &got_picture_ptr, avPacket);
                if (got_picture_ptr) {
                    //int32_t ANativeWindow_lock(ANativeWindow* window, ANativeWindow_Buffer* outBuffer,
                    // ARect* inOutDirtyBounds);
                    ANativeWindow_lock(window, aNativeWindow_buffer, 0);
                    sws_scale(swsContext, (const uint8_t *const *) avFrame->data, avFrame->linesize,
                              0,
                              avCodecContext->height, avFrameRGBA->data, avFrameRGBA->linesize);

                    /* ANativeWindow_Buffer 中的stride存放的是一行的像素个数，可能大于宽度*/
                    uint8_t *dst = (uint8_t *) aNativeWindow_buffer->bits;
                    //每个像素是由RGBA构成的也就是4个字节构成的，所以变化为byte就是乘以4.
                    int32_t dstStride = aNativeWindow_buffer->stride * 4;

                    /*AVFrame中的linesize存放的是一行所占据的字节数*/
                    uint8_t *src = avFrameRGBA->data[0];
                    int32_t srcStride = avFrameRGBA->linesize[0];

                    //逐行复制数据
                    int h;
                    for (h = 0; h < videoHeight; h++) {
                        memcpy(dst + h * dstStride, src + srcStride * h, srcStride);
                    }

                    ANativeWindow_unlockAndPost(window);
                }
            }
            av_packet_unref(avPacket);

        }
        //释放内存空间
        delete avPacket;
        av_free(avFrame);
        av_free(avFrameRGBA);
        avcodec_close(avCodecContext);
        avformat_close_input(&avFormatContext);
        return result;

    }


    //java AudioTack对应的路径
    static const char *const DL_JAVA_AUDIO_CLASS_NAME = "com/ws/ffmpegandroidplayer/test/AudioTrackWrapper";

/**
 * 解码声音
 * @param env
 * @param clazz
 * @param surface
 * @return
 */
    int Test::renderAudioCore(JNIEnv *env, jclass clazz) {
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
        jobject j_AudioTrack = env->CallObjectMethod(j_AudioTrackWrapper,
                                                     j_AudioTrackWrapper_createAudioTrackMethod,
                                                     1, 3);
        if (!j_AudioTrack) {
            LOG("create java AudioTrack fail !");
        }
        //调用AudioTrack的paly()方法
        //android.media.AudioTrack
        jclass j_AudioTrackClass = env->FindClass("android/media/AudioTrack");
        //public void play()
        jmethodID j_AudioTrack_playMethod = env->GetMethodID(j_AudioTrackClass,"play","()V");
        jmethodID j_AudioTrack_stopMethod = env->GetMethodID(j_AudioTrackClass,"stop","()V");
        env->CallVoidMethod(j_AudioTrack,j_AudioTrack_playMethod);

        //获取write（）方法
        //public int write(@NonNull short[] audioData, int offsetInShorts, int sizeInShorts)
        jmethodID  j_AudioTrack_writeMethod = env->GetMethodID(j_AudioTrackClass,"write","([BII)I");

        LOG("renderCore start ");
        //注册所有的组件
        av_register_all();
        //打开输入
        //int avformat_open_input(AVFormatContext **ps, const char *url, AVInputFormat *fmt, AVDictionary **options);
        AVFormatContext *avFormatContext = nullptr;
        //文件路径
        char *fileName = "/storage/emulated/0/51vv/mvbox/2.mp4";
        AVInputFormat *avInputFormat = nullptr;
        AVDictionary *avDictionary = nullptr;
        int result = avformat_open_input(&avFormatContext, fileName, avInputFormat, &avDictionary);
        if (result != SUCCESS) {
            LOG("Couldn't open file:%s\n", fileName);
            return ERROR;
        }
        LOG("open file success");
        //查找视频流
        // int avformat_find_stream_info(AVFormatContext *ic, AVDictionary **options);
        result = avformat_find_stream_info(avFormatContext, nullptr);
        if (result != SUCCESS) {
            LOG("find stream fail :\n");
            return ERROR;
        }

        AVCodecContext *avCodecContext = nullptr;
        int audioStreamIndex = -1;
        for (int i = 0; i < avFormatContext->nb_streams; i++) {
            if(avFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO && audioStreamIndex <0){
                audioStreamIndex = i;
                //视频流中的解码上下文环境指针变量
                avCodecContext = avFormatContext->streams[i]->codec;
            }
        }
        if (audioStreamIndex == -1 || avCodecContext == nullptr) {
            LOG("no video stream  :\n");
            return ERROR;
        }

        //获取解码器对象
        //AVCodec *avcodec_find_decoder(enum AVCodecID id);
        AVCodec *avCodec = nullptr;
        avCodec = avcodec_find_decoder(avCodecContext->codec_id);
        if (avCodec == nullptr) {
            LOG("no video stream decoder  :\n");
            return ERROR;
        }

        //int avcodec_open2(AVCodecContext *avctx, const AVCodec *codec, AVDictionary **options);
        result = avcodec_open2(avCodecContext, avCodec, nullptr);
        if (result != SUCCESS) {
            LOG("open fail  :\n");
            return ERROR;
        }
        //读取数据
        //int av_read_frame(AVFormatContext *s, AVPacket *pkt);
        //c中定义的结构体，在c++中可以直接new
        AVPacket *avPacket = new AVPacket();
        AVFrame *avFrame = av_frame_alloc();
        SwrContext* swrContext = swr_alloc();
        //输入的采样格式(采样一个数据存储空间的大小)
        enum AVSampleFormat in_sample_fmt = avCodecContext->sample_fmt;
        enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
        //输入采样率
        int in_sample_rate = avCodecContext->sample_rate;
        //输出采样率
        int out_sample_rate = in_sample_rate;
        //声道布局
        uint64_t in_channel_layout = avCodecContext->channel_layout;
        //输出立体声
        uint64_t out_channel_layout = AV_CH_LAYOUT_STEREO;

        swr_alloc_set_opts(swrContext,out_channel_layout,out_sample_fmt,out_sample_rate,
                                      in_channel_layout,in_sample_fmt,in_sample_rate,0,nullptr);
        swr_init(swrContext);
        int out_channel_nb = av_get_channel_layout_nb_channels(out_channel_layout);
        uint8_t* out_buffer = (uint8_t *) av_malloc(2 * 44100);
        int ret,got_frame,frameCount =0;
        while (av_read_frame(avFormatContext,avPacket)>=0){
            if(avPacket->stream_index==audioStreamIndex){
                ret = avcodec_decode_audio4(avCodecContext,avFrame,&got_frame,avPacket);
                if(ret<0){
                    LOG("audio decode finish !");
                }
                if(got_frame){
                    swr_convert(swrContext, &out_buffer,2*44100, (const uint8_t **) avFrame->data, avFrame->nb_samples);
                    int out_buffer_size = av_samples_get_buffer_size(NULL,out_channel_nb,
                                                                     avFrame->nb_samples,out_sample_fmt,1);

                    jbyteArray  audio_sample_array = env->NewByteArray(out_buffer_size);
                    jbyte* sample_byte_array = env->GetByteArrayElements(audio_sample_array, nullptr);
                    memcpy(sample_byte_array,out_buffer,out_buffer_size);
                    env->ReleaseByteArrayElements(audio_sample_array,sample_byte_array,0);
                    env->CallIntMethod(j_AudioTrack,j_AudioTrack_writeMethod,audio_sample_array,0,out_buffer_size);
                    env->DeleteLocalRef(audio_sample_array);

                }

            }
            av_free_packet(avPacket);

        }
        //停止播放
        env->CallVoidMethod(j_AudioTrack,j_AudioTrack_stopMethod);
        //释放内存空间
        delete avPacket;
        av_free(avFrame);
        avcodec_close(avCodecContext);
        avformat_close_input(&avFormatContext);
        return result;

    }

    int Test::renderAudioCore1(JNIEnv *env, jclass clazz) {
        test();
        LOG("renderCore start ");
        //注册所有的组件
        av_register_all();
        //打开输入
        //int avformat_open_input(AVFormatContext **ps, const char *url, AVInputFormat *fmt, AVDictionary **options);
        AVFormatContext *avFormatContext = nullptr;
        //文件路径
        char *fileName = "/storage/emulated/0/51vv/mvbox/2.mp4";
        AVInputFormat *avInputFormat = nullptr;
        AVDictionary *avDictionary = nullptr;
        int result = avformat_open_input(&avFormatContext, fileName, avInputFormat, &avDictionary);
        if (result != SUCCESS) {
            LOG("Couldn't open file:%s\n", fileName);
            return ERROR;
        }
        LOG("open file success");
        //查找视频流
        // int avformat_find_stream_info(AVFormatContext *ic, AVDictionary **options);
        result = avformat_find_stream_info(avFormatContext, nullptr);
        if (result != SUCCESS) {
            LOG("find stream fail :\n");
            return ERROR;
        }

        AVCodecContext *avCodecContext = nullptr;
        int audioStreamIndex = -1;
        for (int i = 0; i < avFormatContext->nb_streams; i++) {
            if(avFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO && audioStreamIndex <0){
                audioStreamIndex = i;
                //视频流中的解码上下文环境指针变量
                avCodecContext = avFormatContext->streams[i]->codec;
            }
        }
        if (audioStreamIndex == -1 || avCodecContext == nullptr) {
            LOG("no video stream  :\n");
            return ERROR;
        }

        //获取解码器对象
        //AVCodec *avcodec_find_decoder(enum AVCodecID id);
        AVCodec *avCodec = nullptr;
        avCodec = avcodec_find_decoder(avCodecContext->codec_id);
        if (avCodec == nullptr) {
            LOG("no video stream decoder  :\n");
            return ERROR;
        }

        //int avcodec_open2(AVCodecContext *avctx, const AVCodec *codec, AVDictionary **options);
        result = avcodec_open2(avCodecContext, avCodec, nullptr);
        if (result != SUCCESS) {
            LOG("open fail  :\n");
            return ERROR;
        }
        //读取数据
        //int av_read_frame(AVFormatContext *s, AVPacket *pkt);
        //c中定义的结构体，在c++中可以直接new
        AVPacket *avPacket = new AVPacket();
        AVFrame *avFrame = av_frame_alloc();
        SwrContext* swrContext = swr_alloc();
        //输入的采样格式(采样一个数据存储空间的大小)
        enum AVSampleFormat in_sample_fmt = avCodecContext->sample_fmt;
        enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
        //输入采样率
        int in_sample_rate = avCodecContext->sample_rate;
        //输出采样率
        int out_sample_rate = in_sample_rate;
        //声道布局
        uint64_t in_channel_layout = avCodecContext->channel_layout;
        //输出立体声
        uint64_t out_channel_layout = AV_CH_LAYOUT_STEREO;

        swr_alloc_set_opts(swrContext,out_channel_layout,out_sample_fmt,out_sample_rate,
                                      in_channel_layout,in_sample_fmt,in_sample_rate,0,nullptr);
        swr_init(swrContext);
        int out_channel_nb = av_get_channel_layout_nb_channels(out_channel_layout);
        uint8_t* out_buffer = (uint8_t *) av_malloc(2 * 44100);
        int ret,got_frame;
        JavaAudioTrackWrapper* javaAudioTrackWrapper  = new JavaAudioTrackWrapper(env);
        javaAudioTrackWrapper->play();
        while (av_read_frame(avFormatContext,avPacket)>=0){
            if(avPacket->stream_index==audioStreamIndex){
                ret = avcodec_decode_audio4(avCodecContext,avFrame,&got_frame,avPacket);
                if(ret<0){
                    LOG("audio decode finish !");
                }
                if(got_frame){
                    swr_convert(swrContext, &out_buffer,2*44100, (const uint8_t **) avFrame->data, avFrame->nb_samples);
                    int out_buffer_size = av_samples_get_buffer_size(NULL,out_channel_nb,
                                                                     avFrame->nb_samples,out_sample_fmt,1);
                    javaAudioTrackWrapper->write(out_buffer,0,out_buffer_size);

                }

            }
            av_free_packet(avPacket);

        }
        //停止播放
        javaAudioTrackWrapper->stop();
        //释放内存空间
        delete avPacket;
        av_free(avFrame);
        avcodec_close(avCodecContext);
        avformat_close_input(&avFormatContext);
        return result;

    }

    int test() {
        LOG("我是测试方法");
        int a = 1;
        return a;
    }
}