#ifndef SQLMANAGER_H
#define SQLMANAGER_H
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<QList>
#include<QString>
#include<QMap>
#include<map>
#include"title.h"

#include<memory>
using std::shared_ptr;
using std::map;


#pragma execution_character_set("utf-8")

bool myqstrcmp(const QString&str1,const QString&str2);


class DataManager
{
public:
    QSqlDatabase db; //
    QSqlQuery query; //执行sql指令

    // key漫画名 / 漫画名路径 / 是否下面还有章节分支(0无 1有) / 读取进度 / 主界面的排序 ||对应表 title
    QMap<QString,Title> title_map;
    QList<QString> title_list; //辅助上面map表   /index对应num / 漫画名/

    // 漫画下所有个章节的集合
    //Key: chapter
    //value: / 漫画名：title,/ 章节：chapter,/ 章节路径：chap_path(primary Key),
    //页数：page_count,/备注，如果没有章节，则数据插入表page中对应的页数
    map<QString,Chapter,decltype(myqstrcmp)*> chap_map;


    // 漫画下每个章节的所有page的集合
    //Key: page
    //value: / 漫画名：title,/ 章节：chapter,/ 页码 page 页码路径：page_path,
    map<QString,Page,decltype(myqstrcmp)*> page_map;

    static shared_ptr<DataManager> instance_db();


    void download_data(); // 在程序启动的时候的时候从数据库下载所有数据 // 也是一次大刷新

    void refresh_title_data(); // 读取数据库title表数据到容器中
    void refresh_chapter_data(const QString &title); // 读取数据库chapter表数据到容器中
    // 读取数据库page表数据到容器中,有章节从表chapter中找，没有则在表page中找
    //void refresh_page_data(const QString &tit_chap,bool ischapter);
    void down_pageData_byTitle(const QString &title);
    void down_pageData_byChap(const QString &chapter);

    void add_title(const QString & titlepath); //添加漫画名的信息到数据库和容器中
    void change_title_num(const QString & title);
    void del_title_num(const QString &title);
    void add_chapter(const QString & titlepath,const QString &title,bool fa); //添加章节的信息到数据库和容器中
    void add_page(const QString &title,const QString &chapter,const QString &chap_path); //添加章节的信息到数据库和容器中

    void delete_comic(const QString &title);


    ~DataManager();
private:
    static shared_ptr<DataManager> single_sql;
    DataManager();

};

#endif // SQLMANAGER_H
