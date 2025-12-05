#ifndef ATMCONTROLLER_H
#define ATMCONTROLLER_H

#include "bankaccount.h"
#include <vector>
#include <fstream>
#include <QDateTime>
#include "json.hpp"

using json = nlohmann::json;

class ATMController
{
private:
    std::vector<BankAccount> database;
    BankAccount* currentAccount;
    json atmConfig;
    std::string configPath;
    std::string usersPath;

    void loadConfig();
    void saveConfig();

    void loadUsers();
    void saveUsers();

public:
    ATMController();

    bool insertCard(QString cardNumber);
    bool checkPin(QString pin);
    double getBalance();
    QString tryWithdraw(double amount);
    void deposit(int bills100, int bills200, int bills500);
    QString transfer(QString recipientCard, double amount);
    QString createAccount(QString pin);
    std::vector<std::string> getMyHistory();

    void logout();
};

#endif // ATMCONTROLLER_H
