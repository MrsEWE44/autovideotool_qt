#include "videotoollayout.h"

VideoToolLayout::VideoToolLayout()
{

}

VideoToolLayout::VideoToolLayout(QMainWindow *m)
{
    this->m = m;
}

void VideoToolLayout::init()
{
    choiceStrList << "勾选" << "未勾选" << "全选";
    modeStrList << "视频剪辑" << "视频音频提取" << "视频转码";
    timeUnitStrList << "秒" << "分钟" << "小时" << "天";
    picStrList << "中" << "上" << "下" << "左" << "右" << "左上" << "左下" << "右上" << "右下";
    picTextUnitBoxStrList <<"封面集数样式,默认是数字(123)" << "中文简体数字(一二三)" << "中文繁体数字(壹贰叁)";
    picTextTemplateBoxStrList << "封面集数模板,默认是数字" << "第 XX 集" << "part: XX" << "《XX》" << "（XX）";
    outFormatBoxStrList << "视频" << "音频";
    choiceBox = new QComboBox();
    modeBox = new QComboBox();
    timeUnitBox = new QComboBox();
    picBox = new QComboBox();
    picTextUnitBox = new QComboBox();
    picTextTemplateBox = new QComboBox();
    outFormatBox = new QComboBox();
    outFormatUnitBox = new QComboBox();
    vbox = new QVBoxLayout();
    lw = new QListWidget();
    startBt = new QPushButton("开始");
    chooseOutPathBt = new QPushButton("选择输出路径");
    choosePicPathBt = new QPushButton("选择封面文件");
    chooseColorBt = new QPushButton("选择字体颜色");
    chooseFontStyleBt = new QPushButton("选择字体");
    previewPicBt = new QPushButton("预览封面更改");
    fileM = m->menuBar()->addMenu("文件");
    helpM = m->menuBar()->addMenu("帮助");
    fileOpenA = new QAction("打开本地音视频文件");
    dirOpenA = new QAction("打开本地音视频文件夹");
    helpOpenA = new QAction("关于");
    exitA = new QAction("退出");
    outPath = new QLabel("文件输出路径");
    cutTime = new QLabel("视频时间");
    picPath = new QLabel("视频封面路径");
    picLabel = new QLabel("封面预览位置");
    picFontLocation = new QLabel("封面文字位置");
    outFormat = new QLabel("输出格式");
    outPathEdit = new QLineEdit();
    cutTimeEdit = new QLineEdit();
    picPathEdit = new QLineEdit();
    picTextEdit = new QLineEdit();
    initStartCountEdit = new QLineEdit();
    outPathEdit->setPlaceholderText("请输入文件输出路径或者选择一个输出路径");
    cutTimeEdit->setPlaceholderText("请输入剪辑后每个视频的时间长度");
    picPathEdit->setPlaceholderText("请输入视频封面路径或者选择一个视频封面文件");
    picTextEdit->setPlaceholderText("请输入在视频封面显示的文字");
    initStartCountEdit->setPlaceholderText("请输入视频剪辑前的起始位置(默认是从1开始)");
    QIntValidator *q = new QIntValidator();
    q->setRange(0,INT_MAX);
    cutTimeEdit->setValidator(q);
    initStartCountEdit->setValidator(q);
    lw->setSortingEnabled(true);
    fileM->addAction(fileOpenA);
    fileM->addAction(dirOpenA);
    fileM->addSeparator();
    fileM->addAction(exitA);
    helpM->addAction(helpOpenA);
    choiceBox->addItems(choiceStrList);
    modeBox->addItems(modeStrList);
    timeUnitBox->addItems(timeUnitStrList);
    picBox->addItems(picStrList);
    picTextUnitBox->addItems(picTextUnitBoxStrList);
    picTextTemplateBox->addItems(picTextTemplateBoxStrList);
    outFormatBox->addItems(outFormatBoxStrList);
    outFormatUnitBox->addItems(videoFillterStr.split("*."));

    chooseColor.setRgba(Qt::red);
    chooseFont.setPixelSize(45);
    chooseFont.setFamily("Microsoft YaHei");

    setWidgetFixedSize({previewPicBt,picTextEdit,picTextUnitBox,picTextTemplateBox,picBox,
                        chooseColorBt,chooseFontStyleBt,initStartCountEdit},1);

    setWidgetFixedSize({choiceBox,modeBox,cutTimeEdit,timeUnitBox,picPathEdit,
                        choosePicPathBt,outFormatBox,outFormatUnitBox,outPathEdit,chooseOutPathBt},0);
    outFormatBox->setCurrentIndex(0);
    outFormatUnitBox->setCurrentIndex(1);

    connectFun();
}

