#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionButton_clicked()
{
    QString input = ui->inputField->text();

    if (ui->inputField->echoMode() == QLineEdit::Password) {

        if (atm.checkPin(input)) {
            ui->displayLabel->setText("Успішний вхід! Оберіть дію (Баланс).");
            ui->inputField->clear();
            ui->inputField->setEchoMode(QLineEdit::Normal);
        } else {
            ui->displayLabel->setText("Невірний PIN-код. Спробуйте ще раз.");
            ui->inputField->clear();
        }

    }

    else {

        if (atm.insertCard(input)) {
            ui->displayLabel->setText("Картка прийнята! Введіть PIN-код:");
            ui->inputField->clear();
            ui->inputField->setEchoMode(QLineEdit::Password);
        } else {
            ui->displayLabel->setText("Помилка! Картку не знайдено.");
        }

    }
}


void MainWindow::on_balanceButton_clicked()
{
    double bal = atm.getBalance();
    ui->displayLabel->setText("Ваш баланс: " + QString::number(bal) + " грн");
}

