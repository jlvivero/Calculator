#include "calculator.h"
#include "ui_calculator.h"
#include "parser.h"
#include <QRegExp>

Calculator::Calculator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Calculator)
{
    ui->setupUi(this);
}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::on_lineEdit_returnPressed()
{
    QString s = ui->lineEdit->text();
    std::vector<QString> process;
    long long ans;
    ui->textBrowser->setText("");
    if(s.contains(QRegExp("^\d{1,12}((\+|-|\*|\/)\d{1,12})*"))) //TODO put regular expresion here
    {
        //TODO create class that will change the infix format to the polish notation
        ans = convert();
        ui->textBrowser->setText();
    }
    else
    {
        ui->textBrowser->setText("Syntax error");
    }
}
