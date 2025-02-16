 #include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QButtonGroup>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QButtonGroup *btnGp=new QButtonGroup(this);
    btnGp->addButton(ui->btu_stu,0);
    btnGp->addButton(ui->btn_schedule,1);
    btnGp->addButton(ui->btn_finance,2);
    btnGp->addButton(ui->btn_honor,3);
    btnGp->addButton(ui->btn_system,4);

    connect(btnGp,&QButtonGroup::idClicked,ui->stackedWidget,&QStackedWidget::setCurrentIndex);
    btnGp->button(0)->setCheckable(true);
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
