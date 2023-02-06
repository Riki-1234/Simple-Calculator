#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "qlabel.h"
#include "ui_calculator.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Calculator; }
QT_END_NAMESPACE

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

private slots:
    void on_button_0_clicked();
    void on_button_1_clicked();
    void on_button_2_clicked();
    void on_button_3_clicked();
    void on_button_4_clicked();
    void on_button_5_clicked();
    void on_button_6_clicked();
    void on_button_7_clicked();
    void on_button_8_clicked();
    void on_button_9_clicked();

    void on_buttonClear_clicked();
    void on_buttonClearEntity_clicked();
    void on_buttonErase_clicked();

    void on_buttonAdd_clicked();
    void on_buttonSubtract_clicked();
    void on_buttonMultiply_clicked();
    void on_buttonDivide_clicked();
    void on_buttonPercent_clicked();

    void on_buttonEqual_clicked();
    void on_buttonParenthesisLeft_clicked();
    void on_buttonParenthesisRight_clicked();
    void on_buttonPlusMinus_clicked();
    void on_buttonComma_clicked();
    void on_buttonReciprocal_clicked();

    void on_buttonSquare_clicked();
    void on_buttonSquareRoot_clicked();
    void on_buttonCubicRoot_clicked();

    void on_buttonSinus_clicked();
    void on_buttonCosinus_clicked();
    void on_buttonTangens_clicked();
private:
    Ui::Calculator *ui;

    void clearZero();
    bool isOperator();
    void delay(int millisecondsWait);
    bool isDigit(const QString& expression, int i);

    QLabel* m_inputLine;
    QLabel* m_previewLine;
};
#endif // CALCULATOR_H
