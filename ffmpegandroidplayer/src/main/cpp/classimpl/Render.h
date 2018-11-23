//
// Created by ThinkPad on 2018/11/7.
//

#include <jni.h>
#ifndef RENDER
#define RENDER
namespace renderspace {
    /*
     * 定义一个类纯c++代码负责渲染操作*/
    class Render {
    private:
    protected:
    public:
        Render();

        ~Render();

        /**
         *
         * @param env
         * @param type
         * @param surface
         * @return
         */
        static int render(JNIEnv *env, jclass type, jobject surface);
    };

}
#endif