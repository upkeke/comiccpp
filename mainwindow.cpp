#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include"readwin.h"


#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),readwin(new ReadWin(this))
{
    ui->setupUi(this);
    qDebug()<<"this mainwin";
    mydata = DataManager::instance_db(); //实例数据库
    mydata->refresh_title_data();  //刷新标题的数据
    fill_title_listwid();
    // 双击打开打开漫画
    connect(ui->listWidget,&QListWidget::itemDoubleClicked,readwin
            ,[&](QListWidgetItem *item){
        hide();
        readwin->fill_chap_listwid(item);
        fill_title_listwid(); // 子所以要再次填充书架，原因在于点击了当前漫画，需要把漫画拍到首位

    });

    // 打开文件
    connect(ui->actionopen,&QAction::triggered,readwin,[&](){
        readwin->open_folder();
    });

    //监听子窗口的信号，判断自己是否显示
    connect(readwin,&ReadWin::backMainScene,this,&MainWindow::show);
    //监听 子类如果添加新漫画，修改listWidget排序
    connect(readwin,&ReadWin::sort_title_num,this,&MainWindow::fill_title_listwid);
    // 删除漫画
    connect(ui->del_comic,&QPushButton::clicked,this,[&](){
        auto *item = ui->listWidget->currentItem();
        QString title = item->text();
        mydata->delete_comic(title);
        ui->listWidget->takeItem(ui->listWidget->currentRow());
    });

    //t1是成员变量 类型 QThread*
    t1 = new QThread(this);
    Worker *worker = new Worker();

    worker->moveToThread(t1); // 把worker 放到子线程t1中
    //点击按钮开始子线程任务
    connect(this,&MainWindow::begin_work,worker,&Worker::dowork);
    // 发射信号 begin_work
    connect(ui->Thread_test,&QPushButton::clicked,this,[=](){
        qDebug()<< "点击按钮 "<<ui->Thread_test->text();
        t1->start();
        emit begin_work(100);
    });
    //delete worker 在子线程finished后
    connect(t1,&QThread::finished,worker,[=](){
        qDebug()<<"worker 被销毁";
        worker->deleteLater(); //释放
    });
    connect(ui->Thread_end,&QPushButton::clicked,this,[=](){
        qDebug()<< "点击按钮 "<<ui->Thread_test->text();
        if(t1->isFinished())
            qDebug()<<"子线程结束";
        else
            qDebug()<<"子线程没有结束";
        qDebug()<<__FUNCTION__
               <<"线程id "<<(int)QThread::currentThreadId();
        t1->quit();
        t1->wait();
        qDebug()<<"+++++++++++++++++++++++++++++++++++++++++++";
        if(t1->isFinished())
            qDebug()<<"子线程结束";
        else
            qDebug()<<"子线程没有结束";
        qDebug()<<__FUNCTION__
               <<"线程id "<<(int)QThread::currentThreadId();
        qDebug()<<"over";
    });

}
MainWindow::~MainWindow()
{
    qDebug()<<"\n主窗口被关闭";
    //delete readwin;
    delete ui;
    qDebug()<<"数据库关闭";
    t1->quit();
    t1->wait();
    qDebug()<<"子线程 结束";
    qDebug()<<"MainWindow 析构完毕";

}

void MainWindow::fill_title_listwid()
{
    ui->listWidget->clear();
    for(auto & i:mydata->title_list)
    {
        qDebug()<<i;
        ui->listWidget->addItem(i);
    }
}


