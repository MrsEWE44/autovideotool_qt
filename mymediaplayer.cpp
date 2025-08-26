#include "mymediaplayer.h"

MyMediaPlayer::MyMediaPlayer()
{
    initBt();
}

MyMediaPlayer::MyMediaPlayer(QWidget *parent): QWidget(parent)
{
    initBt();
}

void MyMediaPlayer::initBt()
{

    QtAV::Widgets::registerRenderers();
    QtAV::setLogLevel(QtAV::LogLevel::LogOff);
    QtAV::setFFmpegLogLevel("off");

    m_unit = 1000;
    m_player = new QtAV::AVPlayer(this);
    QVBoxLayout *vl = new QVBoxLayout();
    listWidget = new QListWidget(this);
    m_vo = new QtAV::VideoOutput(this);

    QWidget *vw = m_vo->widget();
    if (!vw) {
        QMessageBox::warning(0, QString::fromLatin1("QtAV error"), QObject::tr("Can not create video renderer"));
    }
    m_player->setRenderer(m_vo);
    mediaTitle = new QLabel("wait media player");
    mediaTitle->setTextInteractionFlags(Qt::TextSelectableByMouse);
    vl->addWidget(mediaTitle,1);
    vl->addWidget(vw,7);
    m_slider = new QSlider();
    volume_slider = new QSlider();
    m_slider->setOrientation(Qt::Horizontal);
    volume_slider->setOrientation(Qt::Horizontal);
    volume_slider->setRange(0,100);
    volume_slider->setValue(80);
    m_player->audio()->setVolume( (float(volume_slider->value() / 100.0)));
    QObject::connect(m_slider, SIGNAL(sliderMoved(int)),SLOT(seekBySlider(int)));
    QObject::connect(m_slider, SIGNAL(sliderPressed),SLOT(seekBySlider()));
    QObject::connect(volume_slider, SIGNAL(valueChanged(int)),SLOT(volumeSeek(int)));
    QObject::connect(m_player,  SIGNAL(positionChanged(qint64)),SLOT(updateSlider(qint64)));
    QObject::connect(m_player,  SIGNAL(started()),SLOT(updateSlider()));
    QObject::connect(m_player,  SIGNAL(notifyIntervalChanged()),SLOT(updateSliderUnit()));
    QObject::connect(m_player,  &QtAV::AVPlayer::stopped,[=](){
        if(isLoopNext){
            int rowid = listWidget->currentRow();
            int index = isPlayNext ?(rowid == listWidget->count()-1?0:rowid+1):(rowid == 0 ? listWidget->count()-1:rowid-1);
            if(isDoublePlay){
                index = rowid;
            }
            QString tt = listWidget->item(index)->text();
            startPlayer(media_map[tt]);
            listWidget->setCurrentRow(index);
        }
        isDoublePlay = false;
        isLoopNext = true;
        isPlayNext = true;

    });


    vl->addWidget(m_slider,1);
    QHBoxLayout *hb = new QHBoxLayout();
    vl->addLayout(hb,1);
    m_openBtn = new QPushButton(QObject::tr("Previous"));
    m_playBtn = new QPushButton(QObject::tr("Play/Pause"));
    m_stopBtn = new QPushButton(QObject::tr("Next"));
    m_loadMetaDataBtn = new QPushButton(QObject::tr("LoadMetaData"));
    m_fullScreenBtn = new QPushButton(QObject::tr("FullScreen"));
    hb->addWidget(m_openBtn);
    hb->addWidget(m_playBtn);
    hb->addWidget(m_stopBtn);
    hb->addWidget(volume_slider);
    hb->addWidget(m_loadMetaDataBtn);
    hb->addWidget(m_fullScreenBtn);
    QObject::connect(m_openBtn, &QPushButton::clicked, [=](){
        isLoopNext = false;
        isPlayNext = false;
        isDoublePlay = false;
        int rowid = listWidget->currentRow();
        int index = rowid == 0 ? listWidget->count()-1:rowid-1;
        QString tt = listWidget->item(index)->text();
        startPlayer(media_map[tt]);
        listWidget->setCurrentRow(index);

    });

    QObject::connect(m_fullScreenBtn, &QPushButton::clicked, [=](){
        //单屏幕
//        QScreen* screen = QGuiApplication::primaryScreen();  //获取主屏幕
//        vw->resize(screen->availableSize());
        //全屏问题暂时没解决
    });
    QObject::connect(m_playBtn, &QPushButton::clicked, [=](){
        playPause();
    });
    QObject::connect(m_stopBtn, &QPushButton::clicked, [=](){
        isLoopNext = false;
        isPlayNext = true;
        isDoublePlay = false;
        int rowid = listWidget->currentRow();
        int index = rowid == listWidget->count()-1?0:rowid+1;
        QString tt = listWidget->item(index)->text();
        startPlayer(media_map[tt]);
        listWidget->setCurrentRow(index);

    });
    QObject::connect(m_loadMetaDataBtn, &QPushButton::clicked, [=](){
        isShowLrc = true;

//        qDebug() <<"duration ::: "<< m_player->duration();
//        qDebug() <<"durationF ::: "<< m_player->durationF();

        QHashIterator<QString, QString> i(mediadata);

        while (i.hasNext()) {
            i.next();
            if(i.key().compare("lyrics-XXX") == 0){
                lyrics_text = i.value();
            }
//            qDebug() << i.key() << ": " << i.value() ;
        }

        if(lyrics_text.isNull() || lyrics_text.isEmpty()){

            QString t = listWidget->currentItem()->text();
            QString p = media_map[t];
            QFileInfo finfo(p);
            QString lrc_file = finfo.absoluteFilePath().replace("."+finfo.completeSuffix(),".lrc");
            QFile f(lrc_file);
            if(f.exists()){
                if(f.open(QIODevice::ReadOnly)){
                    lyrics_text = QString(f.readAll());
                }
            }else{
                qDebug()<< t << " ::: lrc is null  --  " << p << " -- \r\n" << finfo.absoluteFilePath().replace("."+finfo.completeSuffix(),".lrc");
            }
        }

        //qDebug() << "lrc text ::: " << lyrics_text;
        size_t index = decodeHeader();
        std::string m_lrcData = lyrics_text.toStdString();
        if (index == m_lrcData.length()) {
            qDebug() <<"No lyrics text!";
        }else{

            m_lrcData.erase(0,index);
            //qDebug() << QString::fromStdString(m_lrcData);

            std::string line = readLrcLine();
            while (!line.empty()) {
                decodeLrcLine(line);
                line = readLrcLine();
            }
            lrcText.showlayout();

        }

    });

    QObject::connect(listWidget,&QListWidget::itemDoubleClicked,[=](){
        isDoublePlay = true;
        isLoopNext = true;
        isPlayNext = true;
        QString name = listWidget->currentItem()->text();
        //qDebug() << name << " -- " << media_map[name];
        startPlayer(media_map[name]);
    });




    hb = new QHBoxLayout();
    hb->addWidget(listWidget,2);
    hb->addLayout(vl,8);
    setLayout(hb);
}

