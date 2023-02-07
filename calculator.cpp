#include <QTimer>
#include <vector>
#include "calculator.h"
#include "./ui_calculator.h"
#include "parser.h"

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);
    setStyleSheet("background-color: white;");

    m_inputLine = this->ui->inputLine;
    m_previewLine = this->ui->previewLine;
}

Calculator::~Calculator() {
    delete ui;
}

void Calculator::clearZero() {
    if(!m_inputLine->text().compare("0")) {
        m_inputLine->setText("");
    }
}

bool Calculator::isEndOfLineOperator() {
    if(m_inputLine->text().back() == '+' || m_inputLine->text().back() == '-' || m_inputLine->text().back() == '*' || m_inputLine->text().back() == '/') {
        return true;
    }
    return false;
}

inline void Calculator::delay(int millisecondsWait)
{
    QEventLoop loop;
    QTimer timer;
    timer.connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start(millisecondsWait);
    loop.exec();
}

bool Calculator::isDigit(const QString& expression, int i) {
    std::vector<QChar> digits = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    for(int j = 0; j < digits.size(); j++) {
        if(expression[i] == digits[j]) {
            return true;
        }
    }
    return false;
}

bool Calculator::isOperator(const QString& expression, int i) {
    std::vector<QChar> operators = {'+', '-', '*', '/', '^', '%'};
    for(int j = 0; j < operators.size(); j++) {
        if(expression[i] == operators[j]) {
            return true;
        }
    }
    return false;
}

bool Calculator::isEndOfLineDigit() {
    std::vector<QChar> digits = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    for(int i = 0; i < digits.size(); i++) {
        if(m_inputLine->text().back() == digits[i]) {
            return true;
        }
    }
    return false;
}

void Calculator::on_button_0_clicked() {
    clearZero();

    m_inputLine->setText(m_inputLine->text() + "0");
}

void Calculator::on_button_1_clicked() {
    clearZero();

    m_inputLine->setText(m_inputLine->text() + "1");
}

void Calculator::on_button_2_clicked() {
    clearZero();

    m_inputLine->setText(m_inputLine->text() + "2");
}

void Calculator::on_button_3_clicked() {
    clearZero();

    m_inputLine->setText(m_inputLine->text() + "3");
}

void Calculator::on_button_4_clicked() {
    clearZero();

    m_inputLine->setText(m_inputLine->text() + "4");
}

void Calculator::on_button_5_clicked() {
    clearZero();

    m_inputLine->setText(m_inputLine->text() + "5");
}

void Calculator::on_button_6_clicked() {
    clearZero();

    m_inputLine->setText(m_inputLine->text() + "6");
}

void Calculator::on_button_7_clicked() {
    clearZero();

    m_inputLine->setText(m_inputLine->text() + "7");
}

void Calculator::on_button_8_clicked() {
    clearZero();

    m_inputLine->setText(m_inputLine->text() + "8");
}

void Calculator::on_button_9_clicked() {
    clearZero();

    m_inputLine->setText(m_inputLine->text() + "9");
}

void Calculator::on_buttonClear_clicked() {
    m_inputLine->setText("0");
    m_previewLine->setText("0");
}


void Calculator::on_buttonClearEntity_clicked() {

}


void Calculator::on_buttonErase_clicked() {
    QString inputLineContent = m_inputLine->text();
    if(m_inputLine->text().compare("0")) {
        inputLineContent.chop(1);

        if(inputLineContent.isEmpty()) {
            inputLineContent.insert(0, '0');
        }

        m_inputLine->setText(inputLineContent);
    }
}

void Calculator::on_buttonAdd_clicked() {
    QString inputLineContent = m_inputLine->text();
    if(isEndOfLineOperator()) {
        inputLineContent.chop(1);

        inputLineContent += "+";
        m_inputLine->setText(inputLineContent);
    }
    else if(m_inputLine->text().back() == '(') {
        return;
    }
    else if(m_inputLine->text().compare("0")) {
        m_inputLine->setText(m_inputLine->text() + "+");
    }
}

void Calculator::on_buttonSubtract_clicked() {
    QString inputLineContent = m_inputLine->text();
    if(isEndOfLineOperator()) {
        inputLineContent.chop(1);

        inputLineContent += "-";
        m_inputLine->setText(inputLineContent);
    }
    else if(m_inputLine->text().compare("0")) {
        m_inputLine->setText(m_inputLine->text() + "-");
    }
}

void Calculator::on_buttonMultiply_clicked() {
    QString inputLineContent = m_inputLine->text();
    if(isEndOfLineOperator()) {
        inputLineContent.chop(1);

        inputLineContent += "*";
        m_inputLine->setText(inputLineContent);
    }
    else if(m_inputLine->text().back() == '(') {
        return;
    }
    else if(m_inputLine->text().compare("0")) {
        m_inputLine->setText(m_inputLine->text() + "*");
    }
}

void Calculator::on_buttonDivide_clicked() {
    QString inputLineContent = m_inputLine->text();
    if(isEndOfLineOperator()) {
        inputLineContent.chop(1);

        inputLineContent += "/";
        m_inputLine->setText(inputLineContent);
    }
    else if(m_inputLine->text().back() == '(') {
        return;
    }
    else if(m_inputLine->text().compare("0")) {
        m_inputLine->setText(m_inputLine->text() + "/");
    }
}

