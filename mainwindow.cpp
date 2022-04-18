#include "mainwindow.h"
#include "ui_mainwindow.h"

#pragma execution_character_set("utf-8")
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),childwin(new ReadWin(this)),wintest(new mainwintest(this))
{
    ui->setupUi(this);
    wintest->show();
    //进入子窗口
    connect(ui->pushButton,&QPushButton::clicked,childwin,[&](){
        childwin->show();
        hide();
    });
    //监听子窗口的信号，判断自己是否显示
    connect(childwin,&ReadWin::backMainScene,this,&MainWindow::show);
}
MainWindow::~MainWindow()
{
    delete ui;
}



