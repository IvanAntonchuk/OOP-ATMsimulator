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

    void on_btnTransferMenu_clicked();
    void on_btnTransNext_clicked();
    void on_btnTransSend_clicked();

    void on_btnTransCancel_clicked();
    void on_btnTransBack_clicked();

    void on_btnTransCardClear_clicked();
    void on_btnTransAmountClear_clicked();

    void on_btnLoginOk_clicked();
    void on_btnLoginClear_clicked();

    void on_btnOpenAccount_clicked();
    void on_btnRegConfirm_clicked();
    void on_btnRegBack_clicked();
    void on_btnRegClear_clicked();

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
