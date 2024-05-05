#include "coreutils.h"

CoreUtils::CoreUtils()
{

}

QString CoreUtils::runCMD(QString cmdstr)
{
    cmdResultStr.clear();
    p = new QProcess();
    QObject::connect(p,&QProcess::readyReadStandardOutput,[=](){
        cmdResultStr.append(QString::fromLocal8Bit(p->readAllStandardOutput()));
    });
    QObject::connect(p,&QProcess::readyReadStandardError,[=](){
        cmdResultStr.append( QString::fromLocal8Bit(p->readAllStandardError()));
    });
    p->start(cmdstr);
    p->waitForFinished();
    p->close();
    return  cmdResultStr;
}

QString CoreUtils::getCurrentPath()
{
    return QCoreApplication::applicationDirPath();
}

QString CoreUtils::FormatHHMMSS(long total)
{
    int hh = total / (60 * 60);
    int mm = (total- (hh * 60 * 60)) / 60;
    int ss = (total - (hh * 60 * 60)) - mm * 60;

    QString strTime = QString("%1:%2:%3")
            .arg(hh, 2, 10, QLatin1Char('0'))
            .arg(mm, 2, 10, QLatin1Char('0'))
            .arg(ss, 2, 10, QLatin1Char('0'));

    return strTime;
}

QString CoreUtils::numberToCustomStr(int num,int mode)
{
    QString ss;
    if(mode == 0){
        return QString::number(num);
    }
    int needZero = -1;
    int pos=0;           //节权位的位置
    if(num==0){
        //如果num为0，进行特殊处理。
        ss.insert(0,mode == 1 ? CH_Simplified_NUMBER[0] : CH_Traditional_NUMBER[0]);
    }
    while (num > 0) {
        int section = num % 10000;
        if (needZero == 0) {
            ss.insert(0, mode == 1 ? CH_Simplified_NUMBER[0] : CH_Traditional_NUMBER[0]);
        }
        QString sectionToChn = SectionNumToChn(section,mode);
        //判断是否需要节权位
        sectionToChn += (section != 0) ? (mode == 1 ? CH_Simplified_UNIT_SECTION[pos] : CH_Traditional_UNIT_SECTION[pos]) : (mode == 1 ? CH_Simplified_UNIT_SECTION[0] : CH_Traditional_UNIT_SECTION[0]);
        ss.insert(0, sectionToChn);
        needZero = ((section < 1000 && section > 0) ? 0 : -1); //判断section中的千位上是不是为零，若为零应该添加一个零。
        pos++;
        num = num / 10000;
    }
    return  ss;
}

QPixmap CoreUtils::drawImgText(QString imgPath, QString text,QFont font , QColor c, int mode)
{
    QPixmap p(imgPath);
    QPainter painter(&p);
    painter.setPen(c);
    painter.setFont(font);
    int imgx,imgy,newx,newy;
    imgx = p.width();
    imgy = p.height();
    QFontMetrics fm(font);
    int fontw = fm.width(text);
    int fonth = fm.height();

    switch (mode) {
    case 0:
        newx = ((imgx /2)-(fontw/2));
        newy = ((imgy /2)-(fonth/2));
        break;
    case 1:
        newx = ((imgx /2)-(fontw/2));
        newy = imgy - (imgy-fonth-(fonth/2)-font.pixelSize());
        break;
    case 2:
        newx = ((imgx /2)-(fontw/2));
        newy = (imgy -fonth-font.pixelSize());
        break;
    case 3:
        newx = (imgx %10);
        newy = ((imgy /2)-(fonth/2));
        break;
    case 4:
        newx = imgx-fontw - font.pixelSize();
        newy = ((imgy /2)-(fonth/2));
        break;
    case 5:
        newx = (imgx %10);
        newy = fonth+font.pixelSize();
        break;
    case 6:
        newx = (imgx %10);
        newy = (imgy -(fonth/2));
        break;
    case 7:
        newx = imgx-fontw - font.pixelSize();
        newy = fonth+font.pixelSize();
        break;
    case 8:
        newx = imgx-fontw - font.pixelSize();
        newy = (imgy -(fonth/2));
        break;

    }
    QPoint po(newx,newy);
    painter.drawText(po,text);
    return p;
}

QString CoreUtils::SectionNumToChn(int section,int mode)
{
    QString returnStr;
    int unitPos = 0;       //节权位的位置编号，0-3依次为个十百千;

    int zero = -1;
    while (section > 0) {
        int v = (section % 10);
        if (v == 0) {
            if ((section == 0) || zero != -1) {
                zero = 0; /*需要补0，zero的作用是确保对连续的多个0，只补一个中文零*/
                //chnStr.insert(0, chnNumChar[v]);
                returnStr.insert(0, mode == 1 ? CH_Simplified_NUMBER[v] : CH_Traditional_NUMBER[v]);
            }
        } else {
            zero = -1; //至少有一个数字不是0
            QString tempStr = mode == 1 ? CH_Simplified_NUMBER[v] : CH_Traditional_NUMBER[v];//数字v所对应的中文数字
            tempStr.append(mode == 1 ? CH_Simplified_UNIT[unitPos] : CH_Traditional_UNIT[unitPos]);  //数字v所对应的中文权位
            returnStr.insert(0, tempStr);
        }
        unitPos++; //移位
        section = section / 10;
    }
    return returnStr;
}


