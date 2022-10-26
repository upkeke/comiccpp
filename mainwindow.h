#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"datamanager.h"
#include<QThread>
#include<QListWidgetItem>
#include"worker.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class ReadWin;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void fill_title_listwid();
    void change_title_listwid(QListWidgetItem *item);
private:
    Ui::MainWindow *ui;
    ReadWin * readwin;
    QThread* t1;
    //QThread t1;
signals:
    void dis_comic(QString title);
    void begin_work(int x);
private slots:

};

#endif // MAINWINDOW_H
