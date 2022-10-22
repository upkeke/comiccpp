#ifndef TITLE_H
#define TITLE_H
#include<QString>

class Title
{
public:
    QString title;  //漫画名
    QString title_path; //绝对路径
    int ischapter;  //有无章节细分
    float progress; //阅读进度
    int chap_count; //漫画有多少章
    int num; //漫画在主界面的排序
    Title()=default;
    Title(const QString &title,const QString &title_path,int ischapter,float progress,int chap_count,int num);

};

class Chapter
{
    //漫画名：title,/ 章节：chapter(Key),/ 章节路径：chap_path,
    //页数：page_count,/备注，如果没有章节，则数据插入表page中对应的页数
public:
    QString title;
    QString chapter;
    QString chap_path;
    int page_count; //这一章有多少页
    Chapter()=default;
    Chapter(const QString &title,const QString &chapter,const QString &chap_path,int page_count);
};

class Page
{
    // 漫画下每个章节的所有page的集合
    //Key: page
    //value: / 漫画名：title,/ 章节：chapter,/ 页码 page 页码路径：page_path,
public:
    QString title;
    QString chapter ;
    QString page;
    QString page_path;
    int page_num;
    Page()=default;
    Page(const QString &title,const QString &page,const QString &page_path,int page_num,const QString &chapter= "none")
        :title(title),chapter(chapter)
        ,page(page),page_path(page_path),page_num(page_num){}
};

#endif // TITLE_H
