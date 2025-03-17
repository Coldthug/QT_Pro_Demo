#include "financialwidget.h"
#include "ui_financialwidget.h"
#include <qsqlerror.h>
#include <QPieSeries>
#include <QPieSlice>

FinancialWidget::FinancialWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FinancialWidget)
{

    ui->setupUi(this);
    setupUI();

    populateStudentComboBox();  // 添加学生数据加载
    loadFinancialRecords();     // 初始加载财务数据
}

FinancialWidget::~FinancialWidget()
{
    delete ui;
}

void FinancialWidget::setupUI()
{
    QVBoxLayout* mainLayout=new QVBoxLayout(this);
    QHBoxLayout* topLayout=new QHBoxLayout();
    QHBoxLayout* middleLayout=new QHBoxLayout();
    chartView=new QChartView();
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(middleLayout,60);
    mainLayout->addWidget(chartView,40);

    topLayout->addWidget(new QLabel("学生姓名:",this));
    studentComboBox=new QComboBox(this);
    topLayout->addWidget(studentComboBox);

    topLayout->addWidget(new QLabel("起始日期:",this));
    startDateEdit=new QDateEdit(QDate::currentDate().addMonths(-1));
    startDateEdit->setCalendarPopup(true);
    topLayout->addWidget(startDateEdit);

    topLayout->addWidget(new QLabel("结束日期:",this));
    endDateEdit=new QDateEdit(QDate::currentDate());
    endDateEdit->setCalendarPopup(true);
    topLayout->addWidget(endDateEdit);

    addButton=new QPushButton("添加");
    deleteButton=new QPushButton("删除");
    editButton=new QPushButton("修改");
    topLayout->addWidget(addButton);
    topLayout->addWidget(deleteButton);
    topLayout->addWidget(editButton);
    topLayout->addStretch();

    tableWidget=new QTableWidget();
    tableWidget->setFixedWidth(550);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setAlternatingRowColors(true);
    QStringList header=QStringList()<<"ID"<<"学生姓名"<<"缴费日期"<<"金额"<<"支付类型"<<"备注";
    tableWidget->setColumnCount(header.count());
    tableWidget->setHorizontalHeaderLabels(header);
    tableWidget->setColumnHidden(0,true);
    middleLayout->addWidget(tableWidget);
    pieChartView=new QChartView();
    middleLayout->addWidget(pieChartView);
    chartView->setRenderHints(QPainter::Antialiasing);
    chartView->setMinimumHeight(200);

    connect(addButton,&QPushButton::clicked,this,&FinancialWidget::addRecord);
    connect(deleteButton,&QPushButton::clicked,this,&FinancialWidget::deleteRecord);
    connect(editButton,&QPushButton::clicked,this,&FinancialWidget::editRecord);
    connect(studentComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FinancialWidget::loadFinancialRecords);
    connect(startDateEdit,&QDateEdit::dateChanged,this,&FinancialWidget::loadFinancialRecords);
    connect(endDateEdit,&QDateEdit::dateChanged,this,&FinancialWidget::loadFinancialRecords);

}

void FinancialWidget::loadFinancialRecords()
{
    tableWidget->setRowCount(0);
    QString studentId=studentComboBox->currentData().toString();
    QDate startDate=startDateEdit->date();
    QDate endDate=endDateEdit->date();

    QString queryStr = QString(
                           "SELECT fr.id, s.name, fr.payment_date, fr.amount, fr.payment_type, fr.notes " // 补全逗号和空格
                           "FROM financialRecords fr "                   // 补全空格
                           "JOIN studentInfo s ON fr.student_id = s.id " // 修正 stuent_id → student_id
                           "WHERE fr.payment_date BETWEEN '%1' AND '%2' %3 " // 规范关键字大写
                           ).arg(startDate.toString("yyyy-MM-dd"), endDate.toString("yyyy-MM-dd"), (studentId != "-1") ? QString("AND fr.student_id = '%1'").arg(studentId) : "");

    QSqlQuery query(queryStr);

    while(query.next()){
        int row=tableWidget->rowCount();
        tableWidget->insertRow(row);

        for(int col=0;col<6;++col){
            QTableWidgetItem* item =new QTableWidgetItem(query.value(col).toString());
            item->setTextAlignment(Qt::AlignCenter);
            tableWidget->setItem(row,col,item);
        }
    }

    tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    updateChart();
    updatePieChart();
}

