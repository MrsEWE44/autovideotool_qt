#ifndef SHOWLRCTEXT_H
#define SHOWLRCTEXT_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QScreen>
#include <QGuiApplication>
#include <QMouseEvent>
#include <QPushButton>
#include <QFontDialog>  //字体对话框
#include <QColorDialog> // 颜色对话框


class showLrcText : public QWidget
{
    Q_OBJECT
public:
    showLrcText();
    showLrcText(QString currentlrc,QString nextlrc);
    showLrcText(QString currentlrc);

protected:
    //拖拽窗口
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public:
    QString currentlrc,nextlrc;
    bool isShow = false;
    QLabel *currentLrcLabel,*nextLrcLabel;
    // 是否拖拽
    bool drag_ = false;
    QPoint mouse_start_point_;
    QPoint window_top_left_point_;
    QPushButton *fontChangeBt,*closeLrcBt,*fontColorChangeBt;


public:
    void init();
    void showlayout();
    QFont getFont();
};

#endif // SHOWLRCTEXT_H
