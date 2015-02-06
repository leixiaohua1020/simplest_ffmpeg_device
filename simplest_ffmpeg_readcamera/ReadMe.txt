 最简单的基于FFmpeg的AVDevice例子（读取摄像头）
 Simplest FFmpeg Device (Read Camera)

 雷霄骅 Lei Xiaohua
 leixiaohua1020@126.com
 中国传媒大学/数字电视技术
 Communication University of China / Digital TV Technology
 http://blog.csdn.net/leixiaohua1020

 本程序实现了本地摄像头数据的获取解码和显示。是基于FFmpeg
 的libavdevice类库最简单的例子。通过该例子，可以学习FFmpeg中
 libavdevice类库的使用方法。
 本程序在Windows下可以使用2种方式读取摄像头数据：
  1.VFW: Video for Windows 屏幕捕捉设备。注意输入URL是设备的序号，
          从0至9。
  2.dshow: 使用Directshow。注意作者机器上的摄像头设备名称是
         “Integrated Camera”，使用的时候需要改成自己电脑上摄像头设
          备的名称。
 在Linux下则可以使用video4linux2读取摄像头设备。

 This software read data from Computer's Camera and play it.
 It's the simplest example about usage of FFmpeg's libavdevice Library. 
 It's suiltable for the beginner of FFmpeg.
 This software support 2 methods to read camera in Microsoft Windows:
  1.gdigrab: VfW (Video for Windows) capture input device.
             The filename passed as input is the capture driver number,
             ranging from 0 to 9.
  2.dshow: Use Directshow. Camera's name in author's computer is 
             "Integrated Camera".
 It use video4linux2 to read Camera in Linux.