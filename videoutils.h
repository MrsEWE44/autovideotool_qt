#ifndef VIDEOUTILS_H
#define VIDEOUTILS_H

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavdevice/avdevice.h>
    #include <libavformat/avformat.h>
    #include <libavformat/avio.h>
    #include <libavutil/avutil.h>
    #include <libavfilter/buffersink.h>
    #include <libavfilter/buffersrc.h>
    #include <libavutil/channel_layout.h>
    #include <libavutil/opt.h>
    #include <libavutil/pixdesc.h>
}
#include "coreutils.h"


class VideoUtils : public CoreUtils
{
public:
    VideoUtils();
public:
    //定义一个结构体，用来存放视频信息
    typedef struct{
        long videoTimeSeconds; //视频的时间长度，单位是秒
        int width; //视频的宽度,单位是像素
        int height; //视频的高度，单位是像素
    } vinfo;

public:
    //检查ffmpeg是否存在
    int check(QString videoOutPath);
    //返回ffmpeg的绝对位置
    QString getFFMPEGPath();

    /*
     * 裁剪视频的功能
       fork:
       https://stackoverflow.com/questions/75116994/how-to-cut-videos-audios-using-ffmpeg-c-apis?newreg=966befcf7d8e4bf98b8a1056b73ecf9f

       @filePath: input video full path
       @outFilePath : split video out path
       @startTimeSeconds : input split start time , like 10s
       @endTimeSeconds : input split end time , like 10s

       how to use this function ?
       create var in :
       QString filePath = "e:\\a.mp4";
       QString outFilePath = "e:\\a_split.mp4"
       int startTimeSeconds = 10;
       int endTimeSeconds = 20;
       use the function :
       cutVideo(filePath , outFilePath , startTimeSeconds,endTimeSeconds);

       it's will work ...
       split video time in 10s ~ 20s, total time is 20s - 10s = 10s , so split video total time has 10s.


       */
    int cutVideo(QString videoFilePath , QString videoOutPath , int startTimeSeconds,int endTimeSeconds);
    int cutVideoByMP4(QString videoFilePath , QString videoOutPath , int startTimeSeconds,int endTimeSeconds);
    /*
     * 获取视频信息。
           @videoFilePath : input video file full path.

           how to use this function ?
           create var :
           QString videoFilePath = "e:\\a.mp4";
           long time = 0l;
           use this function :
           time = getVideoTimeSeconds(videoFilePath);

           it's will get video total time in var {time}.
       */
    vinfo getVideoInfos(QString videoFilePath);

    /*
     * 获取视频可以被切割的数量。
            @videoTotalTimeSeconds : input video total time seconds.
            @startTimeSeconds : input split start time , like 10s
            @endTimeSeconds : input split end time , like 10s
            how to use this function ?
            create var :
            QString videoFilePath = "e:\\a.mp4";
            long videoTotalTimeSeconds = getVideoTimeSeconds(videoFilePath);
            int startTimeSeconds = 10;
            int endTimeSeconds = 20;
            int sum = -1;
            use this function :
            sum = getCutVideoSum(videoTotalTimeSeconds,startTimeSeconds,endTimeSeconds);

            it's will get video cut sum.
        */
    int getCutVideoSum(int videoTotalTimeSeconds , int startTimeSeconds,int endTimeSeconds);

    /*
     * 提取音视频文件。
        fork:https://blog.csdn.net/yinshipin007/article/details/131095064

        void splitVideoAudio(QString filePath , QString outFilePath);
        void splitVideo(QString filePath , QString outFilePath);
        void dumpVideo(QString filePath , QString outFilePath,int audio);

        @filePath  : input video full path.
        @outFilePath : output file full path.
        @audio : if the var is 0 , and dump video audio ... if is 1 , and dump video ...

        how to use this function ?
        create var :
        QString filePath = "e:\\a.mp4";
        QString outFilePath1 = "e:\\a.aac";
        QString outFilePath2 =  "e:\\aa.mp4";

        if you want split video audio , use this :
        splitVideoAudio(filePath , outFilePath1);

        it's will start split video audio in outFilePath1 ...

        if you want split video , use this :
        splitVideo(filePath , outFilePath2);

        it's will start remove audio in video file and output file to outFilePath2...
     */
    void splitVideoAudio(QString filePath , QString outFilePath);
    void splitVideo(QString filePath , QString outFilePath);
    void dumpVideo(QString filePath , QString outFilePath,int audio);
    void splitVideoAudioMP4(QString filePath , QString outFilePath);
    void splitVideoMP4(QString filePath , QString outFilePath);
    void dumpVideoMP4(QString filePath , QString outFilePath,int audio);

    /*
     * 音视频转码。
    @filepath : input video full path.
    @outfilepath : video out full path.
    how to use this function?
    create var :
    QString filePath = "e:\\a.mkv";
    QString outFilePath = "e:\\a2.mp4";
    transcodeVideo(filePath,outFilePath);

    it's will start transcode video in mkv to mp4 and save out file on  e:\\a2.mp4...
    */
    void transcodeVideo(QString filePath , QString outFilePath);

    /*
     * 往视频里面插入封面图片。
    @picPath ： video pic image full path.
    @videoInputPath : input video full path.
    @videoOutPath : video out full path.
    how to use this function?
    create var :
    QString picPath = "e:\\gggg.png";
    QString videoInputPath = "e:\\a.mkv";
    QString videoOutPath = "e:\\a2.mp4";
    insertPicToVideo(picPath,videoInputPath,videoOutPath);

    it's will start insert pic image file in video and transcode video  mkv to mp4 and save out file on  e:\\a2.mp4...
    */
    void insertPicToVideo(QString picPath , QString videoInputPath , QString videoOutPath);

};

#endif // VIDEOUTILS_H
