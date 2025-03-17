#ifndef FINANCIALWIDGET_H
#define FINANCIALWIDGET_H

#include <QWidget>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qtablewidget.h>
#include <QtCharts>
#include <QSqlQuery>

namespace Ui {
class FinancialWidget;
}

class FinancialWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FinancialWidget(QWidget *parent = nullptr);
    ~FinancialWidget();

private:
    void setupUI();
    QChartView* pieChartView;
    QTableWidget* tableWidget;
    QComboBox* studentComboBox;
    QPushButton* addButton;
    QPushButton* deleteButton;
    QPushButton* editButton;
    QChartView* chartView;
    QDateEdit* startDateEdit;
    QDateEdit* endDateEdit;
    Ui::FinancialWidget *ui;

    void loadFinancialRecords();
    void populateStudentComboBox();
    void addRecord();
    void editRecord();
    void deleteRecord();
    void updateChart();
    void updatePieChart();


};

#endif // FINANCIALWIDGET_H