size_t MyMediaPlayer::decodeHeader()
{
    size_t offset = 0;
    std::string m_lrcData = lyrics_text.toStdString();
    size_t length = m_lrcData.length();

    if (offset >= length) return offset;

    while(offset < length) {
        std::string meta, data;
        if (m_lrcData.at(offset) == '[') {
            while(++offset < length && m_lrcData.at(offset) != ':') {
                if (m_lrcData.at(offset) >= 'a' && m_lrcData.at(offset) <= 'z')
                    meta += m_lrcData.at(offset);
                else return offset - 1;
            }

            while(++offset < length && m_lrcData.at(offset) != ']') {
                data += m_lrcData.at(offset);
            }

        }

        offset++;
    }

    return offset;

}

std::string MyMediaPlayer::readLrcLine()
{
    std::string m_lrcData = lyrics_text.toStdString();
    size_t length = m_lrcData.length();
    std::string line;
    while (m_currentIndex < length) {
        if (m_lrcData.at(m_currentIndex) == '\n') {
            m_currentIndex++;
            break;
        } else {
            line += m_lrcData.at(m_currentIndex);
            m_currentIndex++;
        }
    }

    return line;
}

void MyMediaPlayer::decodeLrcLine(std::string line)
{
    size_t offset = 0;
    size_t length = line.length();

    if (offset >= length) return;

    std::string time;
    qint64 pts = 0;
    std::vector<qint64> times;
    while(offset < length) {
        if (line.at(offset) == '[') {
            offset++;
        } else if (line.at(offset) >= '0' && line.at(offset) <= '9') {
            time += line.at(offset);
            offset++;
        } else if (line.at(offset) == ']') {
            //10 millisecond
            pts += stoi(time) * 10;
            times.push_back(pts);
            time.clear();
            pts = 0;
            offset++;
        } else if (line.at(offset) == ':') {
            //minute, = 60s * 1000ms
            pts += stoi(time) * 60 * 1000;
            time.clear();
            offset++;
        } else if (line.at(offset) == '.') {
            //second, = 1000 ms
            pts += stoi(time) * 1000;
            time += line.at(offset);
            time.clear();
            offset++;
        } else break;
    }

    std::string data;
    while(offset < length) {
        data += line.at(offset);
        offset++;
    }

    for (qint64 pp : times) {
        MyMediaInfo info;
        info.lrcstr = QString::fromStdString(data);
        info.pts = pp;
        lrcList.push_back(info);
        //qDebug() << "pts :: " << pp  << "-"<<QString::fromStdString(data);
    }
}

