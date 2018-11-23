package com.ws.ffmpegandroidplayer.my;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

import com.ws.ffmpegandroidplayer.R;

/**
 * 播放界面
 */
public class PlayVideoActivity extends AppCompatActivity implements View.OnClickListener {

    private CustomSurfaceView customSurfaceView;
    private View play;
    private View play_audio;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_play);
        customSurfaceView = (CustomSurfaceView) findViewById(R.id.surface_view);
        play = findViewById(R.id.play);
        play_audio = findViewById(R.id.play_audio);
        play.setOnClickListener(this);
        play_audio.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.play:
                customSurfaceView.play();
                break;
            case R.id.play_audio:
                new Thread(){
                    @Override
                    public void run() {
                        Render.renderAudio();
                    }
                }.start();
                break;
        }

    }
}