void VideoToolLayout::connectFun()
{
    QObject::connect(modeBox,&QComboBox::currentTextChanged,[=](){
        switch (modeBox->currentIndex()) {
        case 0:
            setWidgetFixedSize({previewPicBt,picTextEdit,picTextUnitBox,picTextTemplateBox,picBox,
                                chooseColorBt,chooseFontStyleBt,initStartCountEdit},1);

            setWidgetFixedSize({choiceBox,modeBox,cutTimeEdit,timeUnitBox,picPathEdit,
                                choosePicPathBt,outFormatBox,outFormatUnitBox,outPathEdit,chooseOutPathBt},0);
            outFormatBox->setCurrentIndex(0);
            outFormatUnitBox->setCurrentIndex(1);
            break;
        case 1:
            setWidgetFixedSize({previewPicBt,picTextEdit,picTextUnitBox,picTextTemplateBox,picBox,
                                chooseColorBt,chooseFontStyleBt,initStartCountEdit,cutTimeEdit,timeUnitBox,picPathEdit,
                                choosePicPathBt},1);

            setWidgetFixedSize({choiceBox,modeBox,outFormatBox,outFormatUnitBox,outPathEdit,chooseOutPathBt},0);

            outFormatBox->setCurrentIndex(1);
            outFormatUnitBox->setCurrentIndex(2);
            break;
        case 2:
            setWidgetFixedSize({previewPicBt,picTextEdit,picTextUnitBox,picTextTemplateBox,picBox,
                                chooseColorBt,chooseFontStyleBt,initStartCountEdit,cutTimeEdit,timeUnitBox,picPathEdit,
                                choosePicPathBt},1);

            setWidgetFixedSize({choiceBox,modeBox,outFormatBox,outFormatUnitBox,outPathEdit,chooseOutPathBt},0);
            outFormatBox->setCurrentIndex(0);
            outFormatUnitBox->setCurrentIndex(1);
            break;
        }
    });

    QObject::connect(outFormatBox,&QComboBox::currentTextChanged,[=](){
        outFormatUnitBox->clear();
        outFormatUnitBox->addItems(outFormatBox->currentIndex()==0?videoFillterStr.split("*."):audioFillterStr.split("*."));
    });

    QObject::connect(previewPicBt,&QPushButton::clicked,[=](){
        if(!picPathEdit->text().isEmpty()){
            picLabel->setPixmap(getPicTextPixMap(1,300,300));
        }
    });

    QObject::connect(chooseFontStyleBt,&QPushButton::clicked,[=](){
        bool fff = true;
        chooseFont = QFontDialog::getFont(&fff);
    });

    QObject::connect(chooseColorBt,&QPushButton::clicked,[=](){
        chooseColor = QColorDialog::getColor(Qt::white, ww);
    });

    QObject::connect(choosePicPathBt,&QPushButton::clicked,[=](){
        qDebug() << choosePicPathBt->text();
        chooseLocalPicPath = QFileDialog::getOpenFileName(ww,"请选择视频封面文件",".",QString("封面文件 (%1) ;; 所有文件 (*.*)").arg(imgFillterStr));
        if(chooseLocalPicPath.isEmpty()){
            QMessageBox::critical(ww,"错误","请选择一个正确的视频封面文件",QMessageBox::Ok);
        }else{
            previewPicBt->setEnabled(true);
            picTextEdit->setEnabled(true);
            picTextUnitBox->setEnabled(true);
            picTextTemplateBox->setEnabled(true);
            picBox->setEnabled(true);
            chooseColorBt->setEnabled(true);
            chooseFontStyleBt->setEnabled(true);
            initStartCountEdit->setEnabled(true);

            picPathEdit->setText(chooseLocalPicPath);
            QPixmap p(chooseLocalPicPath);
            picLabel->setPixmap(p.scaled(300,300,Qt::KeepAspectRatio));
        }

    });


    QObject::connect(chooseOutPathBt,&QPushButton::clicked,[=](){
        qDebug() << chooseOutPathBt->text();
        chooseOutPathDir = QFileDialog::getExistingDirectory(ww,"请选择一个用于存放输出文件的路径",".",QFileDialog::ShowDirsOnly);
        if(chooseOutPathDir.isEmpty()){
            QMessageBox::critical(ww,"错误","请选择一个用于存放输出文件的正确路径",QMessageBox::Ok);
        }
        outPathEdit->setText(chooseOutPathDir);
    });

    QObject::connect(startBt,&QPushButton::clicked,[=](){
        startBt->setEnabled(false);
        QList<QString> choiceFiles;
        int cut_time = 0;
        for(int i =0;i<lw->count();i++){
            QListWidgetItem *item = lw->item(i);
            QCheckBox *c = (QCheckBox *) lw->itemWidget(item);
            if(choiceBox->currentIndex() ==0 && c->isChecked()){
                choiceFiles.append(item->text());
            }

            if(choiceBox->currentIndex() ==1 && !c->isChecked()){
                choiceFiles.append(item->text());
            }

            if(choiceBox->currentIndex() ==2 ){
                choiceFiles.append(item->text());
            }
        }

        if(modeBox->currentIndex() == 0 &&  cutTimeEdit->text().isEmpty()){
            QMessageBox::critical(ww,"错误","请输入需要剪辑的视频长度大小",QMessageBox::Ok);
            startBt->setEnabled(true);
        }else{
            QProgressDialog  *pd = new QProgressDialog();
            pd->setWindowModality(Qt::WindowModal);
            pd->setMinimumDuration(1);
            switch (modeBox->currentIndex()) {
            case 0:
                if(!cutTimeEdit->text().isEmpty()){
                    cut_time = cutTimeEdit->text().toInt();
                    switch (timeUnitBox->currentIndex()) {
                    case 1:
                        cut_time = cut_time * 60;
                        break;
                    case 2:
                        cut_time = cut_time * 60 * 60;
                        break;
                    case 3:
                        cut_time = cut_time * 60 * 60 * 60;
                        break;
                    }
                }

                if(!picPathEdit->text().isEmpty()){
                    picLabel->setPixmap(getPicTextPixMap(1,300,300));
                }
                pd->setWindowTitle("当前正在裁剪视频(请稍后...)");
                pd->setLabelText("裁剪中...");
                break;
            case 1:
                pd->setWindowTitle("当前正在提取视频的音频(请稍后...)");
                pd->setLabelText("提取中...");
                break;
            case 2:
                pd->setWindowTitle("当前正在转码视频(请稍后...)");
                pd->setLabelText("转码中...");
                break;
            }

            pd->setCancelButton(0);
            pd->setRange(0, choiceFiles.size()-1);
            pd->setFixedSize(ww->width(),200);
            pd->show();
            DialogUtils *du = new DialogUtils(ww);
            QThread *th = new QThread(ww);
            du->NowCount = initStartCountEdit->text().isEmpty()?1:initStartCountEdit->text().toInt();
            du->threads=th->idealThreadCount();
            du->choiceFiles = choiceFiles;
            du->mode = modeBox->currentIndex();
            du->outPath = outPathEdit->text().isEmpty()?c.getCurrentPath():outPathEdit->text();
            du->cutTime = cut_time;
            du->suffix = outFormatUnitBox->currentText();
            th->start();
            du->moveToThread(th);
            QObject::connect(th,&QThread::started,ww,[=](){
                du->Start();
            });

            QObject::connect(du,&DialogUtils::currentIndexFinished,ww,[=](){
                if(!picPathEdit->text().isEmpty()){
                    QString saveLocalTempFile = du->outPath+"/temp.png";
                    QFile f(saveLocalTempFile);
                    if(f.exists()){
                        f.remove();
                    }
                    QPixmap pm = getPicTextPixMap(du->NowCount,720,1080);
                    pm.save(saveLocalTempFile);
                    du->picPath = saveLocalTempFile;
                }else{
                    du->picPath = "";
                }
            });

            QObject::connect(du,&DialogUtils::currentIndex,ww,[=](){
                pd->setValue(du->NowIndex);
                pd->setLabelText(du->nowText);
            });

            QObject::connect(th, &QThread::finished, th, &QThread::deleteLater);
            QObject::connect(du, &DialogUtils::Finished, [=](){
                du->deleteLater();
                th->quit();
                delete pd;
                int ret = QMessageBox::information(ww, "提示","操作已经结束",QMessageBox::Ok);
                startBt->setEnabled(true);
            });
        }
    });

    QObject::connect(fileOpenA,&QAction::triggered,[=]{
        selectLocalFileListStr = QFileDialog::getOpenFileNames(ww,"请选择音视频文件",".",QString("视频文件 (%1) ;; 音频文件 (%2) ;; 所有文件(*.*)").arg(videoFillterStr,audioFillterStr));
        if(selectLocalFileListStr.isEmpty()){
            QMessageBox::critical(ww,"错误","请选择正确的音视频文件",QMessageBox::Ok);
        }else{
            for(QString s : selectLocalFileListStr){
                QListWidgetItem *qlwitem = new QListWidgetItem(lw);
                QCheckBox *chbox = new QCheckBox();
                qlwitem->setText(s);
                lw->addItem(qlwitem);
                lw->setItemWidget(qlwitem,chbox);
            }
        }
    });

    QObject::connect(dirOpenA,&QAction::triggered,[=]{
        selectLocalDirStr = QFileDialog::getExistingDirectory(ww,"请选择存有音视频的文件夹",".",QFileDialog::ShowDirsOnly);
        if(selectLocalDirStr.isEmpty()){
            QMessageBox::critical(ww,"错误","请选择正确的音视频文件夹",QMessageBox::Ok);
        }else{
            QDir d(selectLocalDirStr);
            if(d.exists()){
                d.setFilter(QDir::Files);
                d.setSorting(QDir::Name);
                d.setNameFilters(QString("%1 %2").arg(videoFillterStr,audioFillterStr).split(" "));
                QFileInfoList qfil = d.entryInfoList();
                for(QFileInfo f : qfil){
                    QListWidgetItem *qlwitem = new QListWidgetItem(lw);
                    QCheckBox *chbox = new QCheckBox();
                    qlwitem->setText(f.absoluteFilePath());
                    lw->addItem(qlwitem);
                    lw->setItemWidget(qlwitem,chbox);
                    lw->setItemAlignment(Qt::AlignHCenter);
                }
            }
        }
    });

    QObject::connect(helpOpenA,&QAction::triggered,[=]{
        QMessageBox::information(ww,"帮助","视频自动化工具\r\n\r\n"
                                         "该软件的界面、功能是由qt5+ffmpeg实现.\r\n\r\n"
                                         "主要是用来编辑音视频的,支持批量自动化剪辑、提取音视频、转码等功能.\r\n\r\n"
                                         "在剪辑视频的时候，支持自定义视频封面、输出的视频格式、插入封面文字、封面文字位置.\r\n\r\n"
                                         "提取音频文件的时候,支持选择输出的音频格式.\r\n\r\n"
                                         "转码音视频文件的时候，支持选择输出的音视频格式.\r\n\r\n",QMessageBox::Ok);
    });


    QObject::connect(exitA,&QAction::triggered,[=]{
        QApplication *app;
        app->exit(0);
    });


}

