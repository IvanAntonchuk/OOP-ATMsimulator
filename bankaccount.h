#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include <QString>

class BankAccount
{
private:
    QString cardNumber;
    QString pinCode;
    double balance;

public:
    BankAccount(QString card, QString pin, double initialBalance);

    QString getCardNumber() const;
    double getBalance() const;

    bool checkPin(QString inputPin) const;
    void deposit(double amount);
    bool withdraw(double amount);
};

#endif // BANKACCOUNT_H
