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
    connect(ui->chap_win,&QListWidget::itemDoubleClicked,this,&ReadWin::fill_pageList_fromChap);

    //回主界面
    connect(ui->actionback,&QAction::triggered,[&](){
        emit backMainScene();
        //ui->pag_win->clear();
        pixmapItems.clear();
        scene->clear();
        hide();
    });
}

ReadWin::~ReadWin()
{
    qDebug()<<"readwin被关闭";
    delete ui;
}

void ReadWin::fill_chapList(const QString& title)
{
    setWindowTitle(title);
    current_title = title;

    int last_chaper_count = ui->chap_win->count();
    ui->pag_win->clear();
    mydata->refresh_chapter_data(title); //下载章节信息

    int current_chaper_count = mydata->chap_map.size();
    qDebug()<<"当前的漫画章节数1 "<<last_chaper_count<<"选中漫画章节数"<<current_chaper_count;
    // 这一步大费周章的原因，由于章节的都是第一章，第二章。。。。 如果list窗口已经有这些，
    //不必把他们全部析构调然后重新生成，只需要把多余的部分析构调，缺少的部分补充，
    //保证chapter_count和current_chaper_count同步
    if(last_chaper_count>current_chaper_count)
    {
        while (last_chaper_count>current_chaper_count) {
            ui->chap_win->takeItem(last_chaper_count-1);
            --last_chaper_count;
        }
    }
    else
    {
        for(;last_chaper_count<current_chaper_count;)
        {
            QString str = QString("第%1章").arg(last_chaper_count+1);
            QListWidgetItem * fuItem = new QListWidgetItem;
            listItem *ziItem= new listItem(str,true);
            fuItem->setSizeHint(QSize(ziItem->width(),ziItem->height()));
            ui->chap_win->addItem(fuItem);
            ui->chap_win->setItemWidget(fuItem,ziItem);
            last_chaper_count++;
        }
    }
    qDebug()<<"当前的漫画章节数2 "<<last_chaper_count<<"选中漫画章节数"<<current_chaper_count;

}
void ReadWin::fill_pageList_fromTitle(const QString& title)
{

    setWindowTitle(title);
    ui->chap_win->clear(); //没有章节目录，清空
    //ui->pag_win->clear();
    mydata->down_pageData_byTitle(title); //下载指定的漫画的page到mydata->page_map中
    fill_pageList();
    //直接打印图片
    display_page();
}
void ReadWin::fill_pageList_fromChap(QListWidgetItem *item)
{

    auto temp = ui->chap_win->itemWidget(item);
    listItem* child_item = static_cast<listItem*>(temp);
    mydata->down_pageData_byChap(child_item->name);//下载指定的章节的page到mydata->page_map中
    current_chapter = child_item->name;
    fill_pageList();
    display_page();
}
void ReadWin::fill_pageList()
{
    int current_page_count = mydata->page_map.size();
    int last_page_count = ui->pag_win->count();
    qDebug()<<"当前选中章节页数1 "<<last_page_count<<"选中章节页数"<<current_page_count;
    // 这一步大费周章的原因，由于章节的都是第一章，第二章。。。。 如果list窗口已经有这些，
    //不必把他们全部析构调然后重新生成，只需要把多余的部分析构调，缺少的部分补充，
    //保证chapter_count和current_chaper_count同步
    if(last_page_count>current_page_count)
    {
        while (last_page_count>current_page_count) {
            ui->pag_win->takeItem(last_page_count-1);
            --last_page_count;
        }
    }
    else
    {
        for(;last_page_count<current_page_count;)
        {
            QString str = QString("第%1页").arg(last_page_count+1);
            QListWidgetItem * fuItem = new QListWidgetItem;
            listItem *ziItem= new listItem(str,true);
            fuItem->setSizeHint(QSize(ziItem->width(),ziItem->height()));
            ui->pag_win->addItem(fuItem);
            ui->pag_win->setItemWidget(fuItem,ziItem);
            last_page_count++;
        }
    }
    qDebug()<<"当前的选中章节页数2 "<<last_page_count<<"选中章节页数"<<current_page_count;

}
void ReadWin::display_page()
{
    qDebug()<<"开始打印每一页";
    int last_page_count = pixmapItems.size();
    int h = 0;
    int current_page_count = mydata->page_map.size();
    auto iter =  mydata->page_map.cbegin();
    for(int i=0;i<current_page_count;++i)
    {
        QPixmap pix(iter->second.page_path);
        if(i<last_page_count){
            pixmapItems[i]->setPixmap(pix);
        }
        else {
            QGraphicsPixmapItem * pixitem = new QGraphicsPixmapItem(pix);
            pixmapItems.push_back(pixitem);
            scene->addItem(pixitem);
        }
        pixmapItems[i]->setPos(0,h);
        ++iter;
        h+=pix.height();
    }
    for(int i =last_page_count-1 ;i>=current_page_count;--i)
    {
        scene->removeItem(pixmapItems[i]);
        pixmapItems.removeLast();
    }
    ui->myview->setScene(scene);

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

