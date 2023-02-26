#include "parser.h"
#include <string>
#include <vector>
#include <utility>
#include <cmath>
#include <iostream>

bool Parser::isSquareCubicRoot(const std::string& expression, int i) {
    return (expression[i] == 's' && expression[i + 1] == 'q' || expression[i] == 'c' && expression[i + 1] == 'b');
}

bool Parser::isSinCosTan(const std::string& expression, int i) {
    return (expression[i] == 's' && expression[i + 1] == 'i' || expression[i] == 'c' && expression[i + 1] == 'o' || expression[i] == 't' && expression[i + 1] == 'a');
}

bool Parser::isTopOperator() {
    return (m_operators.top() == "^" || m_operators.top() == "sqrt" || m_operators.top() == "cbrt" || m_operators.top() == "sin" || m_operators.top() == "cos" || m_operators.top() == "tan");
}

bool Parser::isTopHighPrecedenceOperator() {
    if (m_operators.size() > 0) {
        return (m_operators.top() == "sqrt" || m_operators.top() == "cbrt" || m_operators.top() == "sin" || m_operators.top() == "cos" || m_operators.top() == "tan" || m_operators.top() == "^");
    }
    return false;
}

bool Parser::isDigit(const std::string& expression, int i) {
    return (std::isdigit(expression[i]) || expression[i] == 'n' && !isSinCosTan(expression, i + 1) && !isSquareCubicRoot(expression, i + 1) || expression[i] == '.');
}

bool Parser::isAdditionMultiplicationDivision(const std::string& expression, int i) {
    return (expression[i] == '+' || expression[i] == '*' || expression[i] == '/');
}

