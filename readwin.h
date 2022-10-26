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

    void open_folder();  //打开文件夹

    void fill_chapList(const QString& title);  //右边停靠窗口显示章节名的窗口

    void fill_page_win(QString page_name); //右边停靠窗口显示每一页名字的窗口
    void fill_pageList_fromTitle(const QString& title);
    void fill_pageList_fromChap(QListWidgetItem *item);

    void dis_data(); //显示当前漫画名列表
    void mousePressEvent(QMouseEvent *event)override;
private:
    Ui::ReadWin * ui;

    int num;
    QGraphicsScene * scene;
    //QGraphicsView *view;
    QList<QGraphicsPixmapItem*> pixmapItems;
    QDockWidget *myDock1;
    bool isShow;
    QString current_title;
    QString current_chapter;

    void fill_pageList();
    void display_page();



signals:
    //回到主场景的信号
    void backMainScene();
    //打开文件后，选中的漫画应该排在首位，这时候要发射信号，让主界面的目录顺序改变
    void sort_title_num();
};

#endif // READWIN_H
