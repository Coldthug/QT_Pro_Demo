#include "studentinfowidget.h"
#include "ui_studentinfowidget.h"
#include "tabledelegates.h"
#include <qlabel.h>
#include <qsqlerror.h>

StudentInfoWidget::StudentInfoWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StudentInfoWidget)
{
    ui->setupUi(this);

    ui->studentInfoTableWidget->verticalHeader()->setDefaultSectionSize(80);
    ComboBoxDelegate* genderDelegate =new ComboBoxDelegate(this);
    genderDelegate->setItems(QStringList()<<"男"<<"女");
    ui->studentInfoTableWidget->setItemDelegateForColumn(2,genderDelegate);

    ComboBoxDelegate* progressDelegate =new ComboBoxDelegate(this);
    progressDelegate->setItems(QStringList()<<"0%"<<"20%"<<"40%"<<"60%"<<"80%"<<"100%");
    ui->studentInfoTableWidget->setItemDelegateForColumn(6,progressDelegate);

    ui->studentInfoTableWidget->setItemDelegateForColumn(3,new DateEditDelegate(this));
    ui->studentInfoTableWidget->setItemDelegateForColumn(4,new DateEditDelegate(this));

    ui->studentInfoTableWidget->setItemDelegateForColumn(7, new ImageDelegate(this));

    connect(ui->studentInfoTableWidget,&QTableWidget::itemChanged,this,&StudentInfoWidget::handleItemChange);
    refreshTable();
}

StudentInfoWidget::~StudentInfoWidget()
{
    delete ui;
}



void StudentInfoWidget::refreshTable()
{
    ui->studentInfoTableWidget->blockSignals(true); // 阻塞信号，避免重复刷新
    ui->studentInfoTableWidget->setRowCount(0); // 清空表格

    QSqlQuery query("SELECT * FROM studentInfo"); // 查询所有学生信息
    if (!query.exec()) {
        qWarning() << "查询失败:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        int row = ui->studentInfoTableWidget->rowCount();
        ui->studentInfoTableWidget->insertRow(row);

        for (int col = 0; col < ui->studentInfoTableWidget->columnCount(); ++col) {
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setTextAlignment(Qt::AlignCenter);

            // 处理照片列
            if (col == ui->studentInfoTableWidget->columnCount() - 1) {
                QByteArray photoData = query.value(col).toByteArray();
                if (!photoData.isEmpty()) {
                    QPixmap photo;
                    photo.loadFromData(photoData);
                    item->setData(Qt::DecorationRole, photo.scaled(100, 100, Qt::KeepAspectRatio));
                    item->setData(Qt::UserRole, photoData);
                }
            } else {
                item->setText(query.value(col).toString()); // 设置文本数据
            }

            ui->studentInfoTableWidget->setItem(row, col, item); // 将单元格添加到表格
        }
    }

    ui->studentInfoTableWidget->blockSignals(false); // 恢复信号
}

QGroupBox* StudentInfoWidget::createFormGroup()
{
    QGroupBox* formGroup = new QGroupBox(tr("基本信息"));
    QFormLayout* formlayout = new QFormLayout();

    // 创建控件
    QLineEdit* idEdit = new QLineEdit();
    idEdit->setObjectName("idEdit");
    QLineEdit* nameEdit = new QLineEdit();
    nameEdit->setObjectName("nameEdit");
    QComboBox* genderCombo = new QComboBox();
    genderCombo->setObjectName("genderCombo");
    QDateEdit* birthdayEdit = new QDateEdit();
    birthdayEdit->setObjectName("birthdayEdit");
    QDateEdit* joinDateEdit = new QDateEdit();
    joinDateEdit->setObjectName("joinDateEdit");
    QLineEdit* goalEdit = new QLineEdit();
    goalEdit->setObjectName("goalEdit");
    QComboBox* progressCombo = new QComboBox();
    progressCombo->setObjectName("progressCombo");

    // 设置控件属性
    genderCombo->addItems({tr("男"), tr("女")});
    progressCombo->addItems({tr("0%"), tr("20%"), tr("40%"), tr("60%"), tr("80%"), tr("100%")});
    birthdayEdit->setDisplayFormat("yyyy-MM-dd");
    joinDateEdit->setDisplayFormat("yyyy-MM-dd");
    birthdayEdit->setCalendarPopup(true);
    joinDateEdit->setCalendarPopup(true);

    // 添加控件到布局
    formlayout->addRow(tr("编号"), idEdit);
    formlayout->addRow(tr("姓名"), nameEdit);
    formlayout->addRow(tr("性别"), genderCombo);
    formlayout->addRow(tr("出生日期"), birthdayEdit);
    formlayout->addRow(tr("加入时间"), joinDateEdit);
    formlayout->addRow(tr("学习目标"), goalEdit);
    formlayout->addRow(tr("当前进度"), progressCombo);

    // 设置布局
    formGroup->setLayout(formlayout);
    return formGroup;
}

