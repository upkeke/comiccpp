#ifndef READWIN_H
#define READWIN_H

#include<QMainWindow>
#include"ui_ReadWin.h"
#include<QList>
#include<QString>
#include<QDebug>
#include<QDir>
#include<QStringList>
#include<QPushButton>
#include<QListWidget>
#include<QListWidgetItem>
#include<QGraphicsView>
#include<QGraphicsPixmapItem>
#include<QGraphicsScene>
#include<QPixmap>
#include<QScrollBar>
#include<QPointF>
#include<QDockWidget>
#include<QFileDialog>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<QMap>

#include"title.h"
#include"listitem.h"
//#include"mainwindow.h"

#include<utility>
#include<algorithm>
using std::sort;
using std::pair;
using std::make_pair;




extern QSqlDatabase db; //数据库
extern QSqlQuery query; //执行sql指令

// 漫画名 / 漫画名路径 / 是否下面还有章节分支(0无 1有) / 读取进度 / 主界面的排序 ||对应表 title
extern QMap<QString,Title> title_map;
extern QStringList title_list; //辅助上面map表   /index对应num / 漫画名/
void refresh_title_data(); // 读取数据库title表数据到容器中
bool comp(const QString&str1,const QString&str2);

class ReadWin : public QMainWindow
{
    Q_OBJECT
public:
    explicit ReadWin(QWidget *parent = nullptr);
    ~ReadWin();
    void fill_diect_win(QListWidgetItem *item);
    void paint_img();
    void get_item();
    void open_folder();  //打开文件夹

    void fill_child_list(QString str,bool flag);
    //void refresh_title_data(); // 读取数据库title表数据到容器中
    void refresh_chapter_data(const QString &title); // 读取数据库chapter表数据到容器中
    void refresh_page_data(const QString &chapter); // 读取数据库page表数据到容器中


    void add_title(const QString & titlepath); //添加漫画名的信息到数据库和容器中
    void add_chapter(const QString & titlepath,const QString &title,bool fa); //添加章节的信息到数据库和容器中
    void add_page(const QString &title,const QString &chapter,const QString &chap_path); //添加章节的信息到数据库和容器中

    void dis_data(); //显示当前漫画名列表
    void mousePressEvent(QMouseEvent *event)override;
private:
    Ui::ReadWin * ui;
    QDir dir;
    QStringList page_path_list; //文件夹下每张图片的路径

    // 漫画名 / 漫画名路径 / 是否下面还有章节分支(0无 1有) / 读取进度 / 主界面的排序 ||对应表 title
    //QMap<QString,Title> title_map;
    //QStringList title_list; //辅助上面map表   /index对应num / 漫画名/

    // 漫画下所有个章节的集合
    //Key: chapter
    //value: / 漫画名：title,/ 章节：chapter,/ 章节路径：chap_path(primary Key),
    //页数：page_count,/备注，如果没有章节，则数据插入表page中对应的页数
    QMap<QString,Chapter> chap_map;
    QList<QString> chap_list; //辅助上面map表   /index对应 第几章 / 章节名/

    // 漫画下每个章节的所有page的集合
    //Key: page
    //value: / 漫画名：title,/ 章节：chapter,/ 页码 page 页码路径：page_path,
    QMap<QString,Page> page_map;
    QList<QString> page_list; //辅助上面map表   /index对应 第几章 / 章节名/

    int num;
    QGraphicsScene * scene;
    //QGraphicsView *view;
    QList<QGraphicsPixmapItem*> grp_pix_list;
    QDockWidget *myDock1;
    bool isShow;


signals:
    //回到主场景的信号
    void backMainScene();
};

#endif // READWIN_H