void FinancialWidget::populateStudentComboBox()
{
    studentComboBox->clear();
    studentComboBox->addItem("所有学生",QVariant("-1"));

    QSqlQuery query("select id,name from studentInfo");
    while(query.next()){
        QString id =query.value(0).toString();
        QString name = query.value(1).toString();
        studentComboBox->addItem(name,QVariant(id));
    }
}

void FinancialWidget::addRecord()
{
    QDialog dialog(this);
    dialog.setWindowTitle("添加缴费记录");
    QFormLayout form(&dialog);

    QComboBox* studentNameComboBox=new QComboBox(&dialog);
    QSqlQuery query("select id,name from studentInfo");
    while(query.next()){
        QString id =query.value(0).toString();
        QString name=query.value(1).toString();
        studentNameComboBox->addItem(name,QVariant(id));
    }
    QDateEdit* paymentDateEdit=new QDateEdit(&dialog);
    paymentDateEdit->setDate(QDate::currentDate());
    paymentDateEdit->setCalendarPopup(true);

    QLineEdit* amountEdit=new QLineEdit(&dialog);
    QLineEdit* feeTypeEdit=new QLineEdit(&dialog);
    QLineEdit* remarkEdit=new QLineEdit(&dialog);

    form.addRow("学生名称:",studentNameComboBox);
    form.addRow("缴费日期:",paymentDateEdit);
    form.addRow("金额",amountEdit);
    form.addRow("支付类型",feeTypeEdit);
    form.addRow("备注",remarkEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,Qt::Horizontal,&dialog);
    buttonBox.button(QDialogButtonBox::Ok)->setText("确定");
    buttonBox.button(QDialogButtonBox::Cancel)->setText("取消");
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox,&QDialogButtonBox::accepted,&dialog,&QDialog::accept);
    QObject::connect(&buttonBox,&QDialogButtonBox::rejected,&dialog,&QDialog::reject);

    if(dialog.exec()==QDialog::Accepted){
        QString studentId=studentNameComboBox->currentData().toString();
        QString paymentDate=paymentDateEdit->date().toString("yyyy-MM-dd");
        double amount=amountEdit->text().toDouble();
        QString feeType=feeTypeEdit->text();
        QString remark=remarkEdit->text();

        QSqlQuery query;
        query.prepare("insert into financialRecords(student_id,payment_date,amount,payment_type,notes) values(:student_id, :payment_date, :amount, :payment_type, :notes)");
        query.bindValue(":student_id",studentId);
        query.bindValue(":payment_date",paymentDate);
        query.bindValue(":amount",amount);
        query.bindValue(":payment_type",feeType);
        query.bindValue(":notes",remark);

        if(query.exec()){
            qDebug()<<"记录添加成功";
            loadFinancialRecords();
        }else{
            qDebug()<<"添加记录失败"<<query.lastError().text();
        }
    }

}

