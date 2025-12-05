#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "atmcontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnLogin_clicked();

    void onDigitPressed();
    void on_btnClear_clicked();
    void on_btnEnterPin_clicked();
    void on_btnCancel_clicked();

    void on_btnBalance_clicked();
    void on_btnExit_clicked();
    void on_btnBack_clicked();

private:
    Ui::MainWindow *ui;
    ATMController atm;

    void connectKeypad();
};
#endif // MAINWINDOW_H
