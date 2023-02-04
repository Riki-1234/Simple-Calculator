#ifndef PARSER_H
#define PARSER_H

#include <QMainWindow>
#include <QWidget>
#include <stack>

class Parser
{
public:
    void shuntingYardAlgorithm(QString& inputLineContent);
    void sortExpression();
    void _calculateExpression(int i);
    void calculateExpression();
    QString evaluateExpression(QString& inputLineContent);
private:
    std::stack<std::string> m_operands, m_operators;
    std::vector<std::string> m_sortedExpression;
};

#endif
