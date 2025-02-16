#include "mainwindow.h"

#include <QApplication>
#include <QFile>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile styleFile("C:\\Users\\Administrator\\Desktop\\pro\\StudentManagerSystem\\style\\style.qss");

    if(styleFile.open(QFile::ReadOnly)){
        QString styleSheet=QString(styleFile.readAll());
        a.setStyleSheet(styleSheet);
        styleFile.close();
    }else{
        qWarning()<<"Fail to load stylesheet.";
    }

    MainWindow w;
    w.show();
    return a.exec();
}
