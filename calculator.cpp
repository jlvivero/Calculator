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
    Qstring answer;
    std::vector<QString> process;
    long long ans;
    ui->textBrowser->setText("");

    //TODO: change the regular expression so that the string is only changed to string + =, do this after initial testing
    if(s.contains(QRegExp("^\d{1,12}((\+|-|\*|\/)\d{1,12})*\=|d{1,12}\=|-\d{1,12}((\+|-|\*|\/)\d{1,12})*\=")))
    {
        ans = convert(process,s);
        for(int i = 0; i < process.size(); i++)
        {
            answer = answer + process[i] + "\n"
        }
        answer = answer + ans;
        ui->textBrowser->setText(answer);
    }
    else
    {
        ui->textBrowser->setText("Syntax error");
    }
}
