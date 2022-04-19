#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QPushButton>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include <QDebug>

#include"test.h"
#include"readwin.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

extern QSqlDatabase db; //数据库
extern QSqlQuery query; //执行sql指令
class ReadWin;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    ReadWin * childwin;
    //ReadWin *childwin; //阅读窗口
    //QSqlDatabase db;  //数据库
};

#endif // MAINWINDOW_H
