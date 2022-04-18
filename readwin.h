#ifndef READWIN_H
#define READWIN_H


#include <QMainWindow>
#include"ui_ReadWin.h"
#include<QList>
#include<QString>
#include<QDebug>
#include<QDir>
#include <QStringList>
#include<QPushButton>
#include<QGraphicsView>
#include<QGraphicsPixmapItem>
#include<QGraphicsScene>
#include<QPixmap>
#include<QScrollBar>
#include<QPointF>
#include<QDockWidget>
class ReadWin : public QMainWindow
{
    Q_OBJECT
public:
    explicit ReadWin(QWidget *parent = nullptr);
    ~ReadWin();
    void init_ui();
    void get_comic_dir(QString path=R"(E:\Qt\projuct\ComicByCPP\comic\监狱学园\1)");
    void paint_img();
    void get_item();
    void mousePressEvent(QMouseEvent *event)override;
private:
    Ui::ReadWin * ui;
    QDir dir;
    QStringList file_list; //文件夹下每张图片的路径
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
