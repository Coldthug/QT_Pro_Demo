#include "schedulewidget.h"
#include "ui_schedulewidget.h"
#include <qboxlayout.h>
#include <qdatetime.h>
#include <qdatetimeedit.h>
#include <qformlayout.h>
#include <qheaderview.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qsqlerror.h>
#include <qsqlquery.h>
#include <QDialogButtonBox>

int customWeekNumBer(const QDate& date){
    QDate startOfYear(date.year(),1,1);
    int dayOfWeek=startOfYear.dayOfWeek();
    int days = startOfYear.daysTo(date);
    int week =(days + dayOfWeek -1)/7+1;
    return week;
}

ScheduleWidget::ScheduleWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ScheduleWidget)
{
    ui->setupUi(this);

    setupUI();

    int currentYear =QDate::currentDate().year();
    int currentWeek =customWeekNumBer(QDate::currentDate());

    yearComboBox->setCurrentText(QString::number(currentYear));
    weekComBoBox->setCurrentText(QString("第 %1 周").arg(currentWeek));

    loadSchedule();

}

ScheduleWidget::~ScheduleWidget()
{
    delete ui;
}



void ScheduleWidget::setupUI()
{
    QVBoxLayout* mainLayout=new QVBoxLayout(this);
    QHBoxLayout* dateLayout=new QHBoxLayout();
    yearComboBox=new QComboBox(this);
    weekComBoBox=new QComboBox(this);

    int currentYear=QDate::currentDate().year();
    for(int year=2020;year<=currentYear+5;++year){
        yearComboBox->addItem(QString::number(year),year);
    }

    for(int week=1;week<=52;++week){
        weekComBoBox->addItem(QString("第 %1 周").arg(week),week);
    }
    dateRangeLabel=new QLabel(this);

    QPushButton* prevWeekBtn=new QPushButton("上一周",this);
    QPushButton* nextWeekBtn=new QPushButton("下一周",this);
    prevWeekBtn->setFixedWidth(200);
    nextWeekBtn->setFixedWidth(200);

    dateLayout->addWidget(new QLabel("年份:",this));
    dateLayout->addWidget(yearComboBox);
    dateLayout->addWidget(new QLabel("周数:",this));
    dateLayout->addWidget(weekComBoBox);
    dateLayout->addWidget(dateRangeLabel);
    dateLayout->addStretch();

    tableWidget=new QTableWidget(this);
    tableWidget->setAlternatingRowColors(true);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    setupTable();

    addButton=new QPushButton("添加课程",this);
    deleteButton=new QPushButton("删除课程",this);
    addButton->setFixedWidth(200);
    deleteButton->setFixedWidth(200);

    connect(yearComboBox,QOverload<int>::of(&QComboBox::currentIndexChanged),this,&ScheduleWidget::loadSchedule);
    connect(weekComBoBox,QOverload<int>::of(&QComboBox::currentIndexChanged),this,&ScheduleWidget::loadSchedule);

    connect(addButton,&QPushButton::clicked,this,&ScheduleWidget::addCourse);
    connect(deleteButton,&QPushButton::clicked,this,&ScheduleWidget::deleteCourse);
    connect(prevWeekBtn,&QPushButton::clicked,this,&ScheduleWidget::showPreviousWeek);
    connect(nextWeekBtn,&QPushButton::clicked,this,&ScheduleWidget::showNextWeek);
    connect(tableWidget,&QTableWidget::itemChanged,this,&ScheduleWidget::handleItmChanged);

    QHBoxLayout* buttonLayout=new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(prevWeekBtn);
    buttonLayout->addWidget(nextWeekBtn);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();
    mainLayout->addLayout(dateLayout);
    mainLayout->addWidget(tableWidget);
    mainLayout->addLayout(buttonLayout);

    //setLayout(buttonLayout); //错误布局，导致嵌套，QWidget::setLayout: Attempting to set QLayout "" on ScheduleWidget "ScheduleWidget", which already has a layout
    setLayout(mainLayout);

}