void FinancialWidget::editRecord()
{
    int currentRow=tableWidget->currentRow();
    if(currentRow<0){
        QMessageBox::warning(this,"警告","请选择要修改的记录!");
        return;
    }
    QString id=tableWidget->item(currentRow,0)->text();
    QString studentName=tableWidget->item(currentRow,1)->text();
    QString paymentDate=tableWidget->item(currentRow,2)->text();
    QString amount=tableWidget->item(currentRow,3)->text();
    QString feeType=tableWidget->item(currentRow,4)->text();
    QString remark=tableWidget->item(currentRow,5)->text();
    QDialog dialog(this);
    dialog.setWindowIconText("修改缴费记录");
    QFormLayout form(&dialog);

    QComboBox* studentNameComboBox=new QComboBox(&dialog);
    QSqlQuery query("select id,name from studentInfo");
    while(query.next()){
        QString id = query.value(0).toString();
        QString name =query.value(1).toString();
        studentNameComboBox->addItem(name,QVariant(id));
    }
    studentNameComboBox->setCurrentText(studentName);
    // 将 QLineEdit* paymentDateEdit 替换为 QDateEdit*
    QDateEdit* paymentDateEdit = new QDateEdit(QDate::fromString(paymentDate, "yyyy-MM-dd"), &dialog);
    paymentDateEdit->setCalendarPopup(true);
    QLineEdit* amountEdit=new QLineEdit(amount,&dialog);
    QLineEdit* feeTypeEdit=new QLineEdit(feeType,&dialog);
    QLineEdit* remarkEdit=new QLineEdit(remark,&dialog);
    form.addRow("学生名称:",studentNameComboBox);
    form.addRow("缴费日期:",paymentDateEdit);
    form.addRow("金额",amountEdit);
    form.addRow("支付类型",feeTypeEdit);
    form.addRow("备注",remarkEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,Qt::Horizontal,&dialog);
    buttonBox.button(QDialogButtonBox::Ok)->setText("确定");
    buttonBox.button(QDialogButtonBox::Cancel)->setText("取消");
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox,&QDialogButtonBox::accepted,&dialog,&QDialog::accept);
    QObject::connect(&buttonBox,&QDialogButtonBox::rejected,&dialog,&QDialog::reject);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox,&QDialogButtonBox::accepted,&dialog,&QDialog::accept);
    QObject::connect(&buttonBox,&QDialogButtonBox::rejected,&dialog,&QDialog::reject);

    if(dialog.exec()==QDialog::Accepted){
        QString studentId=studentNameComboBox->currentData().toString();
        QString paymentDate=paymentDateEdit->text();
        double amount =amountEdit->text().toDouble();
        QString feeType=feeTypeEdit->text();
        QString remark=remarkEdit->text();

        QSqlQuery query;
        query.prepare("UPDATE financialRecords SET student_id = :student_id, payment_date = :payment_date, amount = :amount, payment_type = :payment_type, notes = :notes WHERE id = :id");
        query.bindValue(":student_id",studentId);
        query.bindValue(":payment_date",paymentDate);
        query.bindValue(":amount",amount);
        query.bindValue(":payment_type",feeType);
        query.bindValue(":notes",remark);
        query.bindValue(":id",id);

        if(query.exec()){
            qDebug()<<"记录修改成功";
            loadFinancialRecords();
        }else{
            qDebug()<<"修改记录失败"<<query.lastError().text();
        }

    }


}

void FinancialWidget::deleteRecord()
{
    int currentRow=tableWidget->currentRow();
    if(currentRow<0){
        QMessageBox::warning(this,"警告","请选择要删除的记录");
        return;
    }
    int id=tableWidget->item(currentRow,0)->text().toInt();

    QMessageBox confirmBox(this);
    confirmBox.setWindowTitle("确认删除");
    confirmBox.setText("确定要删除该记录吗?");

    QPushButton* yesButton=confirmBox.addButton("确定",QMessageBox::YesRole);
    QPushButton* noButton=confirmBox.addButton("取消",QMessageBox::NoRole);

    confirmBox.setDefaultButton(noButton);

    confirmBox.exec();

    if(confirmBox.clickedButton()==yesButton){
        QSqlQuery query;
        query.prepare("delete from financialRecords where id =:id");
        query.bindValue(":id",id);

        if(query.exec()){
            qDebug()<<"记录删除成功";
            loadFinancialRecords();
        }else{
            qDebug()<<"删除记录失败:"<<query.lastError().text();
            QMessageBox::warning(this,"错误","删除记录失败!");
        }
    }
}

