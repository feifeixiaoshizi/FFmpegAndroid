# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

#添加共享类库
add_library(
             native-lib
             SHARED
             src/main/cpp/native-lib.cpp )

#预构建的 NDK 库已经存在于 Android 平台上，因此，您无需再构建或将其打包到 APK 中。
#由于 NDK 库已经是 CMake 搜索路径的一部分，您甚至不需要在您的本地 NDK 安装中指定库的位置 - 只需要向 CMake
#提供您希望使用的库的名称，并将其关联到您自己的原生库。
#将 find_library() 命令添加到您的 CMake 构建脚本中以定位 NDK 库，并将其路径存储为一个变量
find_library(
              log-lib
              log )


set(distribution_DIR ${CMAKE_SOURCE_DIR}/../../../../libs)
#添加三方so库
add_library( avutil-55
             SHARED
             IMPORTED )
#D:\demo\FFmpegAndroid\app\libs
#D:\demo\FFmpegAndroid\app\CMakeLists.txt
#设置三方so库的位置
set_target_properties( avutil-55
                       PROPERTIES IMPORTED_LOCATION
                       ../../../../libs/armeabi-v7a/libavutil-55.so )

add_library( swresample-2
             SHARED
             IMPORTED )
set_target_properties( swresample-2
                       PROPERTIES IMPORTED_LOCATION
                       ../../../../libs/armeabi-v7a/libswresample-2.so )
add_library( avcodec-57
             SHARED
             IMPORTED )
set_target_properties( avcodec-57
                       PROPERTIES IMPORTED_LOCATION
                       ../../../../libs/armeabi-v7a/libavcodec-57.so )
add_library( avfilter-6
             SHARED
             IMPORTED)
set_target_properties( avfilter-6
                       PROPERTIES IMPORTED_LOCATION
                       ../../../../libs/armeabi-v7a/libavfilter-6.so )
add_library( swscale-4
             SHARED
             IMPORTED)
set_target_properties( swscale-4
                       PROPERTIES IMPORTED_LOCATION
                       ../../../../libs/armeabi-v7a/libswscale-4.so )
add_library( avdevice-57
             SHARED
             IMPORTED)
set_target_properties( avdevice-57
                       PROPERTIES IMPORTED_LOCATION
                       ../../../../libs/armeabi-v7a/libavdevice-57.so )
add_library( avformat-57
             SHARED
             IMPORTED)
set_target_properties( avformat-57
                       PROPERTIES IMPORTED_LOCATION
                       ../../../../libs/armeabi-v7a/libavformat-57.so )
#编译时支持C++(也可以在build.gradle文件中配置)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")


#引入头文件目录
include_directories(libs/include)

#target_include_directories(native-lib PRIVATE libs/include)
#链接各个so库
target_link_libraries( native-lib swresample-2 avcodec-57 avfilter-6 swscale-4 avdevice-57 avformat-57
                       ${log-lib} )
