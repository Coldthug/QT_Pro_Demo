#ifndef STUDENTINFOWIDGET_H
#define STUDENTINFOWIDGET_H

#include <QWidget>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <QPixmap>
#include<QDialog>
#include<QGroupBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QFileDialog>
#include <QStandardPaths>
#include <QBuffer>
#include <QMessageBox>

namespace Ui {
class StudentInfoWidget;
}
class QGroupBox;
class StudentInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StudentInfoWidget(QWidget *parent = nullptr);
    ~StudentInfoWidget();

private slots:
    void on_btn_add_clicked();

    void on_btn_DeleteLine_clicked();

    void on_btn_DeleteItem_clicked();

private:
    Ui::StudentInfoWidget *ui;
    void refreshTable();
    QGroupBox* createFormGroup();
    QGroupBox* createPhotoGroup();
    void handleDialogAccepted(QGroupBox* formGroup,QGroupBox* photoGroup);
    QByteArray photoData;
};

#endif // STUDENTINFOWIDGET_H
