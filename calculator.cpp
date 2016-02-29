#include "calculator.h"
#include "ui_calculator.h"
#include "parser.h"
#include <QRegExp>
#include <iostream>
#include <string>

Calculator::Calculator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Calculator)
{
    ui->setupUi(this);
    //idk if anything i put after this is supposed to be here
    QObject::connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(onReturnPressed()));
    ui->plainTextEdit->setReadOnly(true);
}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::on_lineEdit_returnPressed()
{
    QString s = ui->lineEdit->text();
    QString answer;
    std::vector<QString> process;
    long long ans;
    ui->plainTextEdit->setPlainText("");
    ui->textBrowser->setText("");

    //TODO: change the regular expression so that the string is only changed to string + =, do this after initial testing
    if(s.contains(QRegExp("^\\d{1,12}((\\+|-|\\*|\\/)\\d{1,12})*\\=|d{1,12}\\=|-\\d{1,12}((\\+|-|\\*|\\/)\\d{1,12})*\\=")))
    {
        ans = convert(process,s);
        for(int i = 0; i < process.size(); i++)
        {
            answer = answer + process[i] + "\n";
        }
        ui->textBrowser->setText(answer);
        ui->plainTextEdit->setPlainText(std::to_string(ans).c_str());
    }
    else
    {
        ui->textBrowser->setText("Syntax error");
    }
}
