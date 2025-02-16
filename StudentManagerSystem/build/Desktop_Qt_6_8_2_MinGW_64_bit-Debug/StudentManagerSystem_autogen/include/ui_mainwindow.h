/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QToolButton *btu_stu;
    QToolButton *btn_schedule;
    QToolButton *btn_finance;
    QToolButton *btn_honor;
    QToolButton *btn_system;
    QSpacerItem *verticalSpacer;
    QStackedWidget *stackedWidget;
    QWidget *page_stuInfo;
    QLabel *label;
    QWidget *page_finance;
    QLabel *label_2;
    QWidget *page_honor;
    QLabel *label_3;
    QWidget *page_system;
    QLabel *label_4;
    QWidget *page_schedule;
    QLabel *label_5;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1188, 641);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        widget->setMinimumSize(QSize(100, 0));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName("verticalLayout");
        btu_stu = new QToolButton(widget);
        btu_stu->setObjectName("btu_stu");
        btu_stu->setMinimumSize(QSize(50, 50));
        btu_stu->setCheckable(true);

        verticalLayout->addWidget(btu_stu);

        btn_schedule = new QToolButton(widget);
        btn_schedule->setObjectName("btn_schedule");
        btn_schedule->setMinimumSize(QSize(50, 50));
        btn_schedule->setCheckable(true);

        verticalLayout->addWidget(btn_schedule);

        btn_finance = new QToolButton(widget);
        btn_finance->setObjectName("btn_finance");
        btn_finance->setMinimumSize(QSize(50, 50));
        btn_finance->setCheckable(true);

        verticalLayout->addWidget(btn_finance);

        btn_honor = new QToolButton(widget);
        btn_honor->setObjectName("btn_honor");
        btn_honor->setMinimumSize(QSize(50, 50));
        btn_honor->setCheckable(true);

        verticalLayout->addWidget(btn_honor);

        btn_system = new QToolButton(widget);
        btn_system->setObjectName("btn_system");
        btn_system->setMinimumSize(QSize(50, 50));
        btn_system->setCheckable(true);

        verticalLayout->addWidget(btn_system);

        verticalSpacer = new QSpacerItem(20, 240, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addWidget(widget);

        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        page_stuInfo = new QWidget();
        page_stuInfo->setObjectName("page_stuInfo");
        label = new QLabel(page_stuInfo);
        label->setObjectName("label");
        label->setGeometry(QRect(370, 260, 53, 15));
        stackedWidget->addWidget(page_stuInfo);
        page_finance = new QWidget();
        page_finance->setObjectName("page_finance");
        label_2 = new QLabel(page_finance);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(310, 200, 53, 15));
        stackedWidget->addWidget(page_finance);
        page_honor = new QWidget();
        page_honor->setObjectName("page_honor");
        label_3 = new QLabel(page_honor);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(420, 290, 53, 15));
        stackedWidget->addWidget(page_honor);
        page_system = new QWidget();
        page_system->setObjectName("page_system");
        label_4 = new QLabel(page_system);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(450, 270, 53, 15));
        stackedWidget->addWidget(page_system);
        page_schedule = new QWidget();
        page_schedule->setObjectName("page_schedule");
        label_5 = new QLabel(page_schedule);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(430, 250, 53, 15));
        stackedWidget->addWidget(page_schedule);

        horizontalLayout->addWidget(stackedWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1188, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        btu_stu->setText(QCoreApplication::translate("MainWindow", "\345\255\246\345\221\230\344\277\241\346\201\257", nullptr));
        btn_schedule->setText(QCoreApplication::translate("MainWindow", "\350\257\276\347\250\213\345\256\211\346\216\222", nullptr));
        btn_finance->setText(QCoreApplication::translate("MainWindow", "\350\264\242\345\212\241\344\277\241\346\201\257", nullptr));
        btn_honor->setText(QCoreApplication::translate("MainWindow", "\350\215\243\350\252\211\346\216\222\350\241\214", nullptr));
        btn_system->setText(QCoreApplication::translate("MainWindow", "\347\263\273\347\273\237\350\256\276\347\275\256", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "3", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "4", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "5", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
