#ifndef ATMCONTROLLER_H
#define ATMCONTROLLER_H

#include "bankaccount.h"
#include <vector>
#include <fstream>
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

    void logout();
};

#endif // ATMCONTROLLER_H
