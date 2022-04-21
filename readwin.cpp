#include "readwin.h"

#pragma execution_character_set("utf-8")

QSqlDatabase db; //
QSqlQuery query; //执行sql指令
QMap<QString,Title> title_map;
QStringList title_list;

void refresh_title_data()
{
    title_list.clear();
    title_map.clear();
    QString order = QString("select *from title order by num");
    if(!query.exec(order))
        qDebug()<<"获取失败"<<db.lastError();
    else{
        qDebug()<<"获取成功";
        while(query.next()){
            //漫画名的目录
            QString title =query.value("title").toString();
            QString title_path =query.value("title_path").toString();
            int ischapter =query.value("ischapter").toInt();
            float progress =query.value("progress").toFloat();
            int chap_count =query.value("chap_count").toInt();
            int num =query.value("num").toInt();
            //qDebug()<<title<<" "<<title_path<<" "<<ischapter;
            title_map[title]= Title(title,title_path,ischapter,progress,chap_count,num);
            title_list.push_back(title);
        }
    }
}

bool comp(const QString &str1, const QString &str2)
{
    if (str1.size() < str2.size()) return true;
        else if(str1.size() == str2.size())
        {
            for (int i = 0; i < str2.size(); ++i)
            {
                auto &x = str1[i];
                auto &y = str2[i];
                if (x < y)return true;
                else if (x == y) continue;
                else return false;
            }
            return false;
        }
        else return false;
}

ReadWin::ReadWin(QWidget *parent)
    : QMainWindow{parent},ui(new Ui::ReadWin)
{
    ui->setupUi(this);
    title_list.push_back(" "); // 添加一个空值，确保index和num一致
    qDebug()<<"this is ReadWin";
    db = QSqlDatabase::addDatabase("QSQLITE");
    //db.setDatabaseName(":/data/data.db");
    //file:///E:/Qt/projuct/ComicByCPP/data/data.db
    //db.setDatabaseName("file:///E:/Qt/projuct/ComicByCPP/data/comicdata.db");
    db.setDatabaseName("data.db");

    if(!db.open()) qDebug()<<"open error **"<<db.lastError();
    else qDebug()<<"连接成功";
    query = QSqlQuery();
    ui->direct_win->setMaximumWidth(75);

    ui->direct_win->sizeHint();
    //setWindowFlags(Qt::FramelessWindowHint);
    //初始化 ui,和一些成员
    isShow=false;
    ui->direct_win->hide();
    scene = new QGraphicsScene(this);

    //获取数据
    refresh_title_data();

    //get_comic_dir(R"(E:\Qt\projuct\ComicByCPP\comic\魔王勇者\第1话)"); //获得路径
    //get_comic_dir(R"(E:\Qt\projuct\ComicByCPP\comic\监狱学园\1)");
    //paint_img();  //添加图片
    //是否隐藏 章节页 属性栏
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
    connect(ui->chap_win,&QListWidget::itemClicked,ui->myview,[&](QListWidgetItem *item){
        qDebug()<<item->text()<<endl;
        int row_n=ui->pag_win->row(item);
         qDebug()<<row_n;
        ui->myview->centerOn(grp_pix_list[row_n]);

    });
    //回主界面
    connect(ui->actionback,&QAction::triggered,[&](){
        emit backMainScene();
        hide();
    });
    //添加章节目录
    //connect(ui->actionBar,&QAction::triggered,this,&ReadWin::add_chapter);

    //显示当前漫画名列表
    connect(ui->actiondis_data,&QAction::triggered,this,&ReadWin::dis_data);

}

ReadWin::~ReadWin()
{
    delete ui;
}

