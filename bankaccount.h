#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include <QString>
#include <vector>
#include <string>

class BankAccount
{
private:
    QString cardNumber;
    QString pinCode;
    double balance;

    std::vector<std::string> history;

public:
    BankAccount(QString card, QString pin, double initialBalance);

    QString getCardNumber() const;
    QString getPin() const { return pinCode; }

    double getBalance() const;

    bool checkPin(QString inputPin) const;
    void deposit(double amount);
    bool withdraw(double amount);

    void addHistory(std::string record);
    std::vector<std::string> getHistory() const;
};

#endif // BANKACCOUNT_H
