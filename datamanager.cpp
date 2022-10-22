#include "datamanager.h"
#include<QDebug>
#include<QDir>
#include<QStringList>
#include<QPushButton>
#include<QListWidget>
#include<QListWidgetItem>
#include<utility>
#include<algorithm>
using std::make_shared;
using std::sort;

#include<worker.h>

//
bool myqstrcmp(const QString &str1, const QString &str2)
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

shared_ptr<DataManager> DataManager::single_sql = nullptr;

shared_ptr<DataManager> DataManager::instance_db()
{
    if(single_sql == nullptr)
        single_sql = shared_ptr<DataManager>(new DataManager());
    return single_sql;
}


DataManager::DataManager()
{

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data.db");
    if(!db.open()) qDebug()<<"open error **"<<db.lastError();
    else qDebug()<<"连接成功";
    query = QSqlQuery();
}



DataManager::~DataManager()
{
    single_sql =nullptr;
    db.close();
}
//
void DataManager::download_data()
{
    refresh_title_data();
    //额外线程下载 章节 页数的信息
}

// 下载 表title 的信息
void DataManager::refresh_title_data()
{
    title_list.clear();
    title_map.clear();
    //根据num排序
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
            int num =query.value("num").toInt();  //漫画在主界面的排序
            //qDebug()<<title<<" "<<title_path<<" "<<ischapter;
            title_map[title]= Title(title,title_path,ischapter,progress,chap_count,num);
            title_list.push_back(title);
        }
    }
}

// 下载 表chapter 的信息
void DataManager::refresh_chapter_data(const QString &title)
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
            chap_list.push_back(chapter);
        }
    }
    sort(chap_list.begin(),chap_list.end(),myqstrcmp);
}
//下载页的数据
void DataManager::refresh_page_data(const QString &tit_chap, bool ischapter)
{
    page_list.clear();
    page_map.clear();
    QString title;
    QString chapter;
    if(ischapter)  //
    {
        title =chap_map[tit_chap].title;
        chapter = tit_chap; //有章节 tit_chap 代表 chapter
    }
    else
    {
        title = tit_chap; //没有章节 tit_chap 代表title
        chapter = "none";
    }
    QString order = QString("select *from page "
                            "where title = '%1' and chapter = '%2'")
            .arg(title,chapter);
    if(!query.exec(order))
        qDebug()<<"获取失败"<<db.lastError();
    else{
        qDebug()<<"获取成功";
        while(query.next()){
            //page名的目录
            QString title = query.value("title").toString();
            QString chapter = query.value("chapter").toString();
            QString page = query.value("page").toString();
            QString page_path = query.value("page_path").toString();
            // QMap<QString,Page> page_map;

            page_map[page] = {title,chapter,page,page_path};
            page_list.push_back(page);
        }
    }
    // 对page_list重排
    sort(page_list.begin(),page_list.end(),myqstrcmp);
}

//上传数据到表title  //这是通过打开文件修改的title表
//漫画名：title（Key） / 漫画路径：title_path,是否子文件夹：ischapter
//章数 chap_count,/ 阅读进度：progress,/ 主窗口的位次：num
void DataManager::add_title(const QString & path)
{
    QFileInfo title_info(path);
    //判断该漫画是否已经存在
    //如果已存在，则主界面依然要添加，但是要添加后缀
    QString title= title_info.baseName();
    auto contain_iter = title_map.find(title);
    if(contain_iter!=title_map.end())
    {
        //点击一部漫画后就把它放在书架的首位，num就是这本漫画的在所有的漫画的排序
        change_title_num(contain_iter.value().title);
        return;
    }
    // 如果不存在
    QDir dir(path);
    dir.setFilter(QDir::Dirs|QDir::NoDotAndDotDot|QDir::NoDot);
    //dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();
    bool ischapter;
    int chap_count;
    if(list.size()!=0) //如果是漫画文件夹下面是也是文件夹，即含有章节分支
    {
        ischapter = true;
    }
    else {
        dir.setFilter(QDir::Files|QDir::NoDotAndDotDot|QDir::NoDot);
        QStringList namefilters;
        namefilters<<"*jpg"<<"*bmp"<<"*png"<<"jpeg";
        dir.setNameFilters(namefilters);
        list = dir.entryInfoList();
        if (list.size()!=0) //漫画文件夹下面直接是jpg
            ischapter = false;
        else
        {
            qDebug()<<"空文件夹";
            return;
        }
    }
    chap_count =list.size();
    QString insert_ComicTitle = QString("insert into title(title,"
                                        "title_path,ischapter,progress,chap_count"
                                        ",num)values('%1','%2',%3,%4,%5,%6);")
            .arg(title_info.baseName(),path,QString::number(ischapter)
                 ,QString::number(0.0),QString::number(chap_count)
                 ,QString::number(0));
    if(!query.exec(insert_ComicTitle)) qDebug()<<"add 失败"<<db.lastError();
    else qDebug()<<"add 成功";
    //对title的num重新排序
    QString change_num_order = QString("update title set num "
                                       "= replace (num,num,num+1);");
    if(!query.exec(change_num_order)) qDebug()<<"change_num2 失败"<<db.lastError();
    else qDebug()<<"change_num2 成功";
    //将漫画下的章节的传入数据库
    add_chapter(path,title,ischapter);
}

