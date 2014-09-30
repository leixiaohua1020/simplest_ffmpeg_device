最简单的基于FFmpeg的AVDevice例子（屏幕录制）
Simplest FFmpeg Device (Screen Capture)

雷霄骅 Lei Xiaohua
leixiaohua1020@126.com
中国传媒大学/数字电视技术
Communication University of China / Digital TV Technology
http://blog.csdn.net/leixiaohua1020

本程序实现了屏幕录制功能。可以录制并播放桌面数据。是基于FFmpeg
的libavdevice类库最简单的例子。通过该例子，可以学习FFmpeg中
libavdevice类库的使用方法。
本程序在Windows下可以使用2种方式录制屏幕：
 1.gdigrab: Win32下的基于GDI的屏幕录制设备。
            抓取桌面的时候，输入URL为“desktop”。
 2.dshow: 使用Directshow。注意需要安装额外的软件screen-capture-recorder
在Linux下则可以使用x11grab录制屏幕。

This software capture screen of computer. It's the simplest example
about usage of FFmpeg's libavdevice Library. 
It's suiltable for the beginner of FFmpeg.
This software support 2 methods to capture screen in Microsoft Windows:
 1.gdigrab: Win32 GDI-based screen capture device.
            Input URL in avformat_open_input() is "desktop".
 2.dshow: Use Directshow. Need to install screen-capture-recorder.
It use x11grab to capture screen in Linux.
