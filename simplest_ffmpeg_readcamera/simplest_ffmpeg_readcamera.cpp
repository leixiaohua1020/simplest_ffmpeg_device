/**
 * 最简单的基于FFmpeg的AVDevice例子（读取摄像头）
 * Simplest FFmpeg Device (Read Camera)
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序实现了本地摄像头数据的获取解码和显示。是基于FFmpeg
 * 的libavdevice类库最简单的例子。通过该例子，可以学习FFmpeg中
 * libavdevice类库的使用方法。
 * 本程序在Windows下可以使用2种方式读取摄像头数据：
 *  1.VFW: Video for Windows 屏幕捕捉设备。注意输入URL是设备的序号，
 *          从0至9。
 *  2.dshow: 使用Directshow。注意作者机器上的摄像头设备名称是
 *         “Integrated Camera”，使用的时候需要改成自己电脑上摄像头设
 *          备的名称。
 * 在Linux下则可以使用video4linux2读取摄像头设备。
 *
 * This software read data from Computer's Camera and play it.
 * It's the simplest example about usage of FFmpeg's libavdevice Library. 
 * It's suiltable for the beginner of FFmpeg.
 * This software support 2 methods to read camera in Microsoft Windows:
 *  1.gdigrab: VfW (Video for Windows) capture input device.
 *             The filename passed as input is the capture driver number,
 *             ranging from 0 to 9.
 *  2.dshow: Use Directshow. Camera's name in author's computer is 
 *             "Integrated Camera".
 * It use video4linux2 to read Camera in Linux.
 * 
 */


#include "stdafx.h"

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"
	//SDL
#include "sdl/SDL.h"
#include "sdl/SDL_thread.h"
};

//Output YUV420P 
#define OUTPUT_YUV420P 0
//'1' Use Dshow 
//'0' Use VFW
#define USE_DSHOW 0

//Show Device
void show_dshow_device(){
	AVFormatContext *pFormatCtx = avformat_alloc_context();
	AVDictionary* options = NULL;
	av_dict_set(&options,"list_devices","true",0);
	AVInputFormat *iformat = av_find_input_format("dshow");
	printf("Device Info=============\n");
	avformat_open_input(&pFormatCtx,"video=dummy",iformat,&options);
	printf("========================\n");
}

//Show Device Option
void show_dshow_device_option(){
	AVFormatContext *pFormatCtx = avformat_alloc_context();
	AVDictionary* options = NULL;
	av_dict_set(&options,"list_options","true",0);
	AVInputFormat *iformat = av_find_input_format("dshow");
	printf("Device Option Info======\n");
	avformat_open_input(&pFormatCtx,"video=Integrated Camera",iformat,&options);
	printf("========================\n");
}

//Show VFW Device
void show_vfw_device(){
	AVFormatContext *pFormatCtx = avformat_alloc_context();
	AVInputFormat *iformat = av_find_input_format("vfwcap");
	printf("VFW Device Info======\n");
	avformat_open_input(&pFormatCtx,"list",iformat,NULL);
	printf("=====================\n");
}