//点击主界面的漫画 会将该漫画的排序设为1 由于这里不会添加新的漫画仅会修改漫画名
void DataManager::change_title_num(const QString &title)
{
    int num= title_map[title].num;
    // 把当前点击的漫画的序号num设为0，后面会把小于num的序号统一加1，补上了1和缺掉的num
    QString change_num_order = QString("update title set num = 0 "
                                       "where num = %1;").arg(num);
    if(!query.exec(change_num_order)) qDebug()<<"change_num1 失败"<<db.lastError();
    else qDebug()<<"change_num1 成功";

    //num后面的漫画的序号不需要变，只需要要把小于num的所有序号加1
    change_num_order = QString("update title set num "
                               "=(num+1) where num < %1;").arg(num);
    if(!query.exec(change_num_order)) qDebug()<<"change_num2 失败"<<db.lastError();
    else qDebug()<<"change_num2 成功";
}
// 删掉一部漫画后，漫画排序，将排在当前漫画后面的漫画的num减一
void DataManager::del_title_num(const QString &title)
{
    int num= title_map[title].num;
    //num后面的漫画的序号不需要变，只需要要把小于num的所有序号加1
    QString change_num_order = QString("update title set num "
                                       "= num-1  where num > %1;").arg(num);
    if(!query.exec(change_num_order)) qDebug()<<"del_title_num 失败"<<db.lastError();
    else qDebug()<<"del_title_num 成功";
}


//上传 表chapter的数据（含部分表page的数据写入）
//漫画名：title,/ 章节：chapter(Key),/ 章节路径：chap_path,
//页数：page_count,/备注，如果没有章节，则数据插入表page中对应的页数
void DataManager::add_chapter(const QString & title_path,const QString &title,bool fa)
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
            page_count = QDir(chap_path).entryInfoList().size(); //这一章有多少页
            order =QString("insert into chapter("
                           "title,chapter,chap_path,"
                           "page_count) values"
                           "('%1','%2','%3',%4);")
                    .arg(title,chapter,chap_path,QString::number(page_count));
            //qDebug()<<title<<" "<<chapter<<" "<<chap_path<<" "<<page_count;

            if(!query.exec(order)) qDebug()<<"add chap失败"<<db.lastError();

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
            chapter = i.baseName();  //这里的chapter,chap_path 代指page,page_path
            chap_path = i.absoluteFilePath() ;  //方便少声明变量
            order =QString("insert into page("
                           "title,page,page_path) values"
                           "('%1','%2','%3');")
                    .arg(title,chapter,chap_path);
            //qDebug()<<title<<" "<<comic_page<<" "<<page_path<<" "<<numx;
            if(!query.exec(order)) qDebug()<<"add page 失败"<<db.lastError();
        }
        qDebug()<<"page-----";
        //insert into page(title,comic_page,page_path,index ) values('献上祝福','0','E:/Qt/projuct/ComicByCPP/comic/魔王勇者/第1话/0.jpg',0);
    }


}
//上传 表page的数据 (仅含有chapter的page)
//漫画名：title,/ 章节：chapter(Key),/ 章节路径：chap_path,
void DataManager::add_page(const QString &title, const QString &chapter, const QString &chap_path)
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
        page = i.baseName();
        page_path = i.absoluteFilePath() ;  //方便少声明变量
        order =QString("insert into page("
                       "title,chapter,page,page_path) values"
                       "('%1','%2','%3','%4');")
                .arg(title,chapter,page,page_path);
        //qDebug()<<title<<" "<<comic_page<<" "<<page_path<<" "<<numx;
        if(!query.exec(order)) qDebug()<<"add page 失败"<<db.lastError();
    }
}

void DataManager::delete_comic(const QString &title)
{
    qDebug()<<"准备删除"<<title;
    if(title_map[title].ischapter)
    {
        //删除 表chapter中
        QString del_chap = QString("delete from chapter where title='%1';").arg(title);
        if(!query.exec(del_chap))//执行sql语句
            qDebug()<<"chapter删除失败"<<db.lastError().text();
    }
    //删除 表page中
    QString del_page = QString("delete from page where title = '%1';").arg(title);
    if(!query.exec(del_page))//执行sql语句
        qDebug()<<"page删除失败"<<db.lastError().text();

    del_title_num(title);
    //删除 表title
    QString del_tit = QString("delete from title where title = '%1';").arg(title);
    if(!query.exec(del_tit))//执行sql语句
        qDebug()<<"title删除失败"<<db.lastError().text();
}

