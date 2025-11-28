#ifndef ATMCONTROLLER_H
#define ATMCONTROLLER_H

#include "bankaccount.h"
#include <vector>

class ATMController
{
private:
    std::vector<BankAccount> database;
    BankAccount* currentAccount;

public:
    ATMController();

    bool insertCard(QString cardNumber);
    bool checkPin(QString pin);
    double getBalance();
    bool withdrawAmount(double amount);
    void logout();
};

#endif // ATMCONTROLLER_H
