#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    VideoToolLayout *vv = new VideoToolLayout(this);
    vv->showLayout();
}

MainWindow::~MainWindow()
{
    delete ui;
}

