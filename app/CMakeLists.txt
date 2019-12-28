cmake_minimum_required(VERSION 3.4.1)

#添加头文件路径（相对于本文件路径）
include_directories(include)

#设置ffmpeg库所在路径的变量
set(FF ${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI})
add_library(avcodec SHARED IMPORTED)
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION ${FF}/libavcodec.so)

add_library(avformat SHARED IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION ${FF}/libavformat.so)

add_library(avutil SHARED IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION ${FF}/libavutil.so)

add_library(swscale SHARED IMPORTED)
set_target_properties(swscale PROPERTIES IMPORTED_LOCATION ${FF}/libswscale.so)

add_library(swresample SHARED IMPORTED)
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION ${FF}/libswresample.so)

add_library(avfilter SHARED IMPORTED)
set_target_properties(avfilter PROPERTIES IMPORTED_LOCATION ${FF}/libavfilter.so)

add_library(native-lib
            SHARED
            src/main/cpp/native-lib.cpp
            src/main/cpp/demux/IDemux.cpp
            src/main/cpp/demux/FFDemux.cpp
            src/main/cpp/decode/IDecode.cpp
            src/main/cpp/decode/FFDecode.cpp
            src/main/cpp/resample/IResample.cpp
            src/main/cpp/resample/FFResample.cpp
            src/main/cpp/audio/IAudioPlay.cpp
            src/main/cpp/audio/SLAudioPlay.cpp
            src/main/cpp/video/GLVideoView.cpp
            src/main/cpp/video/IVideoView.cpp
            src/main/cpp/opengl/XEGL.cpp
            src/main/cpp/opengl/XShader.cpp
            src/main/cpp/opengl/XTexture.cpp
            src/main/cpp/builder/IPlayerBuilder.cpp
            src/main/cpp/builder/FFPlayerBuilder.cpp
            src/main/cpp/thread/XThread.cpp
            src/main/cpp/thread/IObserver.cpp
            src/main/cpp/XLog.cpp
            src/main/cpp/XData.cpp
            src/main/cpp/XData.cpp
            src/main/cpp/Xlog.cpp
            src/main/cpp/XParameter.cpp
            src/main/cpp/IPlayer.cpp
            src/main/cpp/IPlayerProxy.cpp
            )

find_library(log-lib
              log )
target_link_libraries(native-lib
                      OpenSLES EGL
                      GLESv2
                      OpenSLES
                      android
                      avcodec avformat avutil swscale swresample avfilter
                       ${log-lib} )