#include "videoutils.h"

VideoUtils::VideoUtils()
{

}

int VideoUtils::check(QString videoOutPath)
{
    QFile f(getFFMPEGPath());
    if(f.exists()){
        QFileInfo fi(videoOutPath);
        QFile f2(videoOutPath);
        QDir d = fi.dir();
        if(!d.exists()){
            d.mkdir(d.path());
        }
        if(f2.exists()){
            f2.remove();
        }
        return 0;
    }
    return -1;
}

QString VideoUtils::getFFMPEGPath()
{
    QString ffmpeg_path = getCurrentPath()+"/ffmpeg.exe";
#if defined (Q_OS_WIN)
    ffmpeg_path = getCurrentPath()+"/ffmpeg.exe";
#else
    ffmpeg_path = "/usr/bin/ffmpeg";
#endif
    return  ffmpeg_path;
}

int VideoUtils::cutVideo(QString videoFilePath, QString videoOutPath, int startTimeSeconds,int endTimeSeconds)
{
    if(check(videoOutPath) == 0){
        QString split_video_cmdstr = getFFMPEGPath()+" -i \""+videoFilePath+"\" -ss "+FormatHHMMSS(startTimeSeconds)+" -t "+QString::number(endTimeSeconds-startTimeSeconds)+" -c copy \""+videoOutPath+"\"";
        QString r = runCMD(split_video_cmdstr);
    }
}

int VideoUtils::cutVideoByMP4(QString filePath, QString outFilePath, int startTimeSeconds, int endTimeSeconds)
{
    int operationResult;

    AVPacket* avPacket = NULL;
    AVFormatContext* avInputFormatContext = NULL;
    AVFormatContext* avOutputFormatContext = NULL;

    avPacket = av_packet_alloc();
    if (!avPacket) {
        qCritical("Failed to allocate AVPacket.");
    }

    try {
        operationResult = avformat_open_input(&avInputFormatContext, filePath.toStdString().c_str(), 0, 0);
        if (operationResult < 0) {
            qDebug() << QString("Failed to open the input file '%1'.").arg(filePath);
        }

        operationResult = avformat_find_stream_info(avInputFormatContext, 0);
        if (operationResult < 0) {
            qDebug() << QString("Failed to retrieve the input stream information.");
        }

        avformat_alloc_output_context2(&avOutputFormatContext, NULL, NULL, outFilePath.toStdString().c_str());
        if (!avOutputFormatContext) {
            operationResult = AVERROR_UNKNOWN;
            qDebug() << QString("Failed to create the output context.");
        }

        int streamIndex = 0;
        int nbs = avInputFormatContext->nb_streams;
        int streamMapping[nbs];
        int streamRescaledstartTimeSeconds[nbs];
        int streamRescaledendTimeSeconds[nbs];

        // Copy streams from the input file to the output file.
        for (int i = 0; i < nbs; i++) {
            AVStream* outStream;
            AVStream* inStream = avInputFormatContext->streams[i];
            streamRescaledstartTimeSeconds[i] = av_rescale_q(startTimeSeconds * AV_TIME_BASE, AV_TIME_BASE_Q, inStream->time_base);
            streamRescaledendTimeSeconds[i] = av_rescale_q(endTimeSeconds * AV_TIME_BASE, AV_TIME_BASE_Q, inStream->time_base);

            if (inStream->codecpar->codec_type != AVMEDIA_TYPE_AUDIO &&
                    inStream->codecpar->codec_type != AVMEDIA_TYPE_VIDEO &&
                    inStream->codecpar->codec_type != AVMEDIA_TYPE_SUBTITLE) {
                streamMapping[i] = -1;
                continue;
            }

            streamMapping[i] = streamIndex++;
            outStream = avformat_new_stream(avOutputFormatContext, NULL);
            if (!outStream) {
                operationResult = AVERROR_UNKNOWN;
                qDebug() << QString("Failed to allocate the output stream.");
            }

            operationResult = avcodec_parameters_copy(outStream->codecpar, inStream->codecpar);
            if (operationResult < 0) {
                qDebug() << QString("Failed to copy codec parameters from input stream to output stream.");
            }
            outStream->codecpar->codec_tag = 0;
        }

        if (!(avOutputFormatContext->oformat->flags & AVFMT_NOFILE)) {
            operationResult = avio_open(&avOutputFormatContext->pb, outFilePath.toStdString().c_str(), AVIO_FLAG_WRITE);
            if (operationResult < 0) {
                qDebug() << QString("Failed to open the output file '%1'.").arg(outFilePath);
            }
        }

        operationResult = avformat_write_header(avOutputFormatContext, NULL);
        if (operationResult < 0) {
            qDebug() << QString("Error occurred when opening output file.");
        }

        operationResult = avformat_seek_file(avInputFormatContext, -1, INT64_MIN, startTimeSeconds * AV_TIME_BASE,
                                             startTimeSeconds * AV_TIME_BASE, 0);
        if (operationResult < 0) {
            qDebug() << QString("Failed to seek the input file to the targeted start position.");
        }

        while (av_read_frame(avInputFormatContext, avPacket) >= 0) {

            // Skip packets from unknown streams and packets after the end cut position.
            if (avPacket->stream_index >= nbs || streamMapping[avPacket->stream_index] < 0 ||
                    avPacket->pts > streamRescaledendTimeSeconds[avPacket->stream_index]) {
                av_packet_unref(avPacket);
                continue;
            }

            avPacket->stream_index = streamMapping[avPacket->stream_index];

            // Shift the packet to its new position by subtracting the rescaled start seconds.
            avPacket->pts -= streamRescaledstartTimeSeconds[avPacket->stream_index];
            avPacket->dts -= streamRescaledstartTimeSeconds[avPacket->stream_index];

            av_packet_rescale_ts(avPacket, avInputFormatContext->streams[avPacket->stream_index]->time_base,
                    avOutputFormatContext->streams[avPacket->stream_index]->time_base);
            avPacket->pos = -1;
            operationResult = av_interleaved_write_frame(avOutputFormatContext, avPacket);
            if (operationResult < 0) {
                qDebug() << QString("Failed to mux the packet.");
            }
        }

        av_write_trailer(avOutputFormatContext);
    } catch (std::runtime_error e) {
        qCritical("%s", e.what());
    }
    av_packet_free(&avPacket);
    avformat_close_input(&avInputFormatContext);
    if (avOutputFormatContext && !(avOutputFormatContext->oformat->flags & AVFMT_NOFILE))
        avio_closep(&avOutputFormatContext->pb);
    avformat_free_context(avOutputFormatContext);

    if (operationResult < 0 && operationResult != AVERROR_EOF) {
        qDebug() << "Error occurred: " << operationResult;
    }
}

