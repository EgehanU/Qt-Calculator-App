#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QtWidgets/QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QStack>
#include <QRegularExpression>
#include <QDebug>

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    Calculator(QWidget* parent = nullptr);
    ~Calculator();

private slots:
    void onDigitClicked();
    void onOperatorClicked();
    void onEqualClicked();
    void onClearClicked();

private:
    QLineEdit* displayLineEdit;
    QPushButton* digitButtons[10];
    QPushButton* operatorButtons[4]; 
    QPushButton* equalButton;
    QPushButton* clearButton;

    QString currentNumber;
    QString pendingOperator;
    double result;

    void createLayout();
    void connectSlots();
};

#endif // CALCULATOR_H
