#include "readwin.h"
#include<QDebug>
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
    connect(ui->chap_win,&QListWidget::itemClicked,ui->myview,[=](QListWidgetItem *item){
        listItem* ziitem =static_cast<listItem*>(ui->chap_win->itemWidget(item));
        qDebug()<<ziitem->str;
        int row_n=ui->chap_win->row(item);
        qDebug()<<row_n;
        ui->pag_win->clear(); //清空页码的标签 pag_win
        mydata->refresh_page_data(ziitem->str,true);//ziitem->str 是章节名
        for(auto& i : mydata->page_list)
            fill_listwid(i,false); //填充page_win
        paint_img_nochap(); // 打印每一页
        //ui->myview->centerOn(grp_pix_list[row_n]);

    });

    //回主界面
    connect(ui->actionback,&QAction::triggered,[&](){
        emit backMainScene();
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

//参数可以获得漫画的标题
//这里是点击主界面的 title按钮，如果有章节就就填充chap_win
// 没有章节就就填充pag_win， 并且打印每一页
// 填充 chap_win 章目录这个控件(对于一些不含章目录的会直接 填充page_win)
void ReadWin::fill_chap_listwid(QListWidgetItem *item)
{
    ui->chap_win->clear();
    ui->pag_win->clear();
    show();
    qDebug()<<item->text()<<'\t'<<mydata->title_map[item->text()].ischapter;
    setWindowTitle(item->text());

    if(mydata->title_map[item->text()].ischapter){ //有 chapter
        mydata->refresh_chapter_data(item->text()); // 下载选中标题的数据
        for(auto& i:mydata->chap_list)
            fill_listwid(i,true);
    }
    else {
        mydata->refresh_page_data(item->text(),false);
        for(auto& i : mydata->page_list)
            fill_listwid(i,false);
        //直接打印图片
        paint_img_nochap();
    }
    //更新title表的num排序
    mydata->change_title_num(item->text());
    //下载数据库
    mydata->refresh_title_data();
}
//填充listwidget 元素是widget，
void ReadWin::fill_listwid(QString str,bool flag)
{
    if(flag) ui->pag_win->clear(); //如果只是切换章节，清掉pag_win // 切换漫画在前面已经全部清空了
    QListWidgetItem * fuItem = new QListWidgetItem;
    listItem *ziItem= new listItem(str,false);
    fuItem->setSizeHint(QSize(ziItem->width(),ziItem->height()));
    if(flag){
        // QListWidget *chap_win;
        ui->chap_win->addItem(fuItem); 
        ui->chap_win->setItemWidget(fuItem,ziItem);
    }
    else {
        ui->pag_win->addItem(fuItem);
        ui->pag_win->setItemWidget(fuItem,ziItem);
    }
}
void ReadWin::paint_img_nochap()
{
    scene->clear();
    int w,h;
    QPixmap pix(mydata->page_map[mydata->page_list[0]].page_path);
    w=pix.width();
    //h=pix.height();
    resize(w,500);
    h=0;
    for(int i=0;i<mydata->page_list.size();++i)
    //for(int i=0;i<page_path_list.size();++i)
    {
        //mydata->page_list[i] 获得页码string
        QString pt = mydata->page_map[mydata->page_list[i]].page_path;
        QPixmap pix(pt);
        //qDebug()<<file_list[i];
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
void ReadWin::paint_img_chap(const QString&chapter)
{
    scene->clear();
    ui->pag_win->clear(); //清空页码的标签 pag_win
    mydata->refresh_page_data(chapter,true);

    // 这时已经更新了chap_list，现在只有选中漫画的 page
    // 这时已经更新了page_map，page_map中只有 对应章节的的页
    //chap_list 中只有章节名

    //填充页码的标签 pag_win
    for(auto& i : mydata->page_list)
        fill_listwid(i,false);
    int w,h;
    QPixmap pix(mydata->page_map[mydata->chap_list[0]].page_path);
    w=pix.width();
    //h=pix.height();
    resize(w,500);
    h=0;
    for(int i=0;i<mydata->page_list.size();++i)
    //for(int i=0;i<page_path_list.size();++i)
    {
        QString pt = mydata->page_map[mydata->page_list[i]].page_path;
        QPixmap pix(pt);
        //qDebug()<<file_list[i];
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
    //add_chapter(path);
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


