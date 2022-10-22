#ifndef READWIN_H
#define READWIN_H

#include<QMainWindow>
#include"ui_ReadWin.h"

#include<QGraphicsView>
#include<QGraphicsPixmapItem>
#include<QGraphicsScene>
#include<QPixmap>
#include<QScrollBar>
#include<QPointF>
#include<QDockWidget>
#include<QFileDialog>
#include<QMap>
#include"datamanager.h"

extern shared_ptr<DataManager> mydata;

class ReadWin : public QMainWindow
{
    Q_OBJECT
public:
    explicit ReadWin(QWidget *parent = nullptr);
    ~ReadWin();
    void fill_chap_listwid(QListWidgetItem *item);
    void paint_img_nochap();
    void paint_img_chap(const QString&title);
    void get_item();
    void open_folder();  //打开文件夹

    void fill_listwid(QString str,bool flag);

    void dis_data(); //显示当前漫画名列表
    void mousePressEvent(QMouseEvent *event)override;
private:
    Ui::ReadWin * ui;
    QDir dir;
    QStringList page_path_list; //文件夹下每张图片的路径

    int num;
    QGraphicsScene * scene;
    //QGraphicsView *view;
    QList<QGraphicsPixmapItem*> grp_pix_list;
    QDockWidget *myDock1;
    bool isShow;


signals:
    //回到主场景的信号
    void backMainScene();
    //打开文件后，选中的漫画应该排在首位，这时候要发射信号，让主界面的目录顺序改变
    void sort_title_num();
};

#endif // READWIN_H
