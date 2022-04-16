#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void test_core();
    void init_graphics();
    void get_comic_dir(QString path=R"(E:\Qt\projuct\ComicByCPP\comic\监狱学园)");
    void paint_img();
    void test_img();
private:
    Ui::MainWindow *ui;
    QDir dir;
    QStringList file_list; //文件夹下每张图片的路径
    int num;
    QGraphicsScene * scene;
    QGraphicsView *view;
    QList<QGraphicsPixmapItem*> grp_pix_list;

};
#endif // MAINWINDOW_H