void MyMediaPlayer::startPlayer(QString mediaFilePath)
{
    lyrics_text = "";
    m_currentIndex = 0;
    lrcText.close();
    lrcList.clear();
    m_player->play(mediaFilePath);

}
void MyMediaPlayer::openMedia()
{
    QString file = QFileDialog::getOpenFileName(0, QObject::tr("Open a video"));
    if (file.isEmpty())
        return;
    m_player->play(file);
}

void MyMediaPlayer::seekBySlider(int value)
{
    if (!m_player->isPlaying())
        return;
    m_player->seek(qint64(value*m_unit));
}

void MyMediaPlayer::seekBySlider()
{
    seekBySlider(m_slider->value());
}

void MyMediaPlayer::volumeSeek(int value)
{
    m_player->audio()->setVolume((float(value / 100.0)));
}

void MyMediaPlayer::playPause()
{
    if (!m_player->isPlaying()) {
        m_player->play();
    }
    m_player->pause(!m_player->isPaused());
}

void MyMediaPlayer::updateSlider(qint64 value)
{

    if(lrcList.size() > 0){
        //qDebug() << "lrcMap size ::: " << lrcList.size() <<  " --- " << value;

        for (int i = 0; i < lrcList.size(); i++) {
            int j=i+1;
            MyMediaInfo currentInfo = lrcList.at(i);
            if(j<lrcList.size() -1){
                MyMediaInfo nextInfo = lrcList.at(j);
                //qDebug() << currentInfo.pts << " -- " << nextInfo.pts;
                if(currentInfo.pts <= value && nextInfo.pts >= value){
                    //qDebug() <<"hit ::: " <<currentInfo.lrcstr;
                    lrcText.currentLrcLabel->setText(currentInfo.lrcstr);
                    lrcText.nextLrcLabel->setText(nextInfo.lrcstr);

                }
            }
//            else{
//                lrcText.currentLrcLabel->setText(currentInfo.lrcstr);
//                lrcText.nextLrcLabel->setText(currentInfo.lrcstr);
//            }
        }

    }
    m_slider->setRange(0, int(m_player->duration()/m_unit));
    m_slider->setValue(int(value/m_unit));

}

void MyMediaPlayer::updateSlider()
{
    m_currentIndex = 0;
    lrcList.clear();
    mediadata = m_player->statistics().metadata;
    QHashIterator<QString, QString> i(mediadata);
    QString ARTIST = "";
    QString TITLE = "";
    QString ALBUM = "";
    while (i.hasNext()) {
        i.next();
        QString key = i.key();
        QString value = i.value();
        if(key.compare("ARTIST", Qt::CaseInsensitive) == 0){
            ARTIST = value;
        }

        if(key.compare("TITLE", Qt::CaseInsensitive) == 0){
            TITLE = value;
        }

        if(key.compare("ALBUM", Qt::CaseInsensitive) == 0){
            ALBUM = value;
        }
        //qDebug() << key << ": " << value;
    }

//    qDebug() << "ARTIST ::: " << ARTIST;
//    qDebug() << "TITLE ::: " << TITLE;
//    qDebug() << "ALBUM ::: " << ALBUM;

    mediaTitle->setText(QString("%1-%2-%3").arg(ARTIST,TITLE,ALBUM));

    if(isShowLrc){
        m_loadMetaDataBtn->click();
    }

    updateSlider(m_player->position());
}

void MyMediaPlayer::updateSliderUnit()
{
    m_unit = m_player->notifyInterval();
    updateSlider();
}