int VideoToolLayout::check()
{
    //do...
    return 0;
}

void VideoToolLayout::showLayout()
{
    init();
    if(check() == 0){
        ww = new QWidget();
        m->setWindowTitle("视频自动化工具");
        m->setCentralWidget(ww);
        m->close();
        vbox2 = new QVBoxLayout();
        QHBoxLayout *hbox = new QHBoxLayout();
        vbox2->addLayout(addHboxWidget(choiceBox,modeBox));
        vbox2->addLayout(addHboxWidget(cutTime,cutTimeEdit,timeUnitBox));
        vbox2->addLayout(addHboxWidget(picPath,picPathEdit,choosePicPathBt));
        QVBoxLayout *vboxt = new QVBoxLayout();
        QHBoxLayout *hboxt = new QHBoxLayout();
        vboxt->addLayout(addHboxWidget(picFontLocation,picBox));
        vboxt->addWidget(picTextEdit);
        vboxt->addWidget(initStartCountEdit);
        vboxt->addLayout(addHboxWidget(picTextUnitBox,picTextTemplateBox));
        vboxt->addLayout(addHboxWidget(chooseColorBt,chooseFontStyleBt));
        vboxt->addWidget(previewPicBt);
        hboxt->addWidget(picLabel);
        hboxt->addLayout(vboxt);
        vbox2->addLayout(hboxt);
        vbox2->addLayout(addHboxWidget(outFormat,outFormatBox,outFormatUnitBox));
        vbox2->addLayout(addHboxWidget(outPath,outPathEdit,chooseOutPathBt));
        hbox->addWidget(lw);
        hbox->addLayout(vbox2);
        vbox->addLayout(hbox);
        vbox->addWidget(startBt);
        ww->setLayout(vbox);
    }
}

