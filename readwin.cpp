#include "readwin.h"
#include<QDebug>
#include <QToolBar>
#include <QToolBar>
#include"listitem.h"
#include"datamanager.h"

#pragma execution_character_set("utf-8")

shared_ptr<DataManager> mydata = nullptr;

ReadWin::ReadWin(QWidget *parent)
    : QMainWindow{parent},ui(new Ui::ReadWin)
{
    ui->setupUi(this);
    //mydata = DataManager::instance_db(); //实例数据库
    qDebug()<<"this is ReadWin";
    ui->direct_win->setMaximumWidth(75);
    ui->direct_win->sizeHint();
    //setWindowFlags(Qt::FramelessWindowHint);
    //初始化 ui,和一些成员
    isShow=false;
    ui->direct_win->hide();
    scene = new QGraphicsScene(this);
    connect(ui->actionDock,&QAction::triggered,this,[&](){
        if(!isShow) //没有显示，则显示
        {
            isShow=true;
            ui->direct_win->show();
        }
        else
        {
            isShow=false;
            ui->direct_win->hide();
        }
    });
    //工具栏的open按钮 添加文件夹，即漫画名的路径
    connect(ui->actionopen,&QAction::triggered,this,&ReadWin::open_folder);
    //listwidget 的信号槽  //点击切换页面
    connect(ui->chap_win,&QListWidget::itemClicked,this,&ReadWin::fill_pageList_fromChap);

    //回主界面
    connect(ui->actionback,&QAction::triggered,[&](){
        emit backMainScene();
        scene->clear();
        hide();
    });
    //显示当前漫画名列表
    connect(ui->actiondis_data,&QAction::triggered,this,&ReadWin::dis_data);

}

ReadWin::~ReadWin()
{
    qDebug()<<"readwin被关闭";
    delete ui;
}

void ReadWin::fill_chapList(QListWidgetItem *item)
{
    show();
    setWindowTitle(item->text());
    ui->chap_win->clear();
    ui->pag_win->clear();
    mydata->refresh_chapter_data(item->text()); //下载章节信息
    for(auto& i:mydata->chap_map)
    {
        QListWidgetItem * fuItem = new QListWidgetItem;
        listItem *ziItem= new listItem(i.first,true);
        fuItem->setSizeHint(QSize(ziItem->width(),ziItem->height()));
        ui->chap_win->addItem(fuItem);
        ui->chap_win->setItemWidget(fuItem,ziItem);
    }

}
void ReadWin::fill_pageList_fromTitle(QListWidgetItem *item)
{
    show();
    setWindowTitle(item->text());
    ui->chap_win->clear();
    ui->pag_win->clear();
    mydata->down_pageData_byTitle(item->text()); //下载指定的漫画的page到mydata->page_map中
    fill_pageList();
    //直接打印图片
    paint_img();
}
void ReadWin::fill_pageList_fromChap(QListWidgetItem *item)
{
    auto temp =ui->chap_win->itemWidget(item);
    listItem* child_item = static_cast<listItem*>(temp);
    mydata->down_pageData_byChap(child_item->name);//下载指定的章节的page到mydata->page_map中
    fill_pageList();
    paint_img();
}
void ReadWin::fill_pageList()
{
    ui->pag_win->clear();
    //mydata->page_map 只含有 选中章节的page信息
    for(auto& i:mydata->page_map)
    {
        QListWidgetItem * fuItem = new QListWidgetItem;
        listItem *ziItem= new listItem(i.first,true);
        fuItem->setSizeHint(QSize(ziItem->width(),ziItem->height()));
        ui->pag_win->addItem(fuItem);
        ui->pag_win->setItemWidget(fuItem,ziItem);
    }
}

void ReadWin::paint_img()
{
    scene->clear();
    int w,h;
    QPixmap pix(mydata->page_map.begin()->second.page_path);
    w=pix.width();
    //h=pix.height();
    resize(w,500);
    h=0;
    for(auto &pa:mydata->page_map)
    {
        //mydata->page_list[i] 获得页码string
        QString pt = mydata->page_map.begin()->second.page_path;
        QPixmap pix(pt);
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
    ui->myview->setScene(scene);
    //ui->myview->centerOn(this->width()/2,this->height()/2);
    ui->myview->centerOn(0,0);//场景的的0，0绝对到不了视图的中心，但是这样可以避免菜单工具栏的误差，保证场景视图左上角重合
}

void ReadWin::get_item()
{
    int y=ui->myview->verticalScrollBar()->value();
    //auto item=ui->myview->itemAt(100,y);
    auto item=ui->myview->itemAt(100,100);
    QGraphicsPixmapItem *pixitem=static_cast<QGraphicsPixmapItem*>(item);
    qDebug()<<"当前页数是"<<grp_pix_list.indexOf(pixitem);
    qDebug()<<"图元的尺寸"<<pixitem->pixmap().size();
    qDebug()<<"滚轮顶端y坐标"<<y ;
    qDebug()<<"滚轮左端端x坐标"<<ui->myview->horizontalScrollBar()->value();
    QString str =QString("当前页数是%1,图元的尺寸%2,%3")
            .arg(grp_pix_list.indexOf(pixitem))
            .arg(pixitem->pixmap().size().width())
            .arg(pixitem->pixmap().size().height());
    ui->statusbar->showMessage(str);

}

void ReadWin::open_folder()
{
    QString path = QFileDialog::getExistingDirectory(this,"打开漫画文件夹",
                                                     R"(E:\Qt\projuct\ComicByCPP)");
    if(path=="") return;
    qDebug()<<"路径是 "<<path;

    // 上传数据库
    mydata->add_title(path);

    // 下载数据库
    mydata->refresh_title_data();
    // 显示漫画
    emit sort_title_num();

}

void ReadWin::dis_data()
{
    mydata->refresh_title_data();
    auto iter = mydata->title_map.cbegin();
    qDebug()<<"显示title表";
    while(iter != mydata->title_map.cend())
    {
        qDebug()<<iter.key()<<'\t'<<iter.value().title_path
               <<'\t'<<iter.value().ischapter<<'\t'
              <<iter.value().progress<<'\t'
             <<iter.value().num;
        ++iter;
    }
}
void ReadWin::mousePressEvent(QMouseEvent *event)
{
    if(isShow)
    {
        isShow=false;
        ui->direct_win->hide();
    }
}



