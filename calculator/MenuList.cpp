#include "MenuList.h"
#include "ui_MenuList.h"

MenuList::MenuList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MenuList)
{
    ui->setupUi(this);
}

MenuList::~MenuList()
{
    delete ui;
}
