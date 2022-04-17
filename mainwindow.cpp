#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<vector>
#include<iostream>

#pragma execution_character_set("utf-8")
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::FramelessWindowHint);
    ui->dockWidget->setWindowFlags(Qt::FramelessWindowHint);

    init_ui();  //初始化 ui,和一些成员
    get_comic_dir(); //获得路径
    paint_img();  //添加图片

    //test_img();
    num=0;
    connect(ui->pushButton_3,&QPushButton::clicked,this,[&](){
        ++num;
        //QString xxx = "漫画名 "+"当前页"+"/总页数";
        ui->statusbar->showMessage(file_list[num]);
    });
    connect(ui->pushButton_4,&QPushButton::clicked,this,&MainWindow::get_item);

    //设置 "设置"图标的点击事件
    connect(ui->actionset_dock,&QAction::triggered,this,[&](){
        if(!isShow) //没有显示，则显示
        {
        ui->dockWidget->show();
        isShow=true;
        }
        else
        {
            ui->dockWidget->hide();
            isShow=false;
        }
    });
    //
    connect(ui->actionopen,&QAction::triggered,this,[&](){
        ui->toolBox_2->hide();
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

void MainWindow::init_ui()
{
    isShow=false;
    myDock1=new QDockWidget(this,Qt::FramelessWindowHint);
    addDockWidget(Qt::RightDockWidgetArea,myDock1);
    myDock1->hide();
    //myDock1->setWindowFlag(Qt::RightDockWidgetArea);
    ui->dockWidget->hide();
    ui->dockWidget->setAllowedAreas(Qt::RightDockWidgetArea);//设置只允许靠右
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
    //h=pix.height();
    resize(w,500);
    h=0;
    for(int i=0;i<3;++i)
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
    setCentralWidget(ui->widget);
}

void MainWindow::get_item()
{
    int y=view->verticalScrollBar()->value();
    //auto item=view->itemAt(100,y);
    auto item=view->itemAt(100,100);
    QGraphicsPixmapItem *pixitem=static_cast<QGraphicsPixmapItem*>(item);
    qDebug()<<"当前页数是"<<grp_pix_list.indexOf(pixitem);
    qDebug()<<"图元的尺寸"<<pixitem->pixmap().size();
    qDebug()<<"滚轮顶端y坐标"<<y ;
    qDebug()<<"滚轮左端端x坐标"<<view->horizontalScrollBar()->value();
    QString str =QString("当前页数是%1,图元的尺寸%2,%3")
            .arg(grp_pix_list.indexOf(pixitem))
            .arg(pixitem->pixmap().size().width())
            .arg(pixitem->pixmap().size().height());
    ui->statusbar->showMessage(str);

}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(isShow)
    {
        ui->dockWidget->hide();
        isShow=false;
    }

}



