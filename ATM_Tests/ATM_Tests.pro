QT += core testlib
QT -= gui

CONFIG += c++17
TARGET = tst_atm_tests
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

INCLUDEPATH += ../

SOURCES += \
    tst_atm_tests.cpp \
    ../atmcontroller.cpp \
    ../bankaccount.cpp

HEADERS += \
    tst_atm_tests.h \
    ../atmcontroller.h \
    ../bankaccount.h
