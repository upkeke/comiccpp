#include "listitem.h"
#include "ui_listitem.h"

listItem::listItem(const QString &str, bool flag,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::listItem),name(str),flag(flag)
{
    ui->setupUi(this);
    ui->label->setText(str);
    if(flag)
        ui->radioButton->show();
    else
        ui->radioButton->hide();
}
listItem::~listItem()
{
    delete ui;
}

void listItem::display_favor()
{
    ui->radioButton->show();
    flag=true;
}
void listItem::on_radioButton_toggled(bool checked)
{
    if (!checked)
        ui->radioButton->hide();
    flag= checked;

}

