#include "calculator.h"
#include "ui_calculator.h"

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
    if(s.contains("")) //TODO put regular expresion here
    {
        //TODO create class that will change the infix format to the polish notation
        //TODO create class that will evaluate the polish notation it will return an answer and a list of Qstrings
        ui->textBrowser->setText();
    }
    else
    {
        ui->textBrowser->setText("Syntax error");
    }
}
