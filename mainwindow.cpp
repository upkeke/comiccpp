#include "mainwindow.h"
#include "ui_mainwindow.h"

#pragma execution_character_set("utf-8")


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),readwin(new ReadWin(this))
{
    ui->setupUi(this);
    qDebug()<<"this mainwin";
    refresh_title_data();
    ui->listWidget->addItems(title_list);
    connect(ui->listWidget,&QListWidget::itemDoubleClicked,readwin
            ,[&](QListWidgetItem *item){
        hide();
        readwin->fill_diect_win(item);
    });
    //进入子窗口
    connect(ui->pushButton,&QPushButton::clicked,readwin,[&](){
        readwin->show();
        hide();
    });
    //监听子窗口的信号，判断自己是否显示
    connect(readwin,&ReadWin::backMainScene,this,&MainWindow::show);
}
MainWindow::~MainWindow()
{
    delete ui;
    db.close();
    qDebug()<<"数据库关闭";
}
//test 按钮
void MainWindow::on_pushButton_2_clicked()
{
    QString order = QString("select * from ComicTitle;");
    if(!query.exec(order))
        qDebug()<<"defect"<<db.lastError();
    else{
        qDebug()<<"空";
        while(query.next()){
            qDebug()<<query.value("title").toString()
                   <<'\t'<<query.value("absolute_path").toString()
                  <<'\t'<<query.value("ischapter").toInt();
        }
    }
}
//插入
void MainWindow::on_pushButton_3_clicked()
{
    QString add_data = QString("insert into title(title,"
                               "title_path,ischapter)"
                               " values('监狱学园','c:/vv',1)");
    if(!query.exec(add_data))
        qDebug()<<"插入失败";
}

