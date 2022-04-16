#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<vector>
#include<iostream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::FramelessWindowHint);
    ui->dockWidget->setWindowFlags(Qt::FramelessWindowHint);

    init_graphics();  //初始化 grap
    get_comic_dir(); //获得路径
    paint_img();  //添加图片

    //test_img();
    num=0;
    connect(ui->pushButton_3,&QPushButton::clicked,this,[&](){
        ++num;
        //QString xxx = "漫画名 "+"当前页"+"/总页数";
        ui->statusbar->showMessage(file_list[num]);
    });
    connect(ui->pushButton_4,&QPushButton::clicked,this,[&](){
        qDebug()<<"滚轮顶端y坐标"<<view->verticalScrollBar()->value() ;
        qDebug()<<"滚轮左端端x坐标"<<view->horizontalScrollBar()->value();
        qDebug()<<"000000";
        //qDebug()<<"视图中心的坐标"<<(view->mapToScene(view->sceneRect())).x();
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::test_core()
{
    QList<QString> ls1{"hello","QT","CPP"};
    qDebug()<<ls1.at(1);
    qDebug()<<ls1[2];
}

void MainWindow::init_graphics()
{
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(this);
}

void MainWindow::get_comic_dir(QString path)
{
    dir.cd(path);
    QStringList sl=dir.entryList();
    for(auto &i:sl)
    {
        if(i=="."||i=="..") continue;
        file_list.append(path+"\\"+i);
    }
   // for(auto &i:file_list) qDebug()<<i;
}

void MainWindow::paint_img()
{
    int w,h;
    QPixmap pix(file_list[0]);
    w=pix.width();
    h=pix.height();
    resize(w,500);
    h=0;
    for(int i=0;i<20;++i)
    {
        QPixmap pix(file_list[i]);
        qDebug()<<file_list[i];
        QGraphicsPixmapItem * pixitem = new QGraphicsPixmapItem(pix);

        if(pix.width()<1500)
        {
            pixitem->setPos(0,h);
            h+=pix.height();
        }
        else
        {
            pixitem->setScale(0.5); //经历缩放后，图元的原点由左上角转到图元中心
            pixitem->setPos(0,h);
            h+=pix.height()/2;
        }
        scene->addItem(pixitem);
        grp_pix_list.append(pixitem);
    }
    view->setScene(scene);
    //view->centerOn(this->width()/2,this->height()/2);
    view->centerOn(0,0);//场景的的0，0绝对到不了视图的中心，但是这样可以避免菜单工具栏的误差，保证场景视图左上角重合
    setCentralWidget(view);
}

void MainWindow::test_img()
{
    resize(1200,1600);
    QPixmap pix1(file_list[0]);
    QPixmap pix2(file_list[1]);
    QGraphicsPixmapItem * pixitem1 = new QGraphicsPixmapItem(pix1);
    QGraphicsPixmapItem * pixitem2 = new QGraphicsPixmapItem(pix2);
    pixitem1->setPos(0,0);
    pixitem2->setPos(0,1600);
    scene->addItem(pixitem1);
    scene->addItem(pixitem2);
    view->setScene(scene);
    //view->centerOn(0,0);
    view->centerOn(this->width()/2,this->height()/2);
    setCentralWidget(view);
}


