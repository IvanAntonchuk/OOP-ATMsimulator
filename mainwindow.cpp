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
        if (btn->objectName().startsWith("btn") && btn->objectName().length() == 4) {
            bool isDigit = false;
            btn->objectName().mid(3).toInt(&isDigit);

            if (isDigit) {
                connect(btn, &QPushButton::clicked, this, &MainWindow::onDigitPressed);
            }
        }
    }
}

void MainWindow::onDigitPressed()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString digit = button->text();
        ui->pinInput->setText(ui->pinInput->text() + digit);
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
