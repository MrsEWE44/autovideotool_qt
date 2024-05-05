#ifndef DIALOGUTILS_H
#define DIALOGUTILS_H

#include <QObject>
#include "videoutils.h"


class DialogUtils : public QObject
{
    Q_OBJECT
public:
    explicit DialogUtils(QObject *parent = nullptr);
    DialogUtils(QWidget *ww);

public:
    QList<QString> choiceFiles;
    QWidget *ww;
    int mode;
    int threads;
    int cutTime;
    int NowIndex;
    int NowCount;
    QString nowText;
    QString outPath;
    QString outFile;
    QString outFileByPic;
    QString picPath;
    QString suffix;
public slots:
    //自定义槽
    void Start();

signals:
    //自定义信号
    void currentIndexFinished();
    void currentIndex();
    void outFileStr();
    void Finished();
};

#endif // DIALOGUTILS_H
