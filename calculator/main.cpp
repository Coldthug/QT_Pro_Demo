#include "Calculator.h"
#include "Standard.h"
#include <QApplication>
#include"MenuList.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Calculator w;
    Standard standard;
    MenuList m;
    //m.show();
    standard.show();
    //w.show();
    return a.exec();
}
