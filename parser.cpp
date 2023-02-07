#include "parser.h"
#include <string>
#include <vector>
#include <utility>
#include <cmath>

void Parser::shuntingYardAlgorithm(QString& inputLineContent) {   
    std::string expression = inputLineContent.toStdString();

    while(expression.find("³√") != std::string::npos) {
       expression.replace(expression.find("³√"), 3, "o");
    }
    while(expression.find("√") != std::string::npos) {
       expression.replace(expression.find("√"), 1, 1, 'r');
    }
    while(expression.find("sin") != std::string::npos) {
       expression.replace(expression.find("sin"), 3, "s");
    }
    while(expression.find("cos") != std::string::npos) {
       expression.replace(expression.find("cos"), 3, "c");
    }
    while(expression.find("tan") != std::string::npos) {
       expression.replace(expression.find("tan"), 3, "t");
    }
    while(expression.find("%") != std::string::npos) {
        int percentIndex;
        for(int i = 0; i < expression.size(); i++) {
            if(expression[i] == '%') {
                percentIndex = i;
            }
        }

        expression.insert(expression.begin() + (percentIndex + 1), '1');
        expression.insert(expression.begin() + (percentIndex + 2), '0');
        expression.insert(expression.begin() + (percentIndex + 3), '0');

        expression.replace(expression.find("%"), 1, 1, '/');
    }
    if(expression[0] == '-') {
        expression.replace(0, 1, 1, 'n');
    }

    for (int i = 0; i < expression.length(); i++) {
        if (expression[i] == '+' || expression[i] == '-') {
            if (m_operators.size() == 0) {
                m_operators.push(std::string(1, expression[i]));
            }
            else if (m_operators.top() != "*" || m_operators.top() != "/") {
                m_operators.push(std::string(1, expression[i]));
            }
            else {
                while (m_operators.size() != 0) {
                    m_operands.push(m_operators.top());
                    m_operators.pop();
                }
                m_operators.push(std::string(1, expression[i]));
            }
        }
        else if (expression[i] == '*' || expression[i] == '/') {
            if(!m_operators.empty()) {
                if (m_operators.top() == "^" || m_operators.top() == "r" || m_operators.top() == "o" || m_operators.top() == "s" || m_operators.top() == "c" || m_operators.top() == "t") {
                    while (m_operators.size() != 0) {
                        m_operands.push(m_operators.top());
                        m_operators.pop();
                    }
                    m_operators.push(std::string(1, expression[i]));
                }
                else if (m_operators.top() == "*" || m_operators.top() == "/") {
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
        else if (std::isdigit(expression[i]) || expression[i] == 'n' || expression[i] == '.') {
            std::string multiDigitNum{};
            if (std::isdigit(expression[i + 1]) || expression[i] == 'n' || expression[i + 1] == '.') {
                while (std::isdigit(expression[i]) || expression[i] == 'n' || expression[i] == '.') {
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
        else if (expression[i] == '^' || expression[i] == 'r' || expression[i] == 'o' || expression[i] == 's' || expression[i] == 'c' || expression[i] == 't') {
            m_operators.push(std::string(1, expression[i]));
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
}

void Parser::_calculateExpression(int i) {
    if (m_sortedExpression[i + 1] == "r") {
        m_sortedExpression[i] = std::to_string(std::sqrt(std::stod(m_sortedExpression[i])));
        m_sortedExpression.erase(m_sortedExpression.begin() + (i + 1));
        i = 0;
    }
    else if (m_sortedExpression[i + 1] == "o") {
        m_sortedExpression[i] = std::to_string(std::cbrt(std::stod(m_sortedExpression[i])));
        m_sortedExpression.erase(m_sortedExpression.begin() + (i + 1));
        i = 0;
    }
    else if (m_sortedExpression[i + 1] == "s") {
        m_sortedExpression[i] = std::to_string(std::sin(std::stod(m_sortedExpression[i])));
        m_sortedExpression.erase(m_sortedExpression.begin() + (i + 1));
        i = 0;
    }
    else if (m_sortedExpression[i + 1] == "c") {
        m_sortedExpression[i] = std::to_string(std::cos(std::stod(m_sortedExpression[i])));
        m_sortedExpression.erase(m_sortedExpression.begin() + (i + 1));
        i = 0;
    }
    else if (m_sortedExpression[i + 1] == "t") {
        m_sortedExpression[i] = std::to_string(std::tan(std::stod(m_sortedExpression[i])));
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

QString Parser::evaluateExpression(QString& inputLineContent) {
    shuntingYardAlgorithm(inputLineContent);
    sortExpression();
    calculateExpression();

    std::string expression = m_sortedExpression[0];
    while(expression[expression.size()- 1] == '0' && expression.size() > 1 && expression.find('.') != std::string::npos) {
        if(expression[expression.size() - 2] == '.') {
            expression.erase(expression.size() - 2, 1);
        }

        expression.erase(expression.size() - 1, 1);
    }

    QString result = QString::fromStdString(expression);

    return result;
}
