#ifndef SCHEDULEWIDGET_H
#define SCHEDULEWIDGET_H

#include <QWidget>
#include <qcombobox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qtablewidget.h>

namespace Ui {
class ScheduleWidget;
}

class ScheduleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ScheduleWidget(QWidget *parent = nullptr);
    ~ScheduleWidget();

private slots:
    void loadSchedule();
    void addCourse();
    void deleteCourse();
    void showPreviousWeek();
    void showNextWeek();
    void handleItmChanged(QTableWidgetItem* item);

private:
    void setupUI();
    void setupTable();
    QStringList times;
    QPair<QDate,QDate> getWeekRange(int year,int week);
    QMap<QPair<int,int>,QVector<QVector<QString>>> scheduleData;

    QTableWidget* tableWidget;
    QComboBox* yearComboBox;
    QComboBox* weekComBoBox;
    QLabel* dateRangeLabel;
    QPushButton* addButton;
    QPushButton* deleteButton;
    QPushButton* prevWeekBtn;
    QPushButton* nextWeekBtn;
    Ui::ScheduleWidget *ui;
};

#endif // SCHEDULEWIDGET_H
