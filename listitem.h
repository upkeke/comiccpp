#ifndef LISTITEM_H
#define LISTITEM_H

#include <QWidget>
#include <QString>
namespace Ui {
class listItem;
}

class listItem : public QWidget
{
    Q_OBJECT

public:
    explicit listItem(QString str,bool flag,QWidget *parent = nullptr);
    ~listItem();
    bool flag;
    void display_favor(); //显示 收藏
private slots:
    void on_radioButton_toggled(bool checked);

private:
    Ui::listItem *ui;
    QString str;
};

#endif // LISTITEM_H