VideoUtils::vinfo VideoUtils::getVideoInfos(QString videoFilePath)
{
    vinfo v;

    int operationResult;
    AVFormatContext* avInputFormatContext = NULL;

    try {
        operationResult = avformat_open_input(&avInputFormatContext, videoFilePath.toStdString().c_str(), 0, 0);
        if (operationResult < 0) {
            qDebug() << QString("Failed to open the input file '%1'.").arg(videoFilePath);
        }

        operationResult = avformat_find_stream_info(avInputFormatContext, 0);
        if (operationResult < 0) {
            qDebug() << QString("Failed to retrieve the input stream information.");
        }

        // Copy streams from the input file to the output file.
        for (int i = 0; i < avInputFormatContext->nb_streams; i++) {
            AVStream *s = avInputFormatContext->streams[i];
            if(s->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
                // 获取视频时长
                int64_t duration = avInputFormatContext->duration;
                v.videoTimeSeconds = duration / 1000.0 / 1000.0;
                v.width = s->codecpar->width;
                v.height = s->codecpar->height;
                break;
            }

        }
    } catch (std::runtime_error e) {
        qCritical("%s", e.what());
    }
    avformat_close_input(&avInputFormatContext);
    return v;
}


int VideoUtils::getCutVideoSum(int videoTotalTimeSeconds, int startTimeSeconds, int endTimeSeconds)
{
    int tempTime = endTimeSeconds - startTimeSeconds;
    if(startTimeSeconds>0){
        videoTotalTimeSeconds = videoTotalTimeSeconds-startTimeSeconds;
    }
    if(tempTime > 0){
        int time = videoTotalTimeSeconds/tempTime;
        if((videoTotalTimeSeconds%tempTime) == 0){
            return time;
        }else{
            return time+1;
        }
    }
    return  -1;
}

void VideoUtils::splitVideoAudio(QString filePath, QString outFilePath)
{
    dumpVideo(filePath,outFilePath,0);
}

void VideoUtils::splitVideo(QString filePath, QString outFilePath)
{
    dumpVideo(filePath,outFilePath,-1);
}

void VideoUtils::dumpVideo(QString filePath, QString outFilePath, int audio)
{
    if(check(outFilePath) == 0){
        QString dump_cmd_str = getFFMPEGPath()+" -i \""+filePath+"\" -vn \""+outFilePath+"\"";
        if(audio != 0){
            dump_cmd_str = getFFMPEGPath()+" -i \""+filePath+"\" -an \""+outFilePath+"\"";
        }
        QString r = runCMD(dump_cmd_str);
    }

}

void VideoUtils::splitVideoAudioMP4(QString filePath, QString outFilePath)
{
    dumpVideoMP4(filePath,outFilePath,0);
}

void VideoUtils::splitVideoMP4(QString filePath, QString outFilePath)
{
    dumpVideoMP4(filePath,outFilePath,1);
}