QHBoxLayout *VideoToolLayout::addHboxWidget(QWidget *w1, QWidget *w2)
{
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(w1);
    hbox->addWidget(w2);
    return hbox;
}

QHBoxLayout* VideoToolLayout::addHboxWidget(QWidget *w1, QWidget *w2, QWidget *w3,QWidget *w4)
{
    QHBoxLayout *hbox = new QHBoxLayout();
    QHBoxLayout *hbox1 = new QHBoxLayout();
    hbox->addWidget(w1);
    hbox->addWidget(w2);
    hbox->setAlignment(Qt::AlignLeft);
    hbox1->addLayout(hbox);
    hbox = new QHBoxLayout();
    hbox->addWidget(w3);
    hbox->addWidget(w4);
    hbox->setAlignment(Qt::AlignLeft);
    hbox1->addLayout(hbox);
    return hbox1;
}

QHBoxLayout* VideoToolLayout::addHboxWidget(QWidget *w1, QWidget *w2, QWidget *w3)
{
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(w1);
    hbox->addWidget(w2);
    hbox->addWidget(w3);
    hbox->setAlignment(Qt::AlignLeft);
    return hbox;
}

void VideoToolLayout::insertHboxWidget(int index, QWidget *w1, QWidget *w2)
{
    vbox2->insertLayout(index,addHboxWidget(w1,w2));
}

