#include "Standard.h"
#include "ui_Standard.h"

Standard::Standard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Standard)
{

    ui->setupUi(this);
    ui->lineEdit->setText(expression);
    numbers = {};
    operators={};
}

Standard::~Standard()
{
    delete ui;
}


double Standard::evaluateExpression(const QString &expression) {


    for (int i = 0; i < expression.length(); ++i) {
        QChar ch = expression.at(i);

        if (ch.isDigit() || ch == '.') {
            // 处理数字
            QString numStr;
            while (i < expression.length() && (expression.at(i).isDigit() || expression.at(i) == '.')) {
                numStr += expression.at(i);
                ++i;
            }
            --i; // 回退一步
            numbers.push(numStr.toDouble());
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            // 处理操作符
            while (!operators.isEmpty() && precedence(operators.top()) >= precedence(ch)) {
                applyOperation(numbers, operators.pop());
            }
            operators.push(ch);
        }
    }

    // 处理剩余的操作符
    while (!operators.isEmpty()) {
        applyOperation(numbers, operators.pop());
    }

    if (numbers.size() != 1) {
        throw std::runtime_error("Invalid expression");
    }

    return numbers.pop();
}

int Standard::precedence(QChar op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

void Standard::applyOperation(QStack<double> &numbers, QChar op) {
    if (numbers.size() < 2) {
        throw std::runtime_error("Invalid expression");
    }

    double num2 = numbers.pop();
    double num1 = numbers.pop();

    if (op == '+') numbers.push(num1 + num2);
    else if (op == '-') numbers.push(num1 - num2);
    else if (op == '*') numbers.push(num1 * num2);
    else if (op == '/') {
        if (num2 == 0) {
            throw std::runtime_error("Division by zero");
        }
        numbers.push(num1 / num2);
    }
}




// 运算表达式追加
void Standard::on_btn_0_clicked()
{

    expression.append('0');
    ui->lineEdit->setText(expression);
}

void Standard::on_btn_1_clicked()
{
    expression.append('1');
    ui->lineEdit->setText(expression);
}

void Standard::on_btn_2_clicked()
{
    expression.append('2');
    ui->lineEdit->setText(expression);
}
void Standard::on_btn_3_clicked()
{
    expression.append('3');
    ui->lineEdit->setText(expression);
}

void Standard::on_btn_4_clicked()
{
    expression.append('4');
    ui->lineEdit->setText(expression);
}

void Standard::on_btn_5_clicked()
{
    expression.append('5');
    ui->lineEdit->setText(expression);
}
void Standard::on_btn_6_clicked()
{
    expression.append('6');
    ui->lineEdit->setText(expression);
}

void Standard::on_btn_7_clicked()
{
    expression.append('7');
    ui->lineEdit->setText(expression);
}

void Standard::on_btn_8_clicked()
{
    expression.append('8');
    ui->lineEdit->setText(expression);
}
void Standard::on_btn_9_clicked()
{
    expression.append('9');
    ui->lineEdit->setText(expression);
}

void Standard::on_btn_CE_clicked()
{
    expression.clear();
    numbers.clear();
    operators.clear();
}


void Standard::on_btn_sign_clicked() {
    if (!expression.isEmpty()) {
        if (expression.startsWith('-')) {
            expression.remove(0, 1); // 去掉负号
        } else {
            expression.prepend('-'); // 添加负号
        }
        ui->lineEdit->setText(expression); // 更新 UI
    }
}

void Standard::on_btn_decimal_clicked()
{
    if (!expression.isEmpty() && !expression.endsWith('.')) {
        expression.append('.');
    }
}


void Standard::on_btn_equals_clicked() {
    try {
        double result = evaluateExpression(expression);
        ui->lineEdit->setText(QString::number(result)); // 显示结果
        expression = QString::number(result); // 更新 expression
    } catch (const std::exception &e) {
        ui->lineEdit->setText("Error"); // 显示错误信息
        expression.clear(); // 清空表达式
    }
}

void Standard::on_btn_add_clicked()
{
    expression.append('+');
    ui->lineEdit->setText(expression);
}

void Standard::on_btn_subtract_clicked()
{
    expression.append('-');
    ui->lineEdit->setText(expression);
}

void Standard::on_btn_multiply_clicked()
{
    expression.append('*');
    ui->lineEdit->setText(expression);
}

void Standard::on_btn_divide_clicked()
{
    expression.append('/');
    ui->lineEdit->setText(expression);
}

void Standard::on_btn_sqrt_clicked()
{
    double num = expression.toDouble();
    if (num >= 0) {
        expression = QString::number(sqrt(num));
    } else {
        expression = "Error";  // 负数的平方根无效
    }
}


void Standard::on_btn_pow2_clicked()
{
    double num = expression.toDouble();
    expression = QString::number(num * num);
}
void Standard::on_btn_over_clicked() {
    if (!expression.isEmpty()) {
        double num = expression.toDouble();
        if (num != 0) {
            expression = QString::number(1.0 / num); // 计算倒数
            ui->lineEdit->setText(expression); // 更新 UI
        } else {
            ui->lineEdit->setText("Error"); // 除零错误
            expression.clear();
        }
    }
}
void Standard::on_btn_DEL_clicked()
{
    if (!expression.isEmpty()) {
        expression.chop(1); // 删除最后一个字符
        ui->lineEdit->setText(expression);
    }
}
void Standard::on_btn_remainder_clicked() {
    if (!expression.isEmpty()) {
        expression.append('%'); // 添加求余符号
        ui->lineEdit->setText(expression); // 更新 UI
    }
}

void Standard::on_btn_E_clicked() {
    if (!expression.isEmpty()) {
        expression.append('E'); // 添加科学计数法符号
        ui->lineEdit->setText(expression); // 更新 UI
    }
}