void FinancialWidget::updateChart()
{
    QString studentId=studentComboBox->currentData().toString();
    QDate startDate=startDateEdit->date();
    QDate endDate=endDateEdit->date();

    QString queryStr = QString("SELECT payment_type, SUM(amount) FROM financialRecords WHERE payment_date BETWEEN '%1' AND '%2' %3 GROUP BY payment_type")
                           .arg(startDate.toString("yyyy-MM-dd"), endDate.toString("yyyy-MM-dd"),
                                (studentId != "-1") ? QString("AND student_id = '%1'").arg(studentId) : "");
    QSqlQuery query(queryStr);

    QPieSeries* series =new QPieSeries();
    qreal totalAmount=0;

    while (query.next()) {
        QString type=query.value(0).toString();
        qreal value=query.value(1).toDouble();
        totalAmount+=value;

        if(value>0){
            QString legendLabel=QString("%1\n%2元").arg(type,value);
            QPieSlice* slice=new QPieSlice(legendLabel,value);
            slice->setLabelVisible(false);
            series->append(slice);
        }

    }

    QChart* chart=new QChart();
    chart->addSeries(series);
    chart->setTitle("支付类型分布");

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setBackgroundVisible(true);
    chart->legend()->setBrush(QBrush(Qt::white));
    chart->legend()->setLabelColor(Qt::black);
    chart->legend()->setContentsMargins(10,10,10,10);

    series->setPieSize(0.75);

    if(pieChartView->chart()){
        delete pieChartView->chart();
    }

    pieChartView->setChart(chart);

    pieChartView->repaint();


}

void FinancialWidget::updatePieChart()
{
    QDate startDate=startDateEdit->date();
    QDate endDate=endDateEdit->date();

    if(startDate>endDate){
        std::swap(startDate,endDate);
        startDateEdit->setDate(startDate);
        endDateEdit->setDate(endDate);
    }

    QString studentId=studentComboBox->currentData().toString();
    QString queryStr = QString("SELECT DATE(payment_date) AS day, SUM(amount) AS total "
                               "FROM financialRecords "
                               "WHERE payment_date BETWEEN :startDate AND :endDate %1 "
                               "GROUP BY day ORDER BY day")
                           .arg((studentId != "-1") ? "AND student_id = :studentId" : "");

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":startDate", startDate);
    query.bindValue(":endDate", endDate);
    if (studentId != "-1") query.bindValue(":studentId", studentId);
    if(!query.exec()) qCritical()<<"[SQL错误]"<<query.lastError().text();

    QMap<QDate,qreal> dayDate;
    qreal maxAmount=0;
    while(query.next()){
        QDate day=QDate::fromString(query.value(0).toString(),"yyyy-MM-dd");
        if(!day.isValid()) continue;
        qreal amount=query.value(1).toDouble();
        dayDate[day]=amount;
        if(amount>maxAmount) maxAmount=amount;
    }

    QLineSeries* series=new QLineSeries();
    series->setName("销售额");
    QPen pen(Qt::blue);
    QDate currentDate=startDate;
    while(currentDate<=endDate){
        qreal value=dayDate.value(currentDate,0.0);
        series->append(currentDate.startOfDay().toMSecsSinceEpoch(),value);
        currentDate=currentDate.addDays(1);
    }

    QChart* chart=new QChart();
    chart->addSeries(series);
    QDateTimeAxis* axisX=new QDateTimeAxis();
    axisX->setFormat("yyyy-MM-dd");
    axisX->setTitleText("日期");
    axisX->setRange(startDate.startOfDay(),endDate.startOfDay());
    chart->addAxis(axisX,Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis* axisY=new QValueAxis();
    axisY->setTitleText("金额(元)");
    axisY->setLabelFormat("%.0f");
    axisY->setTickInterval(10);
    axisY->setRange(0,std::ceil(maxAmount/10)*10+10);
    chart->addAxis(axisY,Qt::AlignLeft);
    series->attachAxis(axisY);

    if(chartView->chart()) delete chartView->chart();
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chart->legend()->setVisible(false);





}
