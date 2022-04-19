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
#include<QGraphicsView>
#include<QGraphicsPixmapItem>
#include<QGraphicsScene>
#include<QPixmap>
#include<QScrollBar>
#include<QPointF>
#include<QDockWidget>
#include"listitem.h"
#include"mainwindow.h"
#include<QFileDialog>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>

class ReadWin : public QMainWindow
{
    Q_OBJECT
public:
    explicit ReadWin(QWidget *parent = nullptr);
    ~ReadWin();
    void get_comic_dir(QString path);
    void paint_img();
    void get_item();

    void fill_child_list(QString str,bool flag);
    void get_data(); //读取数据库
    void mousePressEvent(QMouseEvent *event)override;
private:
    Ui::ReadWin * ui;
    QDir dir;
    QStringList page_path_list; //文件夹下每张图片的路径
    // 漫画名 / 漫画名路径 / 是否下面还有章节分支
    QStringList chapter_list; //每个漫画的章节名
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
