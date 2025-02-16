#ifndef STANDARD_H
#define STANDARD_H

#include <QWidget>
#include <QStack>
#include <cmath>
namespace Ui {
class Standard;
}

class Standard : public QWidget
{
    Q_OBJECT

public:
    explicit Standard(QWidget *parent = nullptr);
    ~Standard();


private slots:

    void on_btn_0_clicked();
    void on_btn_1_clicked();
    void on_btn_2_clicked();
    void on_btn_3_clicked();
    void on_btn_4_clicked();
    void on_btn_5_clicked();
    void on_btn_6_clicked();
    void on_btn_7_clicked();
    void on_btn_8_clicked();
    void on_btn_9_clicked();

    void on_btn_CE_clicked();

    void on_btn_sign_clicked();

    void on_btn_decimal_clicked();

    void on_btn_equals_clicked();

    void on_btn_add_clicked();

    void on_btn_subtract_clicked();

    void on_btn_multiply_clicked();

    void on_btn_divide_clicked();

    void on_btn_sqrt_clicked();

    void on_btn_pow2_clicked();

    void on_btn_over_clicked();

    void on_btn_DEL_clicked();

    void on_btn_E_clicked();

    void on_btn_remainder_clicked();

private:
    Ui::Standard *ui;

    QString expression="";
    QStack<double> numbers;
    QStack<QChar> operators;

    double evaluateExpression(const QString &expression);
    int precedence(QChar op);
    void applyOperation(QStack<double> &numbers, QChar op);

};

#endif // STANDARD_H
