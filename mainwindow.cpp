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

    QWidget* current = ui->stackedWidget->currentWidget();
    if (current == ui->page_login) {
        ui->inputCardLogin->setText(ui->inputCardLogin->text() + digit);
    }
    else if (current == ui->page_pin) {
        ui->pinInput->setText(ui->pinInput->text() + digit);
    }
    else if (current == ui->page_withdraw) {
        QString text = ui->lblWithdrawAmount->text();
        if (text == "0") text = "";
        ui->lblWithdrawAmount->setText(text + digit);
    }
    else if (current == ui->page_trans_card) {
        ui->inputTransCard->setText(ui->inputTransCard->text() + digit);
    }
    else if (current == ui->page_trans_amount) {
        QString text = ui->lblTransAmount->text();
        if (text == "0") text = "";
        ui->lblTransAmount->setText(text + digit);
    }
    else if (current == ui->page_register) {
        ui->inputRegPin->setText(ui->inputRegPin->text() + digit);
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

void MainWindow::on_btnLoginOk_clicked()
{
    QString cardNumber = ui->inputCardLogin->text();

    if (atm.insertCard(cardNumber)) {
        ui->pinInput->clear();
        ui->stackedWidget->setCurrentWidget(ui->page_pin);
    } else {
        QMessageBox::warning(this, "Помилка", "Картку не знайдено! Спробуйте іншу.");
        ui->inputCardLogin->clear();
    }
}

void MainWindow::on_btnLoginClear_clicked()
{
    QString text = ui->inputCardLogin->text();
    text.chop(1);
    ui->inputCardLogin->setText(text);
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
    ui->inputCardLogin->clear();
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

void MainWindow::on_btnDepositMenu_clicked()
{
    temp100 = 0;
    temp200 = 0;
    temp500 = 0;
    updateDepositLabel();

    ui->stackedWidget->setCurrentWidget(ui->page_deposit);
}

void MainWindow::on_btnDep100_clicked() { temp100++; updateDepositLabel(); }
void MainWindow::on_btnDep200_clicked() { temp200++; updateDepositLabel(); }
void MainWindow::on_btnDep500_clicked() { temp500++; updateDepositLabel(); }

void MainWindow::updateDepositLabel()
{
    int total = (temp100 * 100) + (temp200 * 200) + (temp500 * 500);
    ui->lblDepositAmount->setText(QString::number(total) + " грн");
}

void MainWindow::on_btnDepositConfirm_clicked()
{
    int total = (temp100 * 100) + (temp200 * 200) + (temp500 * 500);

    if (total == 0) {
        QMessageBox::warning(this, "Увага", "Ви не внесли жодної купюри!");
        return;
    }

    atm.deposit(temp100, temp200, temp500);

    QMessageBox::information(this, "Успіх", "Кошти успішно зараховано!");
    ui->stackedWidget->setCurrentWidget(ui->page_menu);
}

void MainWindow::on_btnDepositBack_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_menu);
}

void MainWindow::on_btnTransferMenu_clicked()
{
    ui->inputTransCard->clear();
    ui->stackedWidget->setCurrentWidget(ui->page_trans_card);
}

void MainWindow::on_btnTransNext_clicked()
{
    QString card = ui->inputTransCard->text();
    if (card.length() < 4) {
        QMessageBox::warning(this, "Помилка", "Введіть коректний номер картки!");
        return;
    }

    ui->lblTransAmount->setText("0");
    ui->stackedWidget->setCurrentWidget(ui->page_trans_amount);
}

void MainWindow::on_btnTransSend_clicked()
{
    QString targetCard = ui->inputTransCard->text();
    double amount = ui->lblTransAmount->text().toDouble();

    if (amount <= 0) {
        QMessageBox::warning(this, "Помилка", "Введіть суму!");
        return;
    }

    QString result = atm.transfer(targetCard, amount);

    if (result == "OK") {
        QMessageBox::information(this, "Успіх", "Кошти успішно надіслано!");
        ui->stackedWidget->setCurrentWidget(ui->page_menu);
    } else {
        QMessageBox::critical(this, "Помилка транзакції", result);
    }
}

void MainWindow::on_btnTransCardClear_clicked()
{
    QString text = ui->inputTransCard->text();
    text.chop(1);
    ui->inputTransCard->setText(text);
}

void MainWindow::on_btnTransAmountClear_clicked()
{
    QString text = ui->lblTransAmount->text();
    text.chop(1);
    if (text.isEmpty()) text = "0";
    ui->lblTransAmount->setText(text);
}

void MainWindow::on_btnTransCancel_clicked() { ui->stackedWidget->setCurrentWidget(ui->page_menu); }
void MainWindow::on_btnTransBack_clicked() { ui->stackedWidget->setCurrentWidget(ui->page_trans_card); }

void MainWindow::on_btnOpenAccount_clicked()
{
    ui->inputRegPin->clear();
    ui->stackedWidget->setCurrentWidget(ui->page_register);
}

void MainWindow::on_btnRegConfirm_clicked()
{
    QString pin = ui->inputRegPin->text();

    if (pin.length() != 4) {
        QMessageBox::warning(this, "Помилка", "PIN-код має складатися з 4 цифр!");
        return;
    }

    QString newCardNumber = atm.createAccount(pin);

    QString message = QString("Вітаємо! Ваш рахунок відкрито.\n\n"
                              "ВАШ НОМЕР КАРТКИ: %1\n"
                              "ВАШ PIN: %2\n\n"
                              "Запам'ятайте ці дані!").arg(newCardNumber, pin);

    QMessageBox::information(this, "Успіх", message);

    ui->stackedWidget->setCurrentWidget(ui->page_login);
    ui->inputCardLogin->setText(newCardNumber);
}

void MainWindow::on_btnRegBack_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_login);
}

void MainWindow::on_btnRegClear_clicked()
{
    QString text = ui->inputRegPin->text();
    text.chop(1);
    ui->inputRegPin->setText(text);
}

void MainWindow::on_btnHistoryMenu_clicked()
{
    ui->listHistory->clear();

    std::vector<std::string> history = atm.getMyHistory();

    for (int i = history.size() - 1; i >= 0; --i) {
        ui->listHistory->addItem(QString::fromStdString(history[i]));
    }

    ui->stackedWidget->setCurrentWidget(ui->page_history);
}

void MainWindow::on_btnHistoryBack_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_menu);
}
