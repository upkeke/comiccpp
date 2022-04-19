#include "mainwindow.h"
#include "ui_mainwindow.h"

#pragma execution_character_set("utf-8")

QSqlDatabase db; //定义数据库
QSqlQuery query;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),childwin(new ReadWin(this))
{
    ui->setupUi(this);
    //连接数据库
    db = QSqlDatabase::addDatabase("QSQLITE");
    //db.setDatabaseName(":/data/datacc.db");
    //file:///E:/Qt/projuct/ComicByCPP/data/data.db
    //db.setDatabaseName("file:///E:/Qt/projuct/ComicByCPP/data/comicdata.db");
    db.setDatabaseName("datacc.db");
    if(!db.open()) qDebug()<<"open error---"<<db.lastError();

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
void MainWindow::on_pushButton_2_clicked()
{
#if 0
    QString create =("CREATE TABLE ComicTitle (title TEXT,"
                     "absolute_path	TEXT,ischapter	INTEGER DEFAULT 1,"
                     "PRIMARY KEY(title))");
    if(!query.exec(create))
        qDebug()<<"创建失败"<<db.lastError();
#endif
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


void MainWindow::on_pushButton_3_clicked()
{
    QString add_data = QString("insert into ComicTitle(title,"
                               "absolute_path,ischapter)"
                               " values('监狱学园','c:/vv',1)");
    if(!query.exec(add_data))
        qDebug()<<"插入失败";

}

