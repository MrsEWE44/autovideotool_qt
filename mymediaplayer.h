#ifndef MYMEDIAPLAYER_H
#define MYMEDIAPLAYER_H

#include <QWidget>
#include <QtAV>
#include <QtAVWidgets>
#include <QPushButton>
#include <QSlider>
#include <QLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QObject>
#include <QDebug>
#include <QListWidget>
#include <QListWidgetItem>
#include <QHash>
#include "showlrctext.h"

class  MyMediaInfo{
  public:
    QString lrcstr;
    qint64 pts;
};

class MyMediaPlayer : public QWidget
{
    Q_OBJECT
public:
    MyMediaPlayer();
    explicit MyMediaPlayer(QWidget *parent = 0);
private:
    void initBt();
    size_t decodeHeader();
    std::string readLrcLine();
    void decodeLrcLine(std::string line);
    void startPlayer(QString mediaFilePath);

public Q_SLOTS:
    void openMedia();
    void seekBySlider(int value);
    void seekBySlider();
    void volumeSeek(int value);
    void playPause();
private Q_SLOTS:
    void updateSlider(qint64 value);
    void updateSlider();
    void updateSliderUnit();

public:
    QtAV::VideoOutput *m_vo;
    QtAV::AVPlayer *m_player;
    QSlider *m_slider,*volume_slider;
    QPushButton *m_openBtn;
    QPushButton *m_playBtn;
    QPushButton *m_stopBtn;
    QPushButton *m_loadMetaDataBtn;
    QPushButton *m_fullScreenBtn;
    int m_unit;
    int m_currentIndex;
    QString lyrics_text;
    QList<MyMediaInfo> lrcList;
    QListWidget *listWidget;
    QAction *openFile,*openDir;
    QHash<QString,QString> media_map,mediadata;
    bool isLoopNext = false;
    bool isPlayNext = false;
    bool isDoublePlay = false;
    bool isShowLrc = false;
    QLabel *mediaTitle;
public:
    showLrcText lrcText;
    bool isShowLayout;
};

#endif // MYMEDIAPLAYER_H
