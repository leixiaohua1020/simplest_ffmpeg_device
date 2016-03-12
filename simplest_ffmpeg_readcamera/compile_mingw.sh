#! /bin/sh
g++ simplest_ffmpeg_readcamera.cpp -g -o simplest_ffmpeg_readcamera.exe \
-I /usr/local/include -L /usr/local/lib \
-lmingw32 -lSDLmain -lSDL -lavformat -lavcodec -lavutil -lavdevice -lswscale
