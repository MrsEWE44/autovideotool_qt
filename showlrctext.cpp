#include "showlrctext.h"

showLrcText::showLrcText()
{

}

showLrcText::showLrcText(QString currentlrc, QString nextlrc)
{
    this->currentlrc = currentlrc;
    this->nextlrc = nextlrc;
    init();
}

showLrcText::showLrcText(QString currentlrc)
{
    this->currentlrc = currentlrc;
    init();
}

void showLrcText::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
       drag_ = true;
       //获得鼠标的初始位置
       mouse_start_point_ = event->globalPos();
       //获得窗口的初始位置
       window_top_left_point_ = this->frameGeometry().topLeft();
    }

    if(event->button() == Qt::RightButton){
        fontChangeBt->setVisible(!fontChangeBt->isVisible());
        closeLrcBt->setVisible(!closeLrcBt->isVisible());
        fontColorChangeBt->setVisible(!fontColorChangeBt->isVisible());
    }

}

void showLrcText::mouseMoveEvent(QMouseEvent *event)
{
    if (drag_){
        // 获取鼠标移动的距离
        QPoint distance = event->globalPos() - mouse_start_point_;
        // 改变窗口的位置
        this->move(window_top_left_point_ + distance);
    }
}

void showLrcText::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        drag_ = false;
    }
}

void showLrcText::init()
{
    if(isShow == false){

        //单屏幕
        QScreen* screen = QGuiApplication::primaryScreen();  //获取主屏幕
        this->setMinimumWidth(screen->availableSize().width());
        QHBoxLayout *hbox = new QHBoxLayout();
        QVBoxLayout *vbox = new QVBoxLayout();
        fontChangeBt = new QPushButton("修改字体");
        closeLrcBt = new QPushButton("关闭歌词");
        fontColorChangeBt = new QPushButton("修改字体颜色");
        currentLrcLabel = new QLabel(currentlrc);
        nextLrcLabel = new QLabel(nextlrc);
        currentLrcLabel->setAlignment(Qt::AlignCenter);
        nextLrcLabel->setAlignment(Qt::AlignCenter);
        fontChangeBt->setVisible(false);
        closeLrcBt->setVisible(false);
        fontColorChangeBt->setVisible(false);
        currentLrcLabel->setFont(getFont());
        nextLrcLabel->setFont(getFont());
        currentLrcLabel->setWordWrap(true);
        nextLrcLabel->setWordWrap(true);

        QObject::connect(closeLrcBt,&QPushButton::clicked,[=](){
            this->close();
        });

        QObject::connect(fontChangeBt,&QPushButton::clicked,[=](){
            bool ok;
            QFont f = QFontDialog::getFont(
                            &ok, QFont("选择字体参数", 10), this);
            if (ok) {
                currentLrcLabel->setFont(f);
                nextLrcLabel->setFont(f);
            }

        });

        QObject::connect(fontColorChangeBt,&QPushButton::clicked,[=](){

            QColor color = QColorDialog::getColor(Qt::white, this);
            // 如果用户选择了颜色
            if (color.isValid()) {
                // 设置 QLabel 的背景颜色
                QPalette palette = currentLrcLabel->palette();
                palette.setColor(QPalette::WindowText, color);
                currentLrcLabel->setPalette(palette);
                // 设置 QLabel 的背景颜色
                QPalette palette2 = nextLrcLabel->palette();
                palette.setColor(QPalette::WindowText, color);
                nextLrcLabel->setPalette(palette);
            }
        });
        hbox->addWidget(closeLrcBt);
        hbox->addWidget(fontChangeBt);
        hbox->addWidget(fontColorChangeBt);
        vbox->addLayout(hbox);
        vbox->addWidget(currentLrcLabel);
        vbox->addWidget(nextLrcLabel);
        setLayout(vbox);
        this->setAttribute(Qt::WA_TranslucentBackground, true);
        this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);

        isShow = true;
    }

}

void showLrcText::showlayout()
{
    init();
    show();
}

QFont showLrcText::getFont()
{
    QFont f;
    f.setPixelSize(80);
    return f;
}

