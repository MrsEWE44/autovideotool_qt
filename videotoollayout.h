#ifndef VIDEOTOOLLAYOUT_H
#define VIDEOTOOLLAYOUT_H
#include <QMainWindow>
#include <QWidget>
#include <QMessageBox>
#include <QThread>
#include <QObject>
#include <QListWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QColorDialog>
#include <QFontDialog>
#include <QProgressDialog>
#include <QTableWidget>
#include <QSlider>
#include <QPixmap>

#include "coreutils.h"
#include "dialogutils.h"
#include "mymediaplayer.h"

class VideoToolLayout
{
public:
    VideoToolLayout();
    VideoToolLayout(QMainWindow *m);

public:
    QTabWidget *tabWidget;
    QWidget *ww,*ww1,*ww2;
    QMainWindow *m;
    QPushButton *startBt,*chooseOutPathBt,*choosePicPathBt,*chooseColorBt
    ,*chooseFontStyleBt,*previewPicBt;
    QVBoxLayout *vbox;
    QMenu *fileM,*helpM;
    QAction *fileOpenA,*dirOpenA,*helpOpenA,*exitA;
    QListWidget *lw;
    QComboBox *choiceBox,*modeBox,*timeUnitBox,*picBox,*picTextUnitBox
    ,*picTextTemplateBox,*outFormatBox,*outFormatUnitBox;
    QLabel *cutTime , *outPath , *picPath , *picLabel , *picFontLocation,*outFormat;
    QLineEdit *cutTimeEdit, *outPathEdit , *picPathEdit,*picTextEdit,*initStartCountEdit;
    QColor chooseColor;
    QFont chooseFont;


public:
    QStringList selectLocalFileListStr ;
    QString selectLocalDirStr,chooseOutPathDir,chooseLocalPicPath;
    QStringList choiceStrList,modeStrList,timeUnitStrList,picStrList,picTextUnitBoxStrList,picTextTemplateBoxStrList,outFormatBoxStrList;
    QString videoFillterStr="*.mp4 *.rmvb *.avi *.mkv *.flv *.mov *.wmv *.webm *.ts";
    QString audioFillterStr = "*.wav *.aac *.mp3 *.flac *.mp4a";
    QString imgFillterStr = "*.png *.xpm *.jpg *.jpeg *.bmp";
    int play_start_index = 0;

public:
    CoreUtils c;
    MyMediaPlayer *myMediaPlayer ;

public:
    //初始化界面、按钮组件
    void init();
    //绑定信号槽
    void connectFun();
    //检查各项功能是否正常
    int check();
    //显示布局界面
    void showLayout();
    //添加组件到QHBoxLayout布局界面里面
    QHBoxLayout* addHboxWidget(QList<QWidget *> w1);
    //拼接封面文字模板内容
    QString getPicTextTemplateBoxStr(int i);
    //拼接封面文字模板内容并返回图片
    QPixmap getPicTextPixMap(int nowcount,int scalew,int scaleh);
    //设置组件高度跟启用状态
    void setWidgetFixedSize(QList<QWidget *> l,int isEnable);


};

#endif // VIDEOTOOLLAYOUT_H