bool Parser::isOperator(const std::string& expression, int i) {
    return (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/' || expression[i] == '^' || expression[i] == '%');
}

bool Parser::isOperatorNoPercent(const std::string& expression, int i) {
    return (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/' || expression[i] == '^');
}

bool Parser::isOperatorNoMinus(const std::string& expression, int i) {
    return (expression[i] == '+' || expression[i] == '*' || expression[i] == '/' || expression[i] == '^' || expression[i] == '%');
}

bool Parser::isMissingMultiplication(const std::string& expression, int i) {
    return (std::isdigit(expression[i]) && expression[i + 1] == '('
            || expression[i] == ')' && std::isdigit(expression[i + 1])
            || expression[i] == ')' && expression[i + 1] == '('
            || expression[i] == '%' && expression[i + 1] == '(');
}

bool Parser::isPointlessExpression(const std::string& expression) {
    int operatorCounter = 0;
    for(int i = 0; i < expression.length(); i++) {
        if(i != expression.length() - 1) {
            if(expression[i] == '(' && expression[i + 1] == '-') {
                if(i == 0) {
                    i++;
                }
                i++;
                continue;
            }
        }
        if(isOperator(expression, i) || isSquareCubicRoot(expression, i) || isSinCosTan(expression, i)) {
            operatorCounter++;
        }
    }
    if(operatorCounter > 0) {
        return false;
    }
    return true;
}

void Parser::lexMultiplicationAndDivision(const std::string& expression, int i) {
    if (m_operators.size() > 0) {
        if (isTopOperator() || m_operators.top() == "*" || m_operators.top() == "/") {
            m_operands.push(m_operators.top());
            m_operators.pop();
            m_operators.push(std::string(1, expression[i]));
        }
        else {
            m_operators.push(std::string(1, expression[i]));
        }
    }
    else {
        m_operators.push(std::string(1, expression[i]));
    }
}

void Parser::lexNumbers(std::string& expression, int& i) {
    std::string multiDigitNum{};
    if (std::isdigit(expression[i + 1]) || expression[i] == 'n' && !isSinCosTan(expression, i + 1) && !isSquareCubicRoot(expression, i + 1) || expression[i + 1] == '.') {
        while (isDigit(expression, i)) {
            if (expression[i] == 'n') {
                expression.replace(i, 1, 1, '-');
            }
            multiDigitNum += std::string(1, expression[i]);
            i++;
        }
        m_operands.push(multiDigitNum);
        i--;
    }
    else {
        m_operands.push(std::string(1, expression[i]));
    }
}

void Parser::lexAdditionAndSubtraction(const std::string& expression, int i) {
    if (m_operators.empty()) {
        m_operators.push(std::string(1, expression[i]));
    }
    else if (m_operators.top() == "*" || m_operators.top() == "/" || isTopHighPrecedenceOperator()) {
        while (m_operators.size() != 0) {
            m_operands.push(m_operators.top());
            m_operators.pop();
        }
        m_operators.push(std::string(1, expression[i]));
    }
    else if (m_operators.top() == "+" || m_operators.top() == "-") {
        m_operands.push(m_operators.top());
        m_operators.pop();
        m_operators.push(std::string(1, expression[i]));
    }
    else {
        m_operators.push(std::string(1, expression[i]));
    }
}

void Parser::lexSinCosTan(std::string& expression, int& i) {
    if (isTopHighPrecedenceOperator()) {
        m_operands.push(m_operators.top());
        m_operators.pop();
        m_operators.push(std::string() + expression[i] + expression[i + 1] + expression[i + 2]);
    }
    else if (expression[i] == 'n') {
        expression.replace(i, 1, 1, '-');

        m_operators.push(std::string() + expression[i] + expression[i + 1] + expression[i + 2] + expression[i + 3]);
    }
    else if (i > 0) {
        if (expression[i - 1] != '-') {
            m_operators.push(std::string() + expression[i] + expression[i + 1] + expression[i + 2]);
        }
    }
    else if (i == 0) {
        m_operators.push(std::string() + expression[i] + expression[i + 1] + expression[i + 2]);
    }
    i += 2;
}

void Parser::lexSqrtCbrt(std::string& expression, int& i) {
    if (isTopHighPrecedenceOperator()) {
        m_operands.push(m_operators.top());
        m_operators.pop();
        m_operators.push(std::string() + expression[i] + expression[i + 1] + expression[i + 2] + expression[i + 3]);
    }
    else if (expression[i] == 'n') {
        expression.replace(i, 1, 1, '-');

        m_operators.push(std::string() + expression[i] + expression[i + 1] + expression[i + 2] + expression[i + 3] + expression[i + 4]);
    }
    else if (i > 0) {
        if (expression[i - 1] != '-') {
            m_operators.push(std::string() + expression[i] + expression[i + 1] + expression[i + 2] + expression[i + 3]);
        }
    }
    else if (i == 0) {
        m_operators.push(std::string() + expression[i] + expression[i + 1] + expression[i + 2] + expression[i + 3]);
    }
    i += 3;
}

void Parser::replacePercentWithDivision(std::string& expression) {
    while (expression.find("%") != std::string::npos) {
        int percentIndex{};
        for (int i = 0; i < expression.size(); i++) {
            if (expression[i] == '%') {
                percentIndex = i;
            }
        }

        expression.insert(expression.begin() + (percentIndex + 1), '1');
        expression.insert(expression.begin() + (percentIndex + 2), '0');
        expression.insert(expression.begin() + (percentIndex + 3), '0');

        expression.replace(expression.find("%"), 1, 1, '/');
    }
}

void Parser::addMissingMultiplication(std::string& expression) {
    for (int i = 0; i < expression.size(); i++) {
        if (isMissingMultiplication(expression, i)) {
            expression.insert(expression.begin() + (i + 1), '*');
        }
    }
}

void Parser::solveMultipleMinuses(std::string& expression) {
    int minusCounter = 0, minusIndex;
    for (int i = 0; i < expression.length(); i++) {
        if (expression[i] == '(' && expression[i + 1] == '-') {
            minusCounter++;

            expression.erase(expression.begin() + (i + 1));
            minusIndex = i + 1;
        }
        else if (i == 0 && expression[i] == '-' && expression[i + 1] == '(') {
            minusCounter++;

            expression.erase(expression.begin() + i);
            minusIndex = i;
            i--;
        }
        else if (expression[i] == '-' && expression[i + 1] == '(' && minusCounter != 0) {
            minusCounter++;

            expression.erase(expression.begin() + i);
            minusIndex = i;
        }
    }

    if (minusCounter % 2 != 0) {
        expression.insert(expression.begin() + minusIndex, '-');
    }
}

void Parser::eraseWhiteSpaces(std::string& expression) {
    for (int i = 0; i < expression.length(); i++) {
        if (expression[i] == ' ') {
            expression.erase(expression.begin() + i);
            if(i == 0) {
                i = -1;
            }
            else {
                i--;
            }
        }
    }
}

void Parser::eraseBrackets(std::string& expression) {
    for(int i = 0; i < expression.length(); i++) {
        if(expression[i] == '(' || expression[i] == ')') {
            expression.erase(expression.begin() + i);
            if(i == 1) {
                i = 0;
            }
            else {
                i--;
            }
        }
    }
}

void Parser::shuntingYardAlgorithm(QString& inputLineContent, bool& isPointless) {
    std::string expression = inputLineContent.toStdString();
    eraseWhiteSpaces(expression);
    if(isPointlessExpression(expression)) {
        solveMultipleMinuses(expression);
        eraseBrackets(expression);
        m_sortedExpression.push_back(expression);
        isPointless = true;
        return;
    }
    replacePercentWithDivision(expression);
    addMissingMultiplication(expression);
    solveMultipleMinuses(expression);

    if (expression[0] == '-') {
        expression.replace(0, 1, 1, 'n');
    }

    for (int i = 0; i < expression.length(); i++) {
        if (expression[i] == '+' || expression[i] == '-') {
            lexAdditionAndSubtraction(expression, i);
        }
        else if (expression[i] == '*' || expression[i] == '/') {
            lexMultiplicationAndDivision(expression, i);
        }
        else if (isDigit(expression, i)) {
            lexNumbers(expression, i);
        }
        else if (expression[i] == '(') {
            if (expression[i + 1] == '-') {
                expression.replace(i + 1, 1, 1, 'n');
            }
            m_operators.push(std::string(1, expression[i]));
        }
        else if (expression[i] == ')') {
            while (m_operators.top() != "(") {
                m_operands.push(m_operators.top());
                m_operators.pop();
            }
            m_operators.pop();
        }
        else if (expression[i] == '^') {
            if (isTopHighPrecedenceOperator()) {
                m_operands.push(m_operators.top());
                m_operators.pop();
                m_operators.push(std::string(1, expression[i]));
            }
            else {
                m_operators.push(std::string(1, expression[i]));
            }
        }
        else if (isSquareCubicRoot(expression, i) || isSquareCubicRoot(expression, i + 1) && expression[i] == 'n') {
            lexSqrtCbrt(expression, i);
        }
        else if (isSinCosTan(expression, i) || isSinCosTan(expression, i + 1) && expression[i] == 'n') {
            lexSinCosTan(expression, i);
        }
    }
    while (m_operators.size() != 0) {
        m_operands.push(m_operators.top());
        m_operators.pop();
    }
}

void Parser::sortExpression() {
    while (m_operands.size() != 0) {
        m_sortedExpression.push_back(m_operands.top());
        m_operands.pop();
    }
    std::reverse(m_sortedExpression.begin(), m_sortedExpression.end());

    for (int i = 0; i < m_sortedExpression.size(); i++) {
        if (m_sortedExpression[i] == "(" || m_sortedExpression[i] == ")") {
            m_sortedExpression.erase(m_sortedExpression.begin() + i);
        }
    }
}

void Parser::_calculateExpression(int i) {
    if (m_sortedExpression[i + 1] == "sqrt") {
        m_sortedExpression[i] = std::to_string(std::sqrt(std::stod(m_sortedExpression[i])));
        m_sortedExpression.erase(m_sortedExpression.begin() + (i + 1));
        i = 0;
    }
    else if (m_sortedExpression[i + 1] == "-sqrt") {
        m_sortedExpression[i] = std::to_string(std::sqrt(std::stod(m_sortedExpression[i])) * -1);
        m_sortedExpression.erase(m_sortedExpression.begin() + (i + 1));
        i = 0;
    }
    else if (m_sortedExpression[i + 1] == "cbrt") {
        m_sortedExpression[i] = std::to_string(std::cbrt(std::stod(m_sortedExpression[i])));
        m_sortedExpression.erase(m_sortedExpression.begin() + (i + 1));
        i = 0;
    }
    else if (m_sortedExpression[i + 1] == "-cbrt") {
        m_sortedExpression[i] = std::to_string(std::cbrt(std::stod(m_sortedExpression[i])) * -1);
        m_sortedExpression.erase(m_sortedExpression.begin() + (i + 1));
        i = 0;
    }
    else if (m_sortedExpression[i + 1] == "sin") {
        m_sortedExpression[i] = std::to_string(std::sin(std::stod(m_sortedExpression[i])));
        m_sortedExpression.erase(m_sortedExpression.begin() + (i + 1));
        i = 0;
    }
    else if (m_sortedExpression[i + 1] == "-sin") {
        m_sortedExpression[i] = std::to_string(std::sin(std::stod(m_sortedExpression[i])) * -1);
        m_sortedExpression.erase(m_sortedExpression.begin() + (i + 1));
        i = 0;
    }
    else if (m_sortedExpression[i + 1] == "cos") {
        m_sortedExpression[i] = std::to_string(std::cos(std::stod(m_sortedExpression[i])));
        m_sortedExpression.erase(m_sortedExpression.begin() + (i + 1));
        i = 0;
    }
    else if (m_sortedExpression[i + 1] == "-cos") {
        m_sortedExpression[i] = std::to_string(std::cos(std::stod(m_sortedExpression[i])) * -1);
        m_sortedExpression.erase(m_sortedExpression.begin() + (i + 1));
        i = 0;
    }
    else if (m_sortedExpression[i + 1] == "tan") {
        m_sortedExpression[i] = std::to_string(std::tan(std::stod(m_sortedExpression[i])));
        m_sortedExpression.erase(m_sortedExpression.begin() + (i + 1));
        i = 0;
    }
    else if (m_sortedExpression[i + 1] == "-tan") {
        m_sortedExpression[i] = std::to_string(std::tan(std::stod(m_sortedExpression[i])) * -1);
        m_sortedExpression.erase(m_sortedExpression.begin() + (i + 1));
        i = 0;
    }
    else if (m_sortedExpression[i + 2] == "+") {
        m_sortedExpression[i] = std::to_string(std::stod(m_sortedExpression[i]) + std::stod(m_sortedExpression[i + 1]));
        m_sortedExpression.erase(m_sortedExpression.begin() + (i + 1));
        m_sortedExpression.erase(m_sortedExpression.begin() + (i + 1));
        i = 0;
    }
    else if (m_sortedExpression[i + 2] == "-") {
        m_sortedExpression[i] = std::to_string(std::stod(m_sortedExpression[i]) - std::stod(m_sortedExpression[i + 1]));
        m_sortedExpression.erase(m_sortedExpression.begin() + (i + 1));
        m_sortedExpression.erase(m_sortedExpression.begin() + (i + 1));
        i = 0;
    }
    else if (m_sortedExpression[i + 2] == "*") {
        m_sortedExpression[i] = std::to_string(std::stod(m_sortedExpression[i]) * std::stod(m_sortedExpression[i + 1]));
        m_sortedExpression.erase(m_sortedExpression.begin() + (i + 1));
        m_sortedExpression.erase(m_sortedExpression.begin() + (i + 1));
        i = 0;
    }
    else if (m_sortedExpression[i + 2] == "/") {
        m_sortedExpression[i] = std::to_string(std::stod(m_sortedExpression[i]) / std::stod(m_sortedExpression[i + 1]));
        m_sortedExpression.erase(m_sortedExpression.begin() + (i + 1));
        m_sortedExpression.erase(m_sortedExpression.begin() + (i + 1));
        i = 0;
    }
    else if (m_sortedExpression[i + 2] == "^") {
        m_sortedExpression[i] = std::to_string(std::pow(std::stod(m_sortedExpression[i]), std::stod(m_sortedExpression[i + 1])));
        m_sortedExpression.erase(m_sortedExpression.begin() + (i + 1));
        m_sortedExpression.erase(m_sortedExpression.begin() + (i + 1));
        i = 0;
    }
    else {
        i++;
    }

    while (m_sortedExpression.size() >= 3) {
        _calculateExpression(i);
    }
}

void Parser::calculateExpression() {
    _calculateExpression(0);
}

void Parser::eraseUnnecessaryDecimals() {
    std::string expression = m_sortedExpression[0];
    while (expression[expression.size() - 1] == '0' && expression.size() > 1 && expression.find('.') != std::string::npos) {
        if (expression[expression.size() - 2] == '.') {
            expression.erase(expression.size() - 2, 1);
        }

        expression.erase(expression.size() - 1, 1);
    }
    m_sortedExpression[0] = expression;
}

int Parser::digitCountFunc(std::string& expression, int& i) {
    int digitCount = 0;
    if (std::isdigit(expression[i + 1]) || expression[i] == 'n' || expression[i + 1] == '.') {
        while (isDigit(expression, i)) {
            i++;
        }
        digitCount++;
        i--;
    }
    else {
        digitCount++;
    }

    return digitCount;
}

bool Parser::checkSyntax(const QString& inputLineContent) {
    std::string expression = inputLineContent.toStdString();
    if(expression.empty()) {
        return false;
    }
    if (isOperatorNoMinus(expression, 0)) {
        return false;
    }

    size_t leftParenCounter = 0, rightParenCounter = 0;
    for (int i = 0; i < expression.length(); i++) {
        if (isOperator(expression, i) && isOperator(expression, i + 1)) {
            return false;
        }
        else if (expression[i] == '(' && expression[i + 1] == ')'
            || expression[i] == '(' && expression[i + 1] == '-' && expression[i + 2] == ')'
            || expression[i] == '(' && isOperatorNoMinus(expression, i + 1)
            || isOperator(expression, i) && expression[i + 1] == ')') {
            return false;
        }
        else if (expression[i] == '(') {
            leftParenCounter++;
        }
        else if (expression[i] == ')') {
            rightParenCounter++;
        }
    }
    if (leftParenCounter != rightParenCounter || isOperatorNoPercent(expression, expression.length() - 1)) {
        return false;
    }
    return true;
}

QString Parser::evaluateExpression(QString& inputLineContent) {
    std::string expression = inputLineContent.toStdString();
    bool isPointless = false;
    shuntingYardAlgorithm(inputLineContent, isPointless);
    if(m_sortedExpression.size() > 0) {
        if(isPointless) {
            return QString::fromStdString(m_sortedExpression[0]);
        }
    }
    sortExpression();
    calculateExpression();
    eraseUnnecessaryDecimals();

    return QString::fromStdString(m_sortedExpression[0]);
}
