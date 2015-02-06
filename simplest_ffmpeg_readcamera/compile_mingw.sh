#! /bin/sh
#最简单的基于FFmpeg的AVDevice例子（读取摄像头） ----MinGW命令行编译
#Simplest FFmpeg Device (Read Camera) ----Compile in MinGW 
#
#雷霄骅 Lei Xiaohua
#leixiaohua1020@126.com
#中国传媒大学/数字电视技术
#Communication University of China / Digital TV Technology
#http://blog.csdn.net/leixiaohua1020
#
#compile
g++ simplest_ffmpeg_readcamera.cpp -g -o simplest_ffmpeg_readcamera.exe \
-I /usr/local/include -L /usr/local/lib \
-lmingw32 -lSDLmain -lSDL -lavformat -lavcodec -lavutil -lavdevice -lswscale
