package com.ws.ffmpegandroidaddmusic;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {


    static {
        System.loadLibrary("native-lib");
    }

    private TextView tv_info;

    private EditText editText1,editText2;
    private Button btnDecoder;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        init();
    }

    private void init() {

        tv_info= (TextView) findViewById(R.id.tv_info);

        editText1 = (EditText) findViewById(R.id.editText1);
        editText2 = (EditText) findViewById(R.id.editText2);
        btnDecoder= (Button) findViewById(R.id.button);

        btnDecoder.setOnClickListener(this);
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {

            case R.id.button:
                startDecode();
                break;

            default:
                break;
        }
    }

    private void startDecode() {

        String folderurl= Environment.getExternalStorageDirectory().getPath()+"/51vv/mvbox";
        //输入一个视频文件地址
        String videoUrl=folderurl+"/"+editText1.getText().toString();
        //输入一个音频文件地址，作为视频文件的背景音乐
        String musicUrl=folderurl+"/"+editText2.getText().toString();

        Log.e("ws-----------videoUrl",videoUrl);
        Log.e("ws------------musicUrl",musicUrl);

        //给视频添加指定的背景音乐
        addBgMusci(videoUrl,musicUrl);

    }

    public native int addBgMusci(String videoUrl, String musicUrl);

}


