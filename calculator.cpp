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


/*
Exceptions:
    0 = no problem everything went alright
    1 = divided by 0, big nono
    2 = square root of a negative number big nono
    3 = unkown Exceptions
    4 = syntax error
*/

void Calculator::on_lineEdit_returnPressed()
{
    int exception = 0;
    QString s = ui->lineEdit->text();
    QString answer;
    std::vector<QString> process;
    long long ans;
    ui->plainTextEdit->setPlainText("");
    ui->textBrowser->setText("");

    //TODO: change the regular expression so that the string is only changed to string + =, do this after initial testing
    if(s.contains(QRegExp("^\\d{1,12}((\\+|-|\\*|\\/)\\d{1,12})*\\=|d{1,12}\\=|-\\d{1,12}((\\+|-|\\*|\\/)\\d{1,12})*\\=")))
    {
        ans = convert(process,s,exception);
        switch(exception)
        {
            case 0:
                for(int i = 0; i < process.size(); i++)
                {
                    answer = answer + process[i] + "\n";
                }
                ui->textBrowser->setText(answer);
                ui->plainTextEdit->setPlainText(std::to_string(ans).c_str());
                break;
            case 1:
                ui->textBrowser->setText("Divided by 0, big nono");
                break;
            case 2:
                ui->textBrowser->setText("square root of a negative number, big nono");
                break;
            case 3:
                ui->textBrowser->setText("Unknown error");
                break;
            case 4:
                ui->textBrowser->setText("Syntax error");
                break;
        }
    }
    else
    {
        exception = 4;
        ui->textBrowser->setText("Syntax error");
    }
}