QPair<QDate,QDate> ScheduleWidget::getWeekRange(int year,int week){
    QDate startDate(year,1,1);
    int daysToSubtract=startDate.dayOfWeek() - Qt::Monday;
    if(daysToSubtract>0)startDate=startDate.addDays(-daysToSubtract);
    QDate weekStart=startDate.addDays((week-1)*7);
    QDate weekEnd=weekStart.addDays(6);
    return qMakePair(weekStart,weekEnd);
}
void ScheduleWidget::setupTable()
{
    QStringList days={"星期一","星期二","星期三","星期四","星期五","星期六","星期日"};
    QStringList times={"上午1","上午2","下午1","下午2","晚上1","晚上2"};
    tableWidget->setRowCount(days.count());
    tableWidget->setColumnCount(times.count());
    int year=yearComboBox->currentData().toInt();
    int week=weekComBoBox->currentData().toInt();
    QPair<QDate,QDate> weekRange=getWeekRange(year,week);
    QDate startDate=weekRange.first;
    QStringList verticalHears;
    for(int i=0;i<days.count();i++){
        QDate currentDate=startDate.addDays(i);
        verticalHears.append(QString("%1\n%2").arg(days[i], currentDate.toString("MM/dd")));
    }
    tableWidget->setVerticalHeaderLabels(verticalHears);
    tableWidget->setHorizontalHeaderLabels(times);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed);
}


void ScheduleWidget::loadSchedule()
{
    tableWidget->blockSignals(true);
    tableWidget->clearContents();

    int year=yearComboBox->currentData().toInt();
    int week=yearComboBox->currentData().toInt();

    QPair<QDate,QDate> weekRange=getWeekRange(year,week);
    QDate startDate=weekRange.first;
    QDate endDate=weekRange.second;
    dateRangeLabel->setText(startDate.toString("yyyy-MM-dd")+"到"+endDate.toString("yyyy-MM-dd"));

    QVector<QVector<QString>> courses(7,QVector<QString>(times.count(),""));

    QSqlQuery query;
    query.prepare("Select date,time,coursr_name from schedule where date between ? and ?");
    query.addBindValue(startDate.toString("yyyy-MM-dd"));
    query.addBindValue(endDate.toString("yyyy-MM-dd"));

    if(query.exec()){
        while(query.next()){
            QDate date =QDate::fromString(query.value(0).toString(),"yyyy-MM-dd");
            QString time =query.value(1).toString();
            int dayIndex=startDate.daysTo(date);
            int timeIndex=times.indexOf(time);
            if(dayIndex>=0&& dayIndex<7&&timeIndex!=-1){
                courses[dayIndex][timeIndex]=query.value(2).toString();
            }
        }
    }

    for(int day =0;day<7;++day){
        for(int time=0;time<times.count();++time){
            QTableWidgetItem* item=new QTableWidgetItem(courses[day][time]);
            item->setTextAlignment(Qt::AlignCenter);
            tableWidget->setItem(day,time,item);
        }
    }

    tableWidget->blockSignals(false);

}

