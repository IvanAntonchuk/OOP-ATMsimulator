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

    void on_btnWithdrawMenu_clicked();

    void on_btnWithdrawClear_clicked();

    void on_btnWithdrawOk_clicked();

    void on_btnWithdrawBack_clicked();

    void on_btn100_clicked();
    void on_btn200_clicked();
    void on_btn500_clicked();

    void on_btnDepositMenu_clicked();
    void on_btnDep100_clicked();
    void on_btnDep200_clicked();
    void on_btnDep500_clicked();
    void on_btnDepositConfirm_clicked();
    void on_btnDepositBack_clicked();

private:
    Ui::MainWindow *ui;
    ATMController atm;
    int temp100 = 0;
    int temp200 = 0;
    int temp500 = 0;

    void updateDepositLabel();
    void connectKeypad();
};
#endif // MAINWINDOW_H
