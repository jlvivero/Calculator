#-------------------------------------------------
#
# Project created by QtCreator 2016-02-28T11:41:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Calculator
TEMPLATE = app


SOURCES += main.cpp\
        calculator.cpp parser.cpp

HEADERS  += calculator.h parser.h

FORMS    += calculator.ui

CONFIG += c++11
