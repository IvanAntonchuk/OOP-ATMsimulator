#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentWidget(ui->page_login);

    connectKeypad();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectKeypad()
{
    QList<QPushButton*> buttons = this->findChildren<QPushButton*>();

    for (QPushButton* btn : buttons) {
        bool isDigit = false;
        btn->text().toInt(&isDigit);

        if (isDigit) {
            connect(btn, &QPushButton::clicked, this, &MainWindow::onDigitPressed);
        }
    }
}

void MainWindow::onDigitPressed()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    QString digit = button->text();

    if (ui->stackedWidget->currentWidget() == ui->page_pin) {
        ui->pinInput->setText(ui->pinInput->text() + digit);
    }
    else if (ui->stackedWidget->currentWidget() == ui->page_withdraw) {
        QString currentText = ui->lblWithdrawAmount->text();
        if (currentText == "0") currentText = "";
        ui->lblWithdrawAmount->setText(currentText + digit);
    }
}

void MainWindow::on_btnClear_clicked()
{
    QString text = ui->pinInput->text();
    text.chop(1);
    ui->pinInput->setText(text);
}

void MainWindow::on_btnCancel_clicked()
{
    ui->pinInput->clear();
    ui->stackedWidget->setCurrentWidget(ui->page_login);
}

void MainWindow::on_btnLogin_clicked()
{
    if (atm.insertCard("1111")) {
        ui->pinInput->clear();
        ui->stackedWidget->setCurrentWidget(ui->page_pin);
    }
}

void MainWindow::on_btnEnterPin_clicked()
{
    QString pin = ui->pinInput->text();

    if (atm.checkPin(pin)) {
        ui->stackedWidget->setCurrentWidget(ui->page_menu);
    } else {
        QMessageBox::warning(this, "Помилка", "Невірний PIN-код!");
        ui->pinInput->clear();
    }
}

void MainWindow::on_btnBalance_clicked()
{
    double amount = atm.getBalance();
    ui->labelBalanceInfo->setText(QString("Ваш баланс:\n%1 грн").arg(amount));
    ui->stackedWidget->setCurrentWidget(ui->page_balance);
}

void MainWindow::on_btnBack_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_menu);
}

void MainWindow::on_btnExit_clicked()
{
    atm.logout();
    ui->stackedWidget->setCurrentWidget(ui->page_login);
}

void MainWindow::on_btnWithdrawMenu_clicked()
{
    ui->lblWithdrawAmount->setText("0");
    ui->stackedWidget->setCurrentWidget(ui->page_withdraw);
}

void MainWindow::on_btnWithdrawClear_clicked()
{
    QString text = ui->lblWithdrawAmount->text();
    text.chop(1);

    if (text.isEmpty()) text = "0";

    ui->lblWithdrawAmount->setText(text);
}

void MainWindow::on_btn100_clicked() { ui->lblWithdrawAmount->setText("100"); }
void MainWindow::on_btn200_clicked() { ui->lblWithdrawAmount->setText("200"); }
void MainWindow::on_btn500_clicked() { ui->lblWithdrawAmount->setText("500"); }

void MainWindow::on_btnWithdrawOk_clicked()
{
    double amount = ui->lblWithdrawAmount->text().toDouble();
    if (amount <= 0) {
        QMessageBox::warning(this, "Помилка", "Оберіть суму!");
        return;
    }

    QString result = atm.tryWithdraw(amount);

    if (result == "OK") {
        QMessageBox::information(this, "Успіх", "Операція успішна! Заберіть гроші.");
        ui->stackedWidget->setCurrentWidget(ui->page_menu);
        ui->lblWithdrawAmount->setText("0");
    } else {
        QMessageBox::critical(this, "Відмова", result);
    }
}



void MainWindow::on_btnWithdrawBack_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_menu);
    ui->lblWithdrawAmount->setText("0");
}