void ScheduleWidget::addCourse()
{
    int dayIndex=tableWidget->currentRow();
    int timeIndex=tableWidget->currentColumn();
    if(dayIndex==-1|| timeIndex==-1){
        QMessageBox::warning(this,"错误","该段时间已被占用");
        return;
    }
    QDialog dialog(this);
    dialog.setWindowTitle("添加课程");
    QFormLayout layout(&dialog);
    QComboBox nameCombo;
    QSqlQuery nameQuery("select name from studentInfo");
    while(nameQuery.next()) nameCombo.addItem(nameQuery.value(0).toString());
    QMap<int,QTime> timePresets{
        {0,QTime(9,0)},{1,QTime(11,0)},{2,QTime(14,0)},{3,QTime(16,0)},{4,QTime(19,0)},{5,QTime(21,0)}
    };
    QTimeEdit timeEdit;
    timeEdit.setDisplayFormat("HH:mm");
    timeEdit.setTime(timePresets.value(timeIndex));
    layout.addRow("学生姓名",&nameCombo);
    layout.addRow("课程时间",&timeEdit);
    QDialogButtonBox buttons(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    buttons.button(QDialogButtonBox::Ok)->setText("确定");
    buttons.button(QDialogButtonBox::Cancel)->setText("取消");
    layout.addRow(&buttons);
    connect(&buttons,&QDialogButtonBox::accepted,&dialog,&QDialog::accept);
    connect(&buttons,&QDialogButtonBox::rejected,&dialog,&QDialog::reject);
    if(dialog.exec()!=QDialog::Accepted) return;
    QString courseName=QString("%1,%2").arg(nameCombo.currentText(), timeEdit.time().toString("HH:mm"));

    int year=yearComboBox->currentData().toInt();
    int week=weekComBoBox->currentData().toInt();
    QPair<QDate,QDate> weekRange=getWeekRange(year,week);
    QDate currentDate=weekRange.first.addDays(dayIndex);
    QString timeSlot=times[timeIndex];

    QSqlQuery query;
    query.prepare("insert into schedule(date,time,course_name) value(?,?,?)");
    query.addBindValue(currentDate.toString("yyyy-MM-dd"));
    query.addBindValue(timeSlot);
    query.addBindValue(courseName);
    if(!query.exec()){
        QMessageBox::critical(this,"错误","添加失败:"+query.lastError().text());
    }else loadSchedule();


}

void ScheduleWidget::deleteCourse()
{
    QMessageBox confirmBox(this);
    confirmBox.setWindowTitle("确认删除");
    confirmBox.setText("确认删除该记录吗?");

    QPushButton* yesButton=confirmBox.addButton("确定",QMessageBox::YesRole);
    QPushButton* noButton=confirmBox.addButton("取消",QMessageBox::NoRole);

    confirmBox.setDefaultButton(noButton);

    confirmBox.exec();

    if(confirmBox.clickedButton()== yesButton){
        int dayIndex=tableWidget->currentRow();
        int timeIndex=tableWidget->currentColumn();

        if(dayIndex==-1||timeIndex==-1){
            QMessageBox::warning(this,"错误","请先选择一个时间段");
            return;
        }
        QTableWidgetItem* item=tableWidget->item(dayIndex,timeIndex);
        if(!item||item->text().isEmpty()){
            QMessageBox::warning(this,"错误","该时间段没有课程!");
            return;
        }

        int year=yearComboBox->currentData().toInt();
        int week=weekComBoBox->currentData().toInt();
        QPair<QDate,QDate> weekRange=getWeekRange(year,week);
        QDate currentDate=weekRange.first.addDays(dayIndex);
        QString time=times[timeIndex];

        QSqlQuery query;
        query.prepare("delete from schedule where date=? and time=?");
        query.addBindValue(currentDate.toString("yyyy-MM-dd"));
        query.addBindValue(time);
        if(!query.exec()){
            QMessageBox::critical(this,"错误","删除失败:"+query.lastError().text());
        }else loadSchedule();
    }

}

void ScheduleWidget::showPreviousWeek()
{
    int currentWeek=weekComBoBox->currentIndex();
    int currentYear=yearComboBox->currentIndex();

    if(currentWeek>0){
        weekComBoBox->setCurrentIndex(currentWeek-1);
    }else{
        if(yearComboBox->currentIndex()>0){
            yearComboBox->setCurrentIndex(currentYear-1);
            weekComBoBox->setCurrentIndex(51);
        }
    }
}

void ScheduleWidget::showNextWeek()
{
    int currentWeek=weekComBoBox->currentIndex();
    int currentYear=yearComboBox->currentIndex();

    if(currentWeek<51){
        weekComBoBox->setCurrentIndex(currentWeek+1);
    }else{
        if(yearComboBox->currentIndex()<yearComboBox->count()-1){
            yearComboBox->setCurrentIndex(currentYear+1);
            weekComBoBox->setCurrentIndex(0);
        }
    }
}

void ScheduleWidget::handleItmChanged(QTableWidgetItem* item)
{
    int day=item->row();
    int timeSlot=item->column();
    QString newCourse=item->text().trimmed();

    int year=yearComboBox->currentData().toInt();
    int week=weekComBoBox->currentData().toInt();

    QPair<QDate,QDate> weekRange=getWeekRange(year,week);
    QDate date=weekRange.first.addDays(day);
    QString time=times[timeSlot];

    QSqlQuery query;
    if(newCourse.isEmpty()){
        query.prepare("delete from schedule where date=? and time=?");
        query.addBindValue(date.toString("yyyy-MM-dd"));
        query.addBindValue(time);
    }else{
        query.prepare("insert or replace into schedule (date,time,course_name) values(?,?,?)");
        query.addBindValue(date.toString("yyyy-MM-dd"));
        query.addBindValue(time);
    }

    if(!query.exec()){
        QMessageBox::critical(this,"错误","操作失败:"+query.lastError().text());
        loadSchedule();
    }
}