void ReadWin::fill_diect_win(QListWidgetItem *item)
{
    show();
    qDebug()<<item->text()<<'\t'<<title_map[item->text()].ischapter;

    QListWidgetItem * firItem ;
    listItem *endItem;
    if(title_map[item->text()].ischapter) //有 chapter
    {
        refresh_chapter_data(item->text());
        auto chap_iter = chap_map.cbegin();
        while(chap_iter!=chap_map.cend())
        {
            firItem = new QListWidgetItem;
            endItem= new listItem(chap_iter.value().chapter,false);
            firItem->setSizeHint(QSize(endItem->width(),endItem->height()));
            ui->chap_win->addItem(firItem);
            ui->chap_win->setItemWidget(firItem,endItem);
            ++chap_iter;
        }
    }

    else {
        auto chap_iter = chap_map.cbegin();
        while(chap_iter!=chap_map.cend())
        {
            firItem = new QListWidgetItem;
            endItem= new listItem(chap_iter.value().chapter,false);
            firItem->setSizeHint(QSize(endItem->width(),endItem->height()));
            ui->pag_win->addItem(firItem);
            ui->pag_win->setItemWidget(firItem,endItem);
            ++chap_iter;
        }
    }
}
void ReadWin::paint_img()
{
    int w,h;
    QPixmap pix(page_path_list[0]);
    w=pix.width();
    //h=pix.height();
    resize(w,500);
    h=0;
    for(int i=0;i<7;++i)
    {
        QPixmap pix(page_path_list[i]);
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
    // 上传数据库
    add_title(path);
    //add_chapter(path);
    // 下载数据库
    refresh_title_data();
    // 显示漫画


}
//填充listwidget 元素是widget， 方法二
void ReadWin::fill_child_list(QString str,bool flag)
{
    if(flag)
    {
        QListWidgetItem * firItem = new QListWidgetItem;
        listItem *endItem= new listItem(str,false);
        firItem->setSizeHint(QSize(endItem->width(),endItem->height()));
        ui->chap_win->addItem(firItem);
        ui->chap_win->setItemWidget(firItem,endItem);
    }
    else {
        QListWidgetItem * firItem = new QListWidgetItem;
        listItem *endItem= new listItem(str,false);
        firItem->setSizeHint(QSize(endItem->width(),endItem->height()));
        ui->pag_win->addItem(firItem);
        ui->pag_win->setItemWidget(firItem,endItem);
    }


}
// 获得 表title 的信息
/*
void ReadWin::refresh_title_data()
{
    title_list.clear();
    title_map.clear();
    QString order = QString("select *from title order by num");
    if(!query.exec(order))
        qDebug()<<"获取失败"<<db.lastError();
    else{
        qDebug()<<"获取成功";
        while(query.next()){
            //漫画名的目录
            QString title =query.value("title").toString();
            QString title_path =query.value("title_path").toString();
            int ischapter =query.value("ischapter").toInt();
            float progress =query.value("progress").toFloat();
            int chap_count =query.value("chap_count").toInt();
            int num =query.value("num").toInt();
            //qDebug()<<title<<" "<<title_path<<" "<<ischapter;
            title_map[title]= Title(title,title_path,ischapter,progress,chap_count,num);
            title_list.push_back(title);
        }
    }
}
*/
// 获得 表chapter 的信息
void ReadWin::refresh_chapter_data(const QString &title)
{
    chap_list.clear();
    chap_map.clear();
    QString order = QString("select *from chapter where title = '%1'").arg(title);
    if(!query.exec(order))
        qDebug()<<"获取失败"<<db.lastError();
    else{
        qDebug()<<"获取成功";
        while(query.next()){
            //漫画名的目录
            QString title =query.value("title").toString();
            QString chapter =query.value("chapter").toString();
            QString chap_path =query.value("chap_path").toString();
            int page_count =query.value("page_count").toInt();
            //qDebug()<<title<<" "<<title_path<<" "<<ischapter;
            chap_map[chapter]= Chapter(title,chapter,chap_path,page_count);
            chap_list.push_back(title);
        }
    }
    // 对chap_list重排
    auto fun = [](const QString&str1,const QString&str2)
    {
        if (str1.size() < str2.size()) return true;
            else if(str1.size() == str2.size())
            {
                for (int i = 0; i < str2.size(); ++i)
                {
                    auto &x = str1[i];
                    auto &y = str2[i];
                    if (x < y)return true;
                    else if (x == y) continue;
                    else return false;
                }
                return false;
            }
            else return false;
    };
    sort(chap_list.begin(),chap_list.end(),fun);
}
// 下载 表page 的信息
void ReadWin::refresh_page_data(const QString &chapter)
{
    chap_list.clear();
    chap_map.clear();
    QString title =chap_map[chapter].title;
    //QString chap_path =chap_map[chapter].chap_path;
    QString order = QString("select *from page "
                            "where [title = '%1'] and [chapter = '%2']")
            .arg(title,chapter);
    if(!query.exec(order))
        qDebug()<<"获取失败"<<db.lastError();
    else{
        qDebug()<<"获取成功";
        while(query.next()){
            //page名的目录
            QString title =query.value("title").toString();
            QString chapter =query.value("chapter").toString();
            QString page =query.value("page").toString();
            QString page_path =query.value("page_path").toString();
            //int page_num =query.value("page_num").toInt();
            //qDebug()<<title<<" "<<title_path<<" "<<ischapter;
            page_map[page]= Page(title,chapter,page,page_path);
            page_list.push_back(page);
        }
    }
    // 对page_list重排
    sort(page_list.begin(),page_list.end(),comp);
}
//漫画名：title（Key） / 漫画路径：title_path,是否子文件夹：ischapter
//章数 chap_count,/ 阅读进度：progress,/ 主窗口的位次：num
void ReadWin::add_title(const QString & path)
{
//    QString path = QFileDialog::getExistingDirectory(this,"打开漫画文件夹",
//                                                R"(E:\Qt\projuct\ComicByCPP)");
    //qDebug()<<path;
    QFileInfo title_info(path);
    //qDebug()<<title_info.baseName();
    //判断该漫画是否已经存在
    //如果已存在，则主界面依然要添加，但是要添加后缀
    QString title= title_info.baseName();
    auto contain_inter = title_map.find(title);
    if(contain_inter!=title_map.end())
    {
        int num=contain_inter.value().num;
        QString change_num_order = QString("update title set num = 0 "
                                           "where num = %1;").arg(num);
        if(!query.exec(change_num_order)) qDebug()<<"change_num1 失败"<<db.lastError();
        else qDebug()<<"change_num1 成功";

        change_num_order = QString("update title set num "
                                    "= replace (num,num,num+1) "
                                    "where num < %1;").arg(num);
        if(!query.exec(change_num_order)) qDebug()<<"change_num2 失败"<<db.lastError();
        else qDebug()<<"change_num2 成功";
        return;
    }
    QDir dir(path);
    dir.setFilter(QDir::Dirs|QDir::NoDotAndDotDot|QDir::NoDot);
    //dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();
    int ischapter;
    int chap_count;
    if(list.size()!=0) //如果是漫画文件夹下面是也是文件夹，即含有章节分支
    {
        ischapter = 1;
    }
    else {
        dir.setFilter(QDir::Files|QDir::NoDotAndDotDot|QDir::NoDot);
        QStringList namefilters;
        namefilters<<"*jpg"<<"*bmp"<<"*png"<<"jpeg";
        dir.setNameFilters(namefilters);
        list = dir.entryInfoList();
        if (list.size()!=0) //漫画文件夹下面直接是jpg
        {
            ischapter = 0;
        }
        else return;
    }
    chap_count =list.size();
    QString insert_ComicTitle = QString("insert into title(title,"
                                        "title_path,ischapter,progress,chap_count"
                                        ",num)values('%1','%2',%3,%4,%5,%6);")
            .arg(title_info.baseName(),path,QString::number(ischapter)
                 ,QString::number(0.0),QString::number(chap_count)
                 ,QString::number(title_map.size()+1));
    if(!query.exec(insert_ComicTitle)) qDebug()<<"add 失败"<<db.lastError();
    else qDebug()<<"add 成功";

    //将漫画下的章节的传入数据库
    add_chapter(path,title,ischapter);
}
//表chapter的数据写入（含部分表page的数据写入）
//漫画名：title,/ 章节：chapter(Key),/ 章节路径：chap_path,
//页数：page_count,/备注，如果没有章节，则数据插入表page中对应的页数
void ReadWin::add_chapter(const QString & title_path,const QString &title,bool fa)
{
    //auto mytitle = title_map[title_list[3]];
   //QString title_path = R"(E:\Qt\projuct\ComicByCPP\comic\魔王勇者)"; //\第1话
    QDir dir(title_path);
    QString chapter,chap_path,order;
    int page_count; //第几章
    QFileInfoList list;
    //查看 这个目录下是否有子目录，还是全是jpg
    if(fa)
    {
        dir.setFilter(QDir::Dirs|QDir::NoDot|QDir::NoDotAndDotDot);
        list = dir.entryInfoList();
        for(auto &i : list)
        {
            chapter = i.baseName();  //章节名
            chap_path = i.absoluteFilePath() ; //章节路径
            page_count = QDir(chap_path).entryInfoList().size();
            order =QString("insert into chapter("
                           "title,chapter,chap_path,"
                           "page_count) values"
                           "('%1','%2','%3',%4)")
                    .arg(title,chapter,chap_path,QString::number(page_count));
            //qDebug()<<title<<" "<<chapter<<" "<<chap_path<<" "<<page_count;

            if(!query.exec(order)) qDebug()<<"add chap page 失败"<<db.lastError();

            //章节下的的传入数据库
            add_page(title,chapter,chap_path);
        }
        qDebug()<<"chapter------";

    }
    else {
        dir.setFilter(QDir::Files|QDir::NoDot|QDir::NoDotAndDotDot);
        QStringList namefilters;
        namefilters<<"*jpg"<<"*bmp"<<"*png"<<"jpeg";
        dir.setNameFilters(namefilters);
        list = dir.entryInfoList();
        for(auto &i : list)
        {
            chapter = i.baseName();  //这里的chapter,chap_path 代指page,page_pat
            chap_path = i.absoluteFilePath() ;  //方便少声明变量
            order =QString("insert into page("
                           "title,page,page_path) values"
                           "('%1','%2','%3')")
                    .arg(title,chapter,chap_path);
            //qDebug()<<title<<" "<<comic_page<<" "<<page_path<<" "<<numx;
            if(!query.exec(order)) qDebug()<<"add chap page 失败"<<db.lastError();  
        }
        qDebug()<<"page-----";
        //insert into page(title,comic_page,page_path,index ) values('献上祝福','0','E:/Qt/projuct/ComicByCPP/comic/魔王勇者/第1话/0.jpg',0);
    }


}

void ReadWin::add_page(const QString &title, const QString &chapter, const QString &chap_path)
{
    QDir dir(chap_path);
    QString page,page_path,order;
    QFileInfoList list;
    dir.setFilter(QDir::Files|QDir::NoDot|QDir::NoDotAndDotDot);
    QStringList namefilters;
    namefilters<<"*jpg"<<"*bmp"<<"*png"<<"jpeg";
    dir.setNameFilters(namefilters);
    list = dir.entryInfoList();
    for(auto &i : list)
    {
        page = i.baseName();  //这里的chapter,chap_path 代指page,page_pat
        page_path = i.absoluteFilePath() ;  //方便少声明变量
        order =QString("insert into page("
                       "title,chapter,page,page_path) values"
                       "('%1','%2','%3','%4')")
                .arg(title,chapter,chapter,chap_path);
        //qDebug()<<title<<" "<<comic_page<<" "<<page_path<<" "<<numx;
        if(!query.exec(order)) qDebug()<<"add chap page 失败"<<db.lastError();
    }
}



void ReadWin::dis_data()
{
    refresh_title_data();
    auto iter = title_map.cbegin();
    qDebug()<<"显示title表";
    while(iter != title_map.cend())
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


