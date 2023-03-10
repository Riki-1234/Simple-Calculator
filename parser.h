#ifndef PARSER_H
#define PARSER_H

#include <QMainWindow>
#include <QWidget>
#include <stack>

class Parser
{
public:
    QString evaluateExpression(QString& inputLineContent);
    bool checkSyntax(const QString& inputLineContent);
private:
    bool isSquareCubicRoot(const std::string& expression, int i);
    bool isSinCosTan(const std::string& expression, int i);
    bool isTopOperator();
    bool isTopHighPrecedenceOperator();
    bool isDigit(const std::string& expression, int i);
    bool isOperator(const std::string& expression, int i);
    bool isOperatorNoPercent(const std::string& expression, int i);
    bool isAdditionMultiplicationDivision(const std::string& expression, int i);
    bool isOperatorNoMinus(const std::string& expression, int i);
    bool isMissingMultiplication(const std::string& expression, int i);
    bool isPointlessExpression(const std::string& expression);

    void lexMultiplicationAndDivision(const std::string& expression, int i);
    void lexNumbers(std::string& expression, int& i);
    void lexAdditionAndSubtraction(const std::string& expression, int i);
    void lexSinCosTan(std::string& expression, int& i);
    void lexSqrtCbrt(std::string& expression, int& i);

    void replacePercentWithDivision(std::string& expression);
    void addMissingMultiplication(std::string& expression);
    void solveMultipleMinuses(std::string& expression);
    void eraseWhiteSpaces(std::string& expression);
    void eraseBrackets(std::string& expression);

    void shuntingYardAlgorithm(QString& inputLineContent, bool& isPointless);
    void sortExpression();
    void _calculateExpression(int i);
    void calculateExpression();

    void eraseUnnecessaryDecimals();
    int digitCountFunc(std::string& expression, int& i);
    std::stack<std::string> m_operands, m_operators;
    std::vector<std::string> m_sortedExpression;
};

#endif
