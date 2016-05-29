#include "calculator.h"
#include "ui_calculator.h"
#include "parser.h"
#include "basec.h"
#include "decimal.h"
#include "validateTokens.h"


#include <QRegExp>
#include <iostream>
#include <string>
dec::decimal<8> ad(0);
rNumber a(ad,0,false);
rNumber b(ad,0,false);
rNumber c(ad,0,false);

Calculator::Calculator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Calculator)
{
    ui->setupUi(this);
    //idk if anything i put after this is supposed to be here
    //this is specifically for my OS ubuntu with numix, i was having problems adapting the gtk delete this next line if you
    // want to use the default theming on your OS
    this->setStyleSheet("background-color: #2d2d2d; border: 1px solid rgba(51, 51, 51, 1); color: #dcdcdc; gridline-color: #2d2d2d;");
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
    3 = integer overflow
    4 = syntax error
*/

void Calculator::on_lineEdit_returnPressed()
{
    int exception = 0;
    rNumber ans;
    std::string b2;
    std::string b16;
    QString s = ui->lineEdit->text();
    QString answer;
    std::vector<QString> process;
    ui->plainTextEdit->setPlainText("");
    ui->textBrowser->setText("");

    /**************************************************************************
    Structure: The first thing the text needs to be passed through is something that divides it into the different kinds of ecuations possible
    for example if, root, normal stuff
    The formats are the following:
    if [condition] AND [condition] THEN [ecuation]
    Root(something) <- this can be en a normal ecuation so it doesn't count as a separate thing
    FORMAT TO REAL(precicion)
    FORMAT TO CN
    FORMAT TO STANDARD
    */
    ans.copy(prepareForParse(process,s,exception,a,b,c));
    //ans.copy(convert(process,s,exception,a,b,c));
    switch(exception)
    {
      case 0:
      for(size_t i = 0; i < process.size(); i++)
      {
        answer = answer + process[i] + "\n";
      }
      ui->textBrowser->setText(answer);
      ui->plainTextEdit->setPlainText(ans.printNumber().c_str());
      break;
      case 1:
      ui->textBrowser->setText("Divided by 0, big nono");
      break;
      case 2:
      ui->textBrowser->setText("square root of a negative number, big nono");
      break;
      case 3:
      ui->textBrowser->setText("Integer overflow");
      break;
      case 4:
      ui->textBrowser->setText("Syntax error");
      break;
      case 5:
      ui->textBrowser->setText("unmatched parenthesis");
      break;
      case 6:
      ui->textBrowser->setText("variable not defined");
      break;
    }
}
