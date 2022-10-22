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
    explicit listItem(const QString &name,bool flag,QWidget *parent = nullptr);

    ~listItem();
    bool flag;
    QString name;
    void display_favor(); //显示 收藏
private slots:
    void on_radioButton_toggled(bool checked);

private:
    Ui::listItem *ui;

};

#endif // LISTITEM_H
