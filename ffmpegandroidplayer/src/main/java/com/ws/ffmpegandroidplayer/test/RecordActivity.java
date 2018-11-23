package com.ws.ffmpegandroidplayer.test;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioRecord;
import android.media.AudioTrack;
import android.media.MediaRecorder;
import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.TextView;

import com.ws.ffmpegandroidplayer.R;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;

/**
 * 自己写的负责测试AudioRecord和AudioTrack录音和播放声音
 * 1.注意点写文件时要和读流时定义的数组类型一致。
 */
public class RecordActivity extends AppCompatActivity {
    File fpath ;
    private int mFrequence = 44100;
    private int mChannelConfig = AudioFormat.CHANNEL_IN_MONO;
    private int mPlayChannelConfig = AudioFormat.CHANNEL_IN_STEREO;
    private int mAudioEncoding = AudioFormat.ENCODING_PCM_16BIT;
    private volatile  boolean   isRecording;
    private TextView record;
    private View play;
    File mAudioFile = null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_record);
        record = (TextView) findViewById(R.id.record);
        play = findViewById(R.id.play);
        record.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(isRecording){
                    isRecording=false;
                    record.setText("开始录制");
                }else{
                    isRecording=true;
                    record.setText("停止录制");
                    record();
                }
            }
        });
        play.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                play();
            }
        });
        fpath = new File(Environment.getExternalStorageDirectory()
                .getAbsolutePath() + "/pauseRecordDemo");
        if (!fpath.exists()) {
            fpath.mkdirs();
        }

    }

    private  void record(){
        //开启现场恒录制音频
        new Thread(){
            @Override
            public void run() {
                try {
                    // 创建临时文件,注意这里的格式为.pcm
                    mAudioFile = File.createTempFile("recording", ".pcm", fpath);
                    DataOutputStream dataOutputStream = new DataOutputStream(new BufferedOutputStream
                            (new FileOutputStream(mAudioFile)));
                    int bufferSize = AudioRecord.getMinBufferSize(mFrequence,mChannelConfig,mAudioEncoding);
                    AudioRecord audioRecord = new AudioRecord(MediaRecorder.AudioSource.MIC,mFrequence,
                            mChannelConfig,mAudioEncoding,bufferSize);
                    short [] buffer = new short[bufferSize];
                    audioRecord.startRecording();
                    while (isRecording){
                        int bufferReadResult = audioRecord.read(buffer,0,buffer.length);
                        for(int i=0;i<bufferReadResult;i++){
                            //定义的是shor，就应该使用writeShort否则会出现出现录制声音播放出来不对。（**）
                            dataOutputStream.writeShort(buffer[i]);
                            //dataOutputStream.write(buffer[i]);(X)
                        }
                    }
                    audioRecord.stop();
                    dataOutputStream.close();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }.start();


    }


    private void play(){
        new Thread(){
            @Override
            public void run() {
                int bufferSize = AudioTrack.getMinBufferSize(mFrequence,mPlayChannelConfig,mAudioEncoding);
                short[] buffer = new short[bufferSize];
                try {
                    // 定义输入流，将音频写入到AudioTrack类中，实现播放
                    DataInputStream dis = new DataInputStream(
                            new BufferedInputStream(new FileInputStream(mAudioFile)));
                    // 实例AudioTrack
                    AudioTrack track = new AudioTrack(AudioManager.STREAM_MUSIC,
                            mFrequence,
                            mPlayChannelConfig, mAudioEncoding, bufferSize,
                            AudioTrack.MODE_STREAM);
                    // 开始播放
                    track.play();
                    // 由于AudioTrack播放的是流，所以，我们需要一边播放一边读取
                    while ( dis.available() > 0) {
                        int i = 0;
                        while (dis.available() > 0 && i < buffer.length) {
                            buffer[i] = dis.readShort();
                            i++;
                        }
                        // 然后将数据写入到AudioTrack中
                        track.write(buffer, 0, buffer.length);
                    }


                    // 播放结束
                    track.stop();
                    dis.close();

                }catch (Exception e){
                    e.printStackTrace();
                }
            }
        }.start();
    }

}