void Calculator::on_buttonPercent_clicked() {
    m_inputLine->setText(m_inputLine->text() + "%");
}

void Calculator::on_buttonEqual_clicked() {
    Parser parser;

    QString expression = m_inputLine->text();

    size_t leftParenCounter = 0, rightParenCounter = 0, digitCounter = 0;
    for(int i = 0, delayMilliseconds = 1600; i < expression.length(); i++) {
        if(expression[i] == '(' && expression[i + 1] == ')'
           || expression[i] == '(' && isOperator(expression, i)
           ||  isOperator(expression, i) && expression[i + 1] == ')') {
            m_inputLine->setText("Invalid Syntax");
            delay(1400);
            m_inputLine->setText("0");
            return;
        }
        else if(expression[i] == '(') {
            leftParenCounter++;
        }
        else if(expression[i] == ')') {
            rightParenCounter++;
        }
        else if(isDigit(expression, i)) {
            digitCounter++;
        }
    }
    if(leftParenCounter != rightParenCounter || isEndOfLineOperator() || digitCounter == expression.size()) {
        m_inputLine->setText("Invalid Syntax");
        delay(1400);
        m_inputLine->setText("0");
        return;
    }

    m_previewLine->setText(m_inputLine->text());
    m_inputLine->setText(parser.evaluateExpression(expression));
}

void Calculator::on_buttonParenthesisLeft_clicked() {
    clearZero();

    if(m_inputLine->text().isEmpty()) {
        m_inputLine->setText("(");
    }
    else if(m_inputLine->text().back().isDigit() || m_inputLine->text().back() == ')') {
        m_inputLine->setText(m_inputLine->text() + "*(");
    }
    else {
        m_inputLine->setText(m_inputLine->text() + "(");
    }
}

void Calculator::on_buttonParenthesisRight_clicked() {
    clearZero();

    m_inputLine->setText(m_inputLine->text() + ")");
}

void Calculator::on_buttonPlusMinus_clicked() {
    clearZero();

    m_inputLine->setText(m_inputLine->text() + "(-");
}

void Calculator::on_buttonComma_clicked() {
    QString expression = m_inputLine->text();
    for(int i = expression.length() - 1, operatorCounter = 0, commaCounter = 0; i >= 0; i--) {
        if(operatorCounter > commaCounter) {
            m_inputLine->setText(m_inputLine->text() + ".");
        }
        else if(commaCounter > operatorCounter) {
            return;
        }

        if(isOperator(expression, i)) {
            operatorCounter++;
        }
        else if(expression[i] == '.') {
            commaCounter++;
        }
    }

    if(isEndOfLineDigit()) {
        m_inputLine->setText(m_inputLine->text() + ".");
    }
}

void Calculator::on_buttonReciprocal_clicked() {
    clearZero();

    if(m_inputLine->text().isEmpty()) {
        m_inputLine->setText("1/");
    }
    else if(m_inputLine->text().back().isDigit() || m_inputLine->text().back() == ')') {
        m_inputLine->setText(m_inputLine->text() + "*1/");
    }
    else {
        m_inputLine->setText(m_inputLine->text() + "1/");
    }
}

void Calculator::on_buttonSquare_clicked() {
    m_inputLine->setText(m_inputLine->text() + "^");
}

void Calculator::on_buttonSquareRoot_clicked() {
    clearZero();

    if(m_inputLine->text().isEmpty()) {
        m_inputLine->setText("√(");
    }
    else if(m_inputLine->text().back().isDigit() || m_inputLine->text().back() == ')') {
        m_inputLine->setText(m_inputLine->text() + "*√(");
    }
    else {
        m_inputLine->setText(m_inputLine->text() + "√(");
    }
}

void Calculator::on_buttonCubicRoot_clicked() {
    clearZero();

    if(m_inputLine->text().isEmpty()) {
        m_inputLine->setText("³√(");
    }
    else if(m_inputLine->text().back().isDigit() || m_inputLine->text().back() == ')') {
        m_inputLine->setText(m_inputLine->text() + "*³√(");
    }
    else {
        m_inputLine->setText(m_inputLine->text() + "³√(");
    }
}

void Calculator::on_buttonSinus_clicked() {
    clearZero();

    if(m_inputLine->text().isEmpty()) {
        m_inputLine->setText("sin(");
    }
    else if(m_inputLine->text().back().isDigit() || m_inputLine->text().back() == ')') {
        m_inputLine->setText(m_inputLine->text() + "*sin(");
    }
    else {
        m_inputLine->setText(m_inputLine->text() + "sin(");
    }
}

void Calculator::on_buttonCosinus_clicked() {
    clearZero();

    if(m_inputLine->text().isEmpty()) {
        m_inputLine->setText("cos(");
    }
    else if(m_inputLine->text().back().isDigit() || m_inputLine->text().back() == ')') {
        m_inputLine->setText(m_inputLine->text() + "*cos(");
    }
    else {
        m_inputLine->setText(m_inputLine->text() + "cos(");
    }
}

void Calculator::on_buttonTangens_clicked() {
    clearZero();

    if(m_inputLine->text().isEmpty()) {
        m_inputLine->setText("tan(");
    }
    else if(m_inputLine->text().back().isDigit() || m_inputLine->text().back() == ')') {
        m_inputLine->setText(m_inputLine->text() + "*tan(");
    }
    else {
        m_inputLine->setText(m_inputLine->text() + "tan(");
    }
}
