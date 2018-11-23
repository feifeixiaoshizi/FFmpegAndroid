package com.ws.ffmpegandroidplayer;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

import com.ws.ffmpegandroidplayer.my.PlayVideoActivity;
import com.ws.ffmpegandroidplayer.test.RecordActivity;
import com.ws.ffmpegandroidplayer.test.TestRecordActivity;


public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback {

    static {
        System.loadLibrary("native-lib");
    }

    SurfaceHolder surfaceHolder;
    private View goto_play;
    private View goto_play1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        SurfaceView surfaceView = (SurfaceView) findViewById(R.id.surface_view);
        surfaceHolder = surfaceView.getHolder();
        surfaceHolder.addCallback(this);
        goto_play = findViewById(R.id.goto_play);
        goto_play1 = findViewById(R.id.goto_play_me);
        goto_play1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this,PlayVideoActivity.class);
                startActivity(intent);
            }
        });
        goto_play.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this,TestRecordActivity.class);
                startActivity(intent);
            }
        });
    }


    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                //surfaceHolder.getSurface().lockCanvas()
                play(surfaceHolder.getSurface());
            }
        }).start();
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    public native int play(Object surface);
}

