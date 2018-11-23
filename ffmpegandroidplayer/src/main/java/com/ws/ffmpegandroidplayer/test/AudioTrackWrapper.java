package com.ws.ffmpegandroidplayer.test;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.util.Log;


/**
 * Created by ThinkPad on 2018/11/12.
 */

public class AudioTrackWrapper {
    private int mFrequence = 44100;
    private int mChannelConfig = AudioFormat.CHANNEL_IN_MONO;
    private int mPlayChannelConfig = AudioFormat.CHANNEL_IN_STEREO;
    private int mAudioEncoding = AudioFormat.ENCODING_PCM_16BIT;

    public AudioTrackWrapper() {
        Log.d("AudioTrackWrapper","AudioTrackWrapper对象构建了！");
    }

    public AudioTrack createAudioTrack(int frequence, int channels){
        Log.d("AudioTrackWrapper","createAudioTrack！");
        int bufferSize = AudioTrack.getMinBufferSize(mFrequence,mPlayChannelConfig,mAudioEncoding);
        AudioTrack track = new AudioTrack(AudioManager.STREAM_MUSIC,
                mFrequence,
                mPlayChannelConfig, mAudioEncoding, bufferSize,
                AudioTrack.MODE_STREAM);
        return  track;
    }

}
