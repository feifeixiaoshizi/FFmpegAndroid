//
// Created by ThinkPad on 2018/11/7.
//

#include "Render.h"
#include <jni.h>
#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}

#define  LOG_TAG    "render"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
/**
 *渲染类的具体实现
 */
namespace renderspace {

    Render::Render() {

    }

    Render::~Render() {

    }

    int Render::render(JNIEnv *env, jclass type, jobject surface) {
        LOGD("play");
        //rendercore(env,type,surface);

// sd卡中的视频文件地址,可自行修改或者通过jni传入
//char *file_name = "/storage/emulated/0/ws2.mp4";
        char *file_name = "/storage/emulated/0/51vv/mvbox/2.mp4";

        av_register_all();
//上下文环境,指针变两年必须赋值才可以使用，否则声明的指针变量未指向有效控件。
        //通过函数返回一个指针变量（只要返回的地址是有效的即可，比如堆中的内存，全局变量地址，静态区变量地址）
        //通过函数avformat_alloc_context（）为指针分配一块内存区域
        //AVFormatContext *pFormatCtx = avformat_alloc_context(); //√ 给指针变量设置默认值
        AVFormatContext *pFormatCtx= nullptr; //√ 给指针变量设置为指向空
        //AVFormatContext *pFormatCtx;//x 指针变量仅仅声明，未初始化，其值是个脏值（0x4300000000）。使用会发生不确定的问题。
        long  address = reinterpret_cast<long>(&pFormatCtx);//取pFromatCtx指针变量的地址值，转换为long类型打印

        //以下两行是为了看指针变量pFormatCtx中存放的值。先把pFormatCtx类型变换为long*类型再取值。
        //(******)reinterpret_cast<long*>(pFormatCtx);执行后pForamtCtx变量的类型就变换了，不能再传递给avformat_open_input（）用了，会报错。所以注释掉。
        //long*  value = reinterpret_cast<long*>(pFormatCtx);
        //long value1 = *value;

// Open video file 开发视频文件，并根据视频文件填充AVFormatContext对象中的信息
        if (avformat_open_input(&pFormatCtx, file_name, NULL, NULL) != 0) {
            LOGD("Couldn't open file:%s\n", file_name);
            return -1; // Couldn't open file
        }

// Retrieve stream information
//获取视频信息
        if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
            LOGD("Couldn't find stream information.");
            return -1;
        }

// Find the first video stream
//找到数据中的视频流对象
        int videoStream = -1, i;
        for (i = 0; i < pFormatCtx->nb_streams; i++) {
            if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO
                && videoStream < 0) {
                videoStream = i;
            }
        }
        if (videoStream == -1) {
            LOGD("Didn't find a video stream.");
            return -1; // Didn't find a video stream
        }

// Get a pointer to the codec context for the video stream
//解码上下文环境
        AVCodecContext *pCodecCtx = pFormatCtx->streams[videoStream]->codec;

// Find the decoder for the video stream
        AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
        if (pCodec == NULL) {
            LOGD("Codec not found.");
            return -1; // Codec not found
        }

        if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
            LOGD("Could not open codec.");
            return -1; // Could not open codec
        }

// 获取native window 根据java层传递进来的surface，构建native层对应的对象
        ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);

// 获取视频宽高
        int videoWidth = pCodecCtx->width;
        int videoHeight = pCodecCtx->height;

// 设置native window的buffer大小,可自动拉伸
        ANativeWindow_setBuffersGeometry(nativeWindow, videoWidth, videoHeight,
                                         WINDOW_FORMAT_RGBA_8888);
//缓冲buffer结构体
        ANativeWindow_Buffer windowBuffer;

//打开解码
        if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
            LOGD("Could not open codec.");
            return -1; // Could not open codec
        }

// Allocate video frame
//一帧视频数据
        AVFrame *pFrame = av_frame_alloc();

// 用于渲染
        AVFrame *pFrameRGBA = av_frame_alloc();
        if (pFrameRGBA == NULL || pFrame == NULL) {
            LOGD("Could not allocate video frame.");
            return -1;
        }

// Determine required buffer size and allocate buffer
// buffer中数据就是用于渲染的,且格式为RGBA
//一帧数据的byte数
        int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, pCodecCtx->width,
                                                pCodecCtx->height,
                                                1);
//uint8_t表示8bit  一帧数据多少个bit位
//申请一帧所需的内存区域，赋值给pFrameRGBA->data指针数组的第一位，以便存放真正图片数据
        uint8_t *buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
//把buffer赋值给指针数组（pFrameRGBA->data）中（数组作为参数，传递的首个地址，数组大小无需匹配）
        av_image_fill_arrays(pFrameRGBA->data, pFrameRGBA->linesize, buffer, AV_PIX_FMT_RGBA,
                             pCodecCtx->width, pCodecCtx->height, 1);

// 由于解码出来的帧格式不是RGBA的,在渲染之前需要进行格式转换
        struct SwsContext *sws_ctx = sws_getContext(pCodecCtx->width,
                                                    pCodecCtx->height,
                                                    pCodecCtx->pix_fmt,
                                                    pCodecCtx->width,
                                                    pCodecCtx->height,
                                                    AV_PIX_FMT_RGBA,
                                                    SWS_BILINEAR,
                                                    NULL,
                                                    NULL,
                                                    NULL);

        int frameFinished;
        AVPacket packet;
        while (av_read_frame(pFormatCtx, &packet) >= 0) {
// Is this a packet from the video stream?
            if (packet.stream_index == videoStream) {

// Decode video frame 从packet中获取数据补充到pFrame上
                avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

// 并不是decode一次就可解码出一帧
                if (frameFinished) {

// lock native window buffer
                    ANativeWindow_lock(nativeWindow, &windowBuffer, 0);

// 格式转换（把解码的数据格式转化，赋值给pFrameRGBA用于渲染）
                    sws_scale(sws_ctx, (uint8_t const *const *) pFrame->data,
                              pFrame->linesize, 0, pCodecCtx->height,
                              pFrameRGBA->data, pFrameRGBA->linesize);

//渲染的容器
                    uint8_t *dst = (uint8_t *) windowBuffer.bits;
                    int dstStride = windowBuffer.stride * 4;
//要渲染的数据
                    uint8_t *src = (pFrameRGBA->data[0]);//指针数组中的第一位存放了这是数据的地址
                    int srcStride = pFrameRGBA->linesize[0];

// 由于window的stride和帧的stride不同,因此需要逐行复制
                    int h;
                    for (h = 0; h < videoHeight; h++) {
                        memcpy(dst + h * dstStride, src + h * srcStride, srcStride);
                    }

//交换buffer，进行渲染在屏幕上
                    ANativeWindow_unlockAndPost(nativeWindow);
                }

            }
            av_packet_unref(&packet);
        }

//buffer是pFrameRGBA结构体中存放真实数据的空间
        av_free(buffer);
        av_free(pFrameRGBA);

// Free the YUV frame
        av_free(pFrame);

// Close the codecs
        avcodec_close(pCodecCtx);

// Close the video file
        avformat_close_input(&pFormatCtx);
        return 0;

    }

}