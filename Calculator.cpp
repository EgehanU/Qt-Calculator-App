#include "calculator.h"

Calculator::Calculator(QWidget* parent)
    : QMainWindow(parent)
{
    createLayout();
    connectSlots();
}

Calculator::~Calculator()
{
}

void Calculator::createLayout()
{
    displayLineEdit = new QLineEdit(this);
    displayLineEdit->setReadOnly(true);

    QGridLayout* mainLayout = new QGridLayout(this);
    mainLayout->addWidget(displayLineEdit, 0, 0, 1, 4);

    for (int i = 0; i < 10; ++i)
    {
        digitButtons[i] = new QPushButton(QString::number(i), this);
        mainLayout->addWidget(digitButtons[i], (i + 3) / 3, (i + 2) % 3);
    }

    operatorButtons[0] = new QPushButton("+", this);
    operatorButtons[1] = new QPushButton("-", this);
    operatorButtons[2] = new QPushButton("*", this);
    operatorButtons[3] = new QPushButton("/", this);

    for (int i = 0; i < 4; ++i) 
    {
        mainLayout->addWidget(operatorButtons[i], i + 1, 3);
    }

    equalButton = new QPushButton("=", this);
    clearButton = new QPushButton("C", this);

    mainLayout->addWidget(equalButton, 4, 2);
    mainLayout->addWidget(clearButton, 4, 1);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

void Calculator::connectSlots()
{
    for (int i = 0; i < 10; ++i)
    {
        connect(digitButtons[i], &QPushButton::clicked, this, &Calculator::onDigitClicked);
    }

    connect(operatorButtons[0], &QPushButton::clicked, this, &Calculator::onOperatorClicked);
    connect(operatorButtons[1], &QPushButton::clicked, this, &Calculator::onOperatorClicked);
    connect(operatorButtons[2], &QPushButton::clicked, this, &Calculator::onOperatorClicked);
    connect(operatorButtons[3], &QPushButton::clicked, this, &Calculator::onOperatorClicked);

    connect(equalButton, &QPushButton::clicked, this, &Calculator::onEqualClicked);
    connect(clearButton, &QPushButton::clicked, this, &Calculator::onClearClicked);
}

void Calculator::onDigitClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button)
    {
        QString digit = button->text();
        currentNumber += digit;
        displayLineEdit->setText(displayLineEdit->text() + digit);
    }
}

void Calculator::onOperatorClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button)
    {
        QString op = button->text();
        if (currentNumber.isEmpty()) {
            // If no number is entered before an operator, replace the existing operator
            int lastIndex = displayLineEdit->text().length() - 1;
            displayLineEdit->setText(displayLineEdit->text().replace(lastIndex, 1, op));
        }
        else {
            currentNumber.clear();
            displayLineEdit->setText(displayLineEdit->text() + op);
        }
    }
}

void Calculator::onEqualClicked()
{
    QString expression = displayLineEdit->text();
    if (expression.contains("++") || expression.contains("--") || expression.contains("**") ||
        expression.contains("//")) {
        displayLineEdit->setText("Error: Invalid expression");
        return;
    }

    QRegularExpression re("(\\d+)([+*/-])");
    QStack<QString> operands;
    QStack<QString> operators;

    QRegularExpressionMatchIterator i = re.globalMatch(expression);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        QString operand = match.captured(1);
        QString op = match.captured(2);

        operands.push(operand);
        operators.push(op);
    }

    operands.push(expression.mid(expression.lastIndexOf(QRegularExpression("[+*/-]")) + 1));

    while (!operators.isEmpty()) {
        QString op = operators.pop();
        QString operand2 = operands.pop();
        QString operand1 = operands.pop();

        double result = 0.0;

        if (op == "+") {
            result = operand1.toDouble() + operand2.toDouble();
        }
        else if (op == "-") {
            result = operand1.toDouble() - operand2.toDouble();
        }
        else if (op == "*") {
            result = operand1.toDouble() * operand2.toDouble();
        }
        else if (op == "/") {
            if (operand2.toDouble() != 0.0) {
                result = operand1.toDouble() / operand2.toDouble();
            }
            else {
                displayLineEdit->setText("Error: Division by zero");
                return;
            }
        }

        operands.push(QString::number(result));
    }

    currentNumber = operands.pop();
    displayLineEdit->setText(currentNumber);
}

void Calculator::onClearClicked()
{
    currentNumber.clear();
    displayLineEdit->clear();
}
