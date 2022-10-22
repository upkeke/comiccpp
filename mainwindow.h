#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"datamanager.h"
#include<QThread>
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
