#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"readwin.h"

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

private:
    Ui::MainWindow *ui;
    ReadWin * readwin;
signals:
    void dis_comic(QString title);
private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
};

#endif // MAINWINDOW_H
