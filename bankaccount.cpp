#include "bankaccount.h"

BankAccount::BankAccount(QString card, QString pin, double initialBalance)
{
    cardNumber = card;
    pinCode = pin;
    balance = initialBalance;
}

QString BankAccount::getCardNumber() const {
    return cardNumber;
}

double BankAccount::getBalance() const {
    return balance;
}

bool BankAccount::checkPin(QString inputPin) const {
    return pinCode == inputPin;
}

void BankAccount::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
    }
}

bool BankAccount::withdraw(double amount) {
    if (amount > 0 && balance >= amount) {
        balance -= amount;
        return true;
    }
    return false;
}

void BankAccount::addHistory(std::string record)
{
    history.push_back(record);
}

std::vector<std::string> BankAccount::getHistory() const
{
    return history;
}