int main(int argc, char* argv[])
{

	AVFormatContext	*pFormatCtx;
	int				i, videoindex;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;
	
	av_register_all();
	avformat_network_init();
	pFormatCtx = avformat_alloc_context();
	
	//Open File
	//char filepath[]="src01_480x272_22.h265";
	//avformat_open_input(&pFormatCtx,filepath,NULL,NULL)

	//Register Device
	avdevice_register_all();
	//Show Dshow Device
	show_dshow_device();
	//Show Device Options
	show_dshow_device_option();
	//Show VFW Options
	show_vfw_device();
//Windows
#ifdef _WIN32
#if USE_DSHOW
	AVInputFormat *ifmt=av_find_input_format("dshow");
	//Set own video device's name
	if(avformat_open_input(&pFormatCtx,"video=Integrated Camera",ifmt,NULL)!=0){
		printf("Couldn't open input stream.（无法打开输入流）\n");
		return -1;
	}
#else
	AVInputFormat *ifmt=av_find_input_format("vfwcap");
	if(avformat_open_input(&pFormatCtx,"0",ifmt,NULL)!=0){
		printf("Couldn't open input stream.（无法打开输入流）\n");
		return -1;
	}
#endif
#endif
//Linux
#ifdef linux
	AVInputFormat *ifmt=av_find_input_format("video4linux2");
	if(avformat_open_input(&pFormatCtx,"/dev/video0",ifmt,NULL)!=0){
		printf("Couldn't open input stream.（无法打开输入流）\n");
		return -1;
	}
#endif


	if(avformat_find_stream_info(pFormatCtx,NULL)<0)
	{
		printf("Couldn't find stream information.（无法获取流信息）\n");
		return -1;
	}
	videoindex=-1;
	for(i=0; i<pFormatCtx->nb_streams; i++) 
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
		{
			videoindex=i;
			break;
		}
	if(videoindex==-1)
	{
		printf("Couldn't find a video stream.（没有找到视频流）\n");
		return -1;
	}
	pCodecCtx=pFormatCtx->streams[videoindex]->codec;
	pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
	if(pCodec==NULL)
	{
		printf("Codec not found.（没有找到解码器）\n");
		return -1;
	}
	if(avcodec_open2(pCodecCtx, pCodec,NULL)<0)
	{
		printf("Could not open codec.（无法打开解码器）\n");
		return -1;
	}
	AVFrame	*pFrame,*pFrameYUV;
	pFrame=avcodec_alloc_frame();
	pFrameYUV=avcodec_alloc_frame();
	uint8_t *out_buffer=(uint8_t *)av_malloc(avpicture_get_size(PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
	avpicture_fill((AVPicture *)pFrameYUV, out_buffer, PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
	//SDL----------------------------
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {  
		printf( "Could not initialize SDL - %s\n", SDL_GetError()); 
		return -1;
	} 
	int screen_w=0,screen_h=0;
	SDL_Surface *screen; 
	screen_w = pCodecCtx->width;
	screen_h = pCodecCtx->height;
	screen = SDL_SetVideoMode(screen_w, screen_h, 0,0);

	if(!screen) {  
		printf("SDL: could not set video mode - exiting:%s\n",SDL_GetError());  
		return -1;
	}
	SDL_Overlay *bmp; 
	bmp = SDL_CreateYUVOverlay(pCodecCtx->width, pCodecCtx->height,SDL_YV12_OVERLAY, screen); 
	SDL_Rect rect;
	//SDL End------------------------
	int ret, got_picture;

	AVPacket *packet=(AVPacket *)av_malloc(sizeof(AVPacket));
	//Output Information-----------------------------
	printf("File Information（文件信息）---------------------\n");
	av_dump_format(pFormatCtx,0,NULL,0);
	printf("-------------------------------------------------\n");

#if OUTPUT_YUV420P 
    FILE *fp_yuv=fopen("output.yuv","wb+");  
#endif  

	struct SwsContext *img_convert_ctx;
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL); 
	//------------------------------
	while(av_read_frame(pFormatCtx, packet)>=0)
	{
		if(packet->stream_index==videoindex)
		{
			ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
			if(ret < 0)
			{
				printf("Decode Error.（解码错误）\n");
				return -1;
			}
			if(got_picture)
			{
				sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
				
#if OUTPUT_YUV420P
				int y_size=pCodecCtx->width*pCodecCtx->height;  
				fwrite(pFrameYUV->data[0],1,y_size,fp_yuv);    //Y 
				fwrite(pFrameYUV->data[1],1,y_size/4,fp_yuv);  //U
				fwrite(pFrameYUV->data[2],1,y_size/4,fp_yuv);  //V
#endif
				SDL_LockYUVOverlay(bmp);
				bmp->pixels[0]=pFrameYUV->data[0];
				bmp->pixels[2]=pFrameYUV->data[1];
				bmp->pixels[1]=pFrameYUV->data[2];     
				bmp->pitches[0]=pFrameYUV->linesize[0];
				bmp->pitches[2]=pFrameYUV->linesize[1];   
				bmp->pitches[1]=pFrameYUV->linesize[2];
				SDL_UnlockYUVOverlay(bmp); 
				rect.x = 0;    
				rect.y = 0;    
				rect.w = screen_w;    
				rect.h = screen_h;  
				SDL_DisplayYUVOverlay(bmp, &rect); 
				//Delay 40ms
				SDL_Delay(40);
			}
		}
		av_free_packet(packet);
	}
	sws_freeContext(img_convert_ctx);

#if OUTPUT_YUV420P 
    fclose(fp_yuv);
#endif 

	SDL_Quit();

	av_free(out_buffer);
	av_free(pFrameYUV);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);

	return 0;
}