QGroupBox* StudentInfoWidget::createPhotoGroup()
{
    QGroupBox* photoGroup = new QGroupBox(tr("照片上传"));
    QVBoxLayout* photoLayout = new QVBoxLayout(photoGroup);

    QLabel* lblPhotoPreview = new QLabel();
    QPushButton* btnSelectPhoto = new QPushButton(tr("选择照片"));

    lblPhotoPreview->setAlignment(Qt::AlignCenter);
    lblPhotoPreview->setMinimumSize(200, 200);
    btnSelectPhoto->setFixedSize(100, 40);

    photoLayout->addWidget(lblPhotoPreview);
    photoLayout->addWidget(btnSelectPhoto, 0, Qt::AlignHCenter);

    connect(btnSelectPhoto, &QPushButton::clicked, [this, lblPhotoPreview]() {
        QString fileName = QFileDialog::getOpenFileName(
            this, tr("选择学生照片"),
            QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
            tr("图片文件(*.png *.jpg *.jpeg)"));
        if (!fileName.isEmpty()) {
            QPixmap pixmap(fileName);
            if (!pixmap.isNull()) {
                pixmap = pixmap.scaled(200, 200, Qt::KeepAspectRatio); // 使用固定大小
                lblPhotoPreview->setPixmap(pixmap);

                QBuffer buffer(&photoData);
                if (buffer.open(QIODevice::WriteOnly)) {
                    pixmap.save(&buffer, "PNG");
                } else {
                    QMessageBox::warning(this, tr("错误"), tr("无法打开缓冲区"));
                }
            } else {
                QMessageBox::warning(this, tr("错误"), tr("无法加载图片文件"));
            }
        }
    });

    photoGroup->setLayout(photoLayout);
    return photoGroup;
}

void StudentInfoWidget::handleDialogAccepted(QGroupBox* formGroup, QGroupBox* photoGroup)
{
    QLineEdit* idEdit = formGroup->findChild<QLineEdit*>("idEdit");
    QLineEdit* nameEdit = formGroup->findChild<QLineEdit*>("nameEdit");
    QComboBox* genderCombo = formGroup->findChild<QComboBox*>("genderCombo");
    QDateEdit* birthdayEdit = formGroup->findChild<QDateEdit*>("birthdayEdit");
    QDateEdit* joinDateEdit = formGroup->findChild<QDateEdit*>("joinDateEdit");
    QLineEdit* goalEdit = formGroup->findChild<QLineEdit*>("goalEdit");
    QComboBox* progressCombo = formGroup->findChild<QComboBox*>("progressCombo");


    if(idEdit->text().isEmpty()||nameEdit->text().isEmpty()){
        QMessageBox::warning(this,tr("错误"),tr("学号和姓名不能为空"));
        return;
    }

    QSqlQuery checkQuery;
    checkQuery.prepare("select id From studentInfo Where id =?");
    checkQuery.addBindValue(idEdit->text());
    if(checkQuery.exec()&&checkQuery.next()){
        QMessageBox::warning(this,tr("错误"),tr("学号%1 已存在").arg(idEdit->text()));
        return;
    }

    QSqlDatabase::database().transaction();
    QSqlQuery insertQuery;
    insertQuery.prepare("insert into studentInfo (id,name,gender,birthday,join_date,student_goal,progress,photo) values(?,?,?,?,?,?,?,?)");

    insertQuery.addBindValue(idEdit->text());
    insertQuery.addBindValue(nameEdit->text()); ;
    insertQuery.addBindValue(genderCombo->currentText()) ;
    insertQuery.addBindValue(birthdayEdit->date().toString("yyyy-MM-dd")) ;
    insertQuery.addBindValue(joinDateEdit->date().toString("yyyy-MM-dd")) ;
    insertQuery.addBindValue(goalEdit->text());
    insertQuery.addBindValue(progressCombo->currentText());
    insertQuery.addBindValue(photoData.isEmpty()? QVariant():photoData);

    if(!insertQuery.exec()){
        QSqlDatabase::database().rollback();
        QMessageBox::critical(this,tr("错误"),tr("添加失败")+insertQuery.lastError().text());
    }else{
        QSqlDatabase::database().commit();
        refreshTable();
        QMessageBox::information(this,tr("成功"),tr("已成功添加学生:%1").arg(nameEdit->text()));
    }
}

