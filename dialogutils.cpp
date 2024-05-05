#include "dialogutils.h"

DialogUtils::DialogUtils(QObject *parent) : QObject(parent)
{

}

DialogUtils::DialogUtils(QWidget *ww)
{
    this->ww = ww;
}

void DialogUtils::Start()
{
    QString tmpstr;
    switch (mode) {
    case 0:
        tmpstr="裁剪";
        break;
    case 1:
        tmpstr="提取";
        break;
    case 2:
        tmpstr="转码";
        break;
    }
    for(int i =0;i<choiceFiles.size();i++){
        QFileInfo fi(choiceFiles[i]);
        nowText = QString("正在%1 %2 -- %3 ...").arg(tmpstr,QString::number(i),fi.fileName());
        NowIndex = i;
        emit currentIndex();
        if(fi.exists()){
            VideoUtils vu;
            if(mode == 0){
                VideoUtils::vinfo vi =  vu.getVideoInfos(fi.absoluteFilePath());
                int videoParts = vu.getCutVideoSum(vi.videoTimeSeconds,0,cutTime);
                int starttime = 0;
                int endtime = cutTime;
                for(int j =0;j<videoParts;j++){
                    outFile = QString("%1/%2_%3_part%4.%5").arg( outPath,fi.baseName(),QString::number(NowCount),QString::number(j+1),suffix.isEmpty()?fi.suffix():suffix);
                    outFile = outFile.replace("\\s+","").replace(" ","");
                    QFile f(outFile);
                    if(f.exists()){
                        f.remove();
                    }
                    emit outFileStr();
                    if(fi.suffix().compare("mp4")==0){
                        vu.cutVideoByMP4(fi.absoluteFilePath(),outFile,starttime,endtime);
                    }else{
                        vu.cutVideo(fi.absoluteFilePath(),outFile,starttime,endtime);
                    }
                    emit currentIndexFinished();
                    if(!picPath.isEmpty()){
                        outFileByPic = QString("%1/%2_%3_part%4_pic.%5").arg( outPath,fi.baseName(),QString::number(NowCount),QString::number(j+1),suffix.isEmpty()?fi.suffix():suffix);
                        outFileByPic = outFileByPic.replace("\\s+","").replace(" ","");
                        vu.insertPicToVideo(picPath,outFile,outFileByPic);
                        if(f.exists()){
                            f.remove();
                        }
                    }
                    starttime=endtime;
                    if((endtime+cutTime) >= vi.videoTimeSeconds){
                        endtime=vi.videoTimeSeconds;
                    }else{
                        endtime=endtime+cutTime;
                    }
                    NowCount++;
                }
            }

            if(mode == 1){
                if(fi.exists()){
                    outFile = QString("%1/%2_%3_split.%4").arg( outPath,fi.baseName(),QString::number(i+1),suffix.isEmpty()?"aac":suffix);
                    outFile = outFile.replace("\\s+","").replace(" ","");
                    QFile f(outFile);
                    if(f.exists()){
                        f.remove();
                    }
                    emit outFileStr();
                    if(fi.suffix().compare("mp4")==0 && outFile.indexOf(".aac") != -1){
                        vu.splitVideoAudioMP4(fi.absoluteFilePath(),outFile);
                    }else{
                        vu.splitVideoAudio(fi.absoluteFilePath(),outFile);
                    }

                }
            }

            if(mode == 2){
                if(fi.exists()){
                    outFile = QString("%1/%2_%3_format.%4").arg( outPath,fi.baseName(),QString::number(i+1),suffix.isEmpty()?"aac":suffix);
                    outFile = outFile.replace("\\s+","").replace(" ","");
                    QFile f(outFile);
                    if(f.exists()){
                        f.remove();
                    }
                    emit outFileStr();
                    vu.transcodeVideo(fi.absoluteFilePath(),outFile);
                }
            }

        }

    }
    emit Finished();
}
