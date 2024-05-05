#ifndef COREUTILS_H
#define COREUTILS_H

#include <QProcess>
#include <QCoreApplication>
#include "BaseIncludeFiles.h"
#include <QFont>
#include <QFontMetrics>
#include <QImage>
#include <QPainter>


class CoreUtils
{
public:
    CoreUtils();
public:
    QString cmdResultStr;
    QProcess *p;
    QStringList CH_Simplified_NUMBER = {"零", "一", "二", "三", "四", "五", "六", "七", "八", "九"};
    QStringList CH_Simplified_UNIT = {"", "十", "百", "千"};
    QStringList CH_Simplified_UNIT_SECTION = {"", "万", "亿", "万亿"};
    QStringList CH_Traditional_NUMBER = {"零", "壹", "贰", "叁", "肆", "伍", "陆", "柒", "捌", "玖"};
    QStringList CH_Traditional_UNIT = {"", "拾", "佰", "仟"};
    QStringList CH_Traditional_UNIT_SECTION = {"", "萬", "億", "萬億"};

public:
    //调用系统自带的命令
    QString runCMD(QString cmdstr);
    //获取程序运行的所在路径
    QString getCurrentPath();
    //格式化时间长度为hh:mm:ss这种格式
    QString FormatHHMMSS(long total);
    //数值转中文大写
    QString numberToCustomStr(int num,int mode);
    //往图片上写入文字
    QPixmap drawImgText(QString imgPath , QString text,QFont font , QColor c , int mode);
private:
    QString SectionNumToChn(int section,int mode);


};

#endif // COREUTILS_H