void StudentInfoWidget::handleItemChange(QTableWidgetItem *item)
{
    const int row=item->row();
    const int col=item->column();

    if(col==0){
        QMessageBox::warning(this,"警告","学生编号是主键，不能修改");
        refreshTable();
        return;
    }

    const QString originalId=ui->studentInfoTableWidget->item(row,0)->text();
    const QString columnName=QStringList{"id","name","gender","birthday","join_date","student_goal","progress","photo"}[col];

    QSqlDatabase::database().transaction();
    try{
        QSqlQuery updateQuery;
        updateQuery.prepare(QString("update studentInfo set %1 =? where id =?").arg(columnName));

        if(columnName=="photo"){
            updateQuery.addBindValue(item->data(Qt::UserRole).toByteArray());
        }else{
            updateQuery.addBindValue(item->text().trimmed());
        }
        updateQuery.addBindValue(originalId);

        if(!updateQuery.exec()){
            throw std::runtime_error("更新失败:"+ updateQuery.lastError().text().toStdString());
        }
        QSqlDatabase::database().commit();
    }catch(const std::exception& e){
        QSqlDatabase::database().rollback();
        refreshTable();
        QMessageBox::critical(this,"操作失败",QString::fromUtf8(e.what()));
    }
}
void StudentInfoWidget::on_btn_add_clicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("添加学生信息");
    dlg.setMinimumSize(600, 400);

    QVBoxLayout* mainLayout = new QVBoxLayout(&dlg);
    QHBoxLayout* contentLayout = new QHBoxLayout();

    QGroupBox* formGroup = createFormGroup();
    QGroupBox* photoGroup = createPhotoGroup();
    contentLayout->addWidget(formGroup, 1);
    contentLayout->addWidget(photoGroup, 1);

    mainLayout->addLayout(contentLayout); // 将 contentLayout 添加到 mainLayout

    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* btnConfirm = new QPushButton(tr("确认"));
    QPushButton* btnCancel = new QPushButton(tr("取消"));

    btnConfirm->setFixedWidth(150);
    btnCancel->setFixedWidth(150);

    btnLayout->addStretch();
    btnLayout->addWidget(btnConfirm);
    btnLayout->addWidget(btnCancel);
    btnLayout->addStretch();

    connect(btnConfirm, &QPushButton::clicked, &dlg, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, &dlg, &QDialog::reject);
    mainLayout->addLayout(btnLayout);

    if (dlg.exec() == QDialog::Accepted) {
        handleDialogAccepted(formGroup, photoGroup);
    }
}


void StudentInfoWidget::on_btn_DeleteLine_clicked()
{
    auto selected =ui->studentInfoTableWidget->selectionModel()->selectedRows();
    if(selected.isEmpty()){
        QMessageBox::warning(this,"警告","请选择要删除的行");
        return;
    }
    QSqlDatabase::database().transaction();
    foreach (const QModelIndex &index, selected) {
        QString id = ui->studentInfoTableWidget->item(index.row(),0)->text();
        QSqlQuery query;
        query.prepare("Delete from studentInfo where id =?");
        query.addBindValue(id);
        if(!query.exec()){
            QSqlDatabase::database().rollback();
            QMessageBox::critical(this,"错误","删除失败"+query.lastError().text());
        }
    }
    QSqlDatabase::database().commit();
    refreshTable();
}


void StudentInfoWidget::on_btn_DeleteItem_clicked()
{
    auto selected=ui->studentInfoTableWidget->selectedItems();
    if(selected.isEmpty()){
        QMessageBox::warning(this,"警告","请选择要删除的单元格");
        return ;
    }

    QSqlDatabase::database().transaction();
    foreach (QTableWidgetItem* item, selected) {
        int row =item->row();
        int col=item->column();
        QString id=ui->studentInfoTableWidget->item(row,0)->text();
        const QStringList columns={"id","name","gender","birthday","join_date","student_goal","progress","photo"};
        QSqlQuery query;
        query.prepare(QString("update studentInfo set %1 =? where id = ?").arg(columns[col]));

        query.addBindValue("");
        query.addBindValue(id);

        if(!query.exec()){
            QSqlDatabase::database().rollback();
            QMessageBox::critical(this,"错误","更新失败"+query.lastError().text());

        }
    }
    QSqlDatabase::database().commit();
    refreshTable();

}

