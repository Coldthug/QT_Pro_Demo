#ifndef MENULIST_H
#define MENULIST_H

#include <QWidget>

namespace Ui {
class MenuList;
}

class MenuList : public QWidget
{
    Q_OBJECT

public:
    explicit MenuList(QWidget *parent = nullptr);
    ~MenuList();

private:
    Ui::MenuList *ui;
};

#endif // MENULIST_H
