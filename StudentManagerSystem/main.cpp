#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include "databasemanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // 先初始化 QApplication

    DataBaseManager::instance(); // 再初始化数据库

    QFile styleFile("C:\\Users\\Administrator\\Desktop\\pro\\StudentManagerSystem\\style\\style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QString(styleFile.readAll());
        a.setStyleSheet(styleSheet);
        styleFile.close();
    } else {
        qWarning() << "Fail to load stylesheet.";
    }

    MainWindow w;
    w.show();
    return a.exec();
}
