#include "readwin.h"


#pragma execution_character_set("utf-8")

ReadWin::ReadWin(QWidget *parent)
    : QMainWindow{parent},ui(new Ui::ReadWin)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::FramelessWindowHint);
    init_ui();  //初始化 ui,和一些成员
    get_comic_dir(R"(E:\Qt\projuct\ComicByCPP\comic\魔王勇者\第1话)"); //获得路径
    paint_img();  //添加图片
    //是否隐藏 章节页 属性栏
    connect(ui->actionDock,&QAction::triggered,this,[&](){
        if(!isShow) //没有显示，则显示
        {
        isShow=true;
        ui->zhanglist->show();
        }
        else
        {
            isShow=false;
            ui->zhanglist->hide();
        }
    });
    //工具栏的open按钮
    connect(ui->actionopen,&QAction::triggered,this,[&](){
       qDebug()<<"this is actionopen";
    });
    //listwidget 的信号槽  //点击切换页面
    connect(ui->listWidget,&QListWidget::itemClicked,ui->myview,[&](QListWidgetItem *item){
        qDebug()<<item->text()<<endl;
        int row_n=ui->listWidget->row(item);
         qDebug()<<row_n;
        ui->myview->centerOn(grp_pix_list[row_n]);

    });
    //回主界面
    connect(ui->actionback,&QAction::triggered,[&](){
        emit backMainScene();
        hide();
    });
}

ReadWin::~ReadWin()
{
    delete ui;
}

void ReadWin::init_ui()
{
    isShow=false;
    ui->zhanglist->hide();
    scene = new QGraphicsScene(this);
}

void ReadWin::get_comic_dir(QString path)
{

    dir.cd(path);
    QStringList sl=dir.entryList();
    for(auto &i:sl)
    {
        //qDebug()<<i;
        if(i=="."||i=="..") continue;
        file_list.append(path+"\\"+i);
        ui->listWidget->addItem(i);
    }
}

void ReadWin::paint_img()
{
    int w,h;
    QPixmap pix(file_list[0]);
    w=pix.width();
    //h=pix.height();
    resize(w,500);
    h=0;
    for(int i=0;i<7;++i)
    {
        QPixmap pix(file_list[i]);
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

void ReadWin::mousePressEvent(QMouseEvent *event)
{
    if(isShow)
    {
        isShow=false;
        ui->zhanglist->hide();
     }

}