void VideoUtils::dumpVideoMP4(QString filePath, QString outFilePath, int audio)
{
    int err_code;
    int audio_stream_index;
    //上下文
    AVFormatContext *fmt_ctx = NULL;
    AVFormatContext *ofmt_ctx = NULL;
    AVStream *in_stream = NULL;
    AVStream *out_stream = NULL;

    AVPacket pkt;

    if(avformat_open_input(&fmt_ctx, filePath.toStdString().c_str(), NULL, NULL) < 0){
        qDebug() << "open video file error .. " << filePath;
    }

    if(avformat_find_stream_info(fmt_ctx, NULL) < 0){
        qDebug() << "failed to find stream info ..." << filePath;
    }


    if (fmt_ctx->nb_streams < 2) {
        //流数小于2，说明这个文件音频、视频流这两条都不能保证，输入文件有错误
        qDebug() << "input file error . stream not 2... " << filePath;
    }

    //拿到文件中音频流
    /**只需要修改这里AVMEDIA_TYPE_VIDEO参数**/
    audio_stream_index = av_find_best_stream(fmt_ctx, audio==0? AVMEDIA_TYPE_AUDIO:AVMEDIA_TYPE_VIDEO /*AVMEDIA_TYPE_VIDEO*/, -1, -1, NULL, 0);
    if (audio_stream_index < 0) {
        qDebug() << "get audio stream error ... " << filePath;
    }

    in_stream = fmt_ctx->streams[audio_stream_index];
    //参数信息
    AVCodecParameters *in_codecpar = in_stream->codecpar;


    // 输出上下文
    ofmt_ctx = avformat_alloc_context();

    //根据目标文件名生成最适合的输出容器
    //支持各种各样的输出文件格式，MP4，FLV，3GP等等
    const AVOutputFormat *output_fmt = av_guess_format(NULL, outFilePath.toStdString().c_str(), NULL);
    if (!output_fmt) {
        qDebug() << "av_guess_format error .. " << outFilePath;
    }

    ofmt_ctx->oformat = output_fmt;

    //新建输出流
    out_stream = avformat_new_stream(ofmt_ctx, NULL);
    if (!out_stream) {
        av_log(NULL, AV_LOG_DEBUG, "创建输出流失败！\n");
        exit(1);
    }

    // 将参数信息拷贝到输出流中，我们只是抽取音频流，并不做音频处理，所以这里只是Copy
    if ((err_code = avcodec_parameters_copy(out_stream->codecpar, in_codecpar)) < 0) {
        qDebug() << "copy avcodec error ... " ;
    }

    out_stream->codecpar->codec_tag = 0;

    //初始化AVIOContext,文件操作由它完成
    if ((err_code = avio_open(&ofmt_ctx->pb, outFilePath.toStdString().c_str(), AVIO_FLAG_WRITE)) < 0) {
        qDebug() << "out file open error ... " << outFilePath;
    }

    //初始化 AVPacket， 我们从文件中读出的数据会暂存在其中
    av_init_packet(&pkt);
    pkt.data = NULL;
    pkt.size = 0;


    // 写头部信息
    if (avformat_write_header(ofmt_ctx, NULL) < 0) {
        qDebug() << "write header error ...";
    }

    //每读出一帧数据
    while (av_read_frame(fmt_ctx, &pkt) >= 0) {
        if (pkt.stream_index == audio_stream_index) {
            pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base,AV_ROUND_NEAR_INF);
            pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base,AV_ROUND_NEAR_INF);
            pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
            pkt.stream_index = 0;
            pkt.pos = -1;
            //将包写到输出媒体文件
            av_interleaved_write_frame(ofmt_ctx, &pkt);
            //减少引用计数，避免内存泄漏
            av_packet_unref(&pkt);
        }
    }

    //写尾部信息
    av_write_trailer(ofmt_ctx);

    //最后别忘了释放内存
    avformat_close_input(&fmt_ctx);
    avio_close(ofmt_ctx->pb);
}

void VideoUtils::transcodeVideo(QString filePath, QString outFilePath)
{
    if(check(outFilePath) == 0){
        QString transcode_cmd_str = getFFMPEGPath()+" -i \""+filePath+"\" \""+outFilePath+"\"";
        QString r = runCMD(transcode_cmd_str);
    }
}

void VideoUtils::insertPicToVideo(QString picPath, QString videoInputPath, QString videoOutPath)
{
    QFile f(picPath);
    if(check(videoOutPath) == 0 && f.exists()){
        QString transcode_cmd_str = getFFMPEGPath()+" -i \""+videoInputPath+"\" -i "+picPath+" -map 1 -map 0 -c copy -disposition:0 attached_pic \""+videoOutPath+"\"";
        QString r = runCMD(transcode_cmd_str);
    }

}