QString VideoToolLayout::getPicTextTemplateBoxStr(int i)
{
    QString picTextTemplateStr,ns = c.numberToCustomStr(i,picTextUnitBox->currentIndex());
    switch (picTextTemplateBox->currentIndex()) {
    case 0:
        picTextTemplateStr = "";
        break;
    case 1:
        picTextTemplateStr = QString("第 %1 集").arg(ns);
        break;
    case 2:
        picTextTemplateStr = QString("part: %1").arg(ns);
        break;
    case 3:
        picTextTemplateStr = QString("《%1》").arg(ns);
        break;
    case 4:
        picTextTemplateStr = QString("（%1）").arg(ns);
        break;
    }
    return  picTextTemplateStr;
}

QPixmap VideoToolLayout::getPicTextPixMap(int nowcount, int scalew, int scaleh)
{
    return c.drawImgText(picPathEdit->text()
                         ,QString("%1 %2").arg(picTextEdit->text(),getPicTextTemplateBoxStr(nowcount))
                         ,chooseFont,chooseColor,picBox->currentIndex())
            .scaled(scalew,scaleh,Qt::KeepAspectRatio);
}

void VideoToolLayout::setWidgetFixedSize(QList<QWidget *> l, int isEnable)
{
    for(QWidget *w : l){
        w->setFixedHeight(40);
        w->setEnabled(isEnable == 0);
    }
}
