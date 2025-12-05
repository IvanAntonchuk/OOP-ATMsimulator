#include "atmcontroller.h"
#include <QDir>
#include <QDebug>
#include <QCoreApplication>
#include <cstdlib>
#include <ctime>

ATMController::ATMController()
{
    currentAccount = nullptr;

    QString appDir = QCoreApplication::applicationDirPath();

    configPath = appDir.toStdString() + "/atm_config.json";
    usersPath = appDir.toStdString() + "/users.json";

    loadConfig();

    loadUsers();
    std::srand(std::time(nullptr));
}

void ATMController::loadConfig()
{
    std::ifstream file(configPath);
    if (file.is_open()) {
        try {
            file >> atmConfig;
        } catch (...) {
            atmConfig["atm_cash"] = { {"100", 0}, {"200", 0}, {"500", 0} };
        }
        file.close();
    } else {
        atmConfig["atm_cash"] = { {"100", 100}, {"200", 100}, {"500", 100} };
        saveConfig();
    }
}

void ATMController::saveConfig()
{
    std::ofstream file(configPath);
    if (file.is_open()) {
        file << atmConfig.dump(4);
        file.close();
    }
}

void ATMController::loadUsers()
{
    database.clear();
    std::ifstream file(usersPath);

    if (file.is_open()) {
        json jUsers;
        try {
            file >> jUsers;
            for (auto& element : jUsers) {
                QString card = QString::fromStdString(element["card"]);
                QString pin = QString::fromStdString(element["pin"]);
                double bal = element["balance"];
                database.push_back(BankAccount(card, pin, bal));
            }
        } catch (...) {
            qDebug() << "Помилка читання users.json";
        }
        file.close();
    } else {
        database.push_back(BankAccount("1111", "1111", 5000.0));
        database.push_back(BankAccount("2222", "0000", 10000.0));
        database.push_back(BankAccount("1234", "1234", 250.0));
        saveUsers();
    }

    qDebug() << "=== ЗАВАНТАЖЕНІ КАРТКИ ===";
    for (const auto& acc : database) {
        qDebug() << "Card:" << acc.getCardNumber() << "PIN:" << acc.getPin();
    }
    qDebug() << "==========================";
}

void ATMController::saveUsers()
{
    json jUsers = json::array();

    for (const auto& acc : database) {
        jUsers.push_back({
            {"card", acc.getCardNumber().toStdString()},
            {"pin", acc.getPin().toStdString()},
            {"balance", acc.getBalance()}
        });
    }

    std::ofstream file(usersPath);
    if (file.is_open()) {
        file << jUsers.dump(4);
        file.close();
    }
}

bool ATMController::insertCard(QString cardNumber)
{
    for (size_t i = 0; i < database.size(); ++i) {
        if (database[i].getCardNumber() == cardNumber) {
            currentAccount = &database[i];
            return true;
        }
    }
    return false;
}

bool ATMController::checkPin(QString pin)
{
    if (currentAccount != nullptr) {
        return currentAccount->checkPin(pin);
    }
    return false;
}

double ATMController::getBalance()
{
    if (currentAccount != nullptr) {
        return currentAccount->getBalance();
    }
    return 0.0;
}

QString ATMController::tryWithdraw(double amount)
{
    if (!currentAccount) return "Помилка авторизації";

    if (currentAccount->getBalance() < amount) {
        return "Недостатньо коштів на картці!";
    }

    int needed = (int)amount;

    int bills500 = atmConfig["atm_cash"]["500"];
    int bills200 = atmConfig["atm_cash"]["200"];
    int bills100 = atmConfig["atm_cash"]["100"];

    int give500 = 0;
    int give200 = 0;
    int give100 = 0;

    while (needed >= 500 && bills500 > 0) {
        needed -= 500;
        bills500--;
        give500++;
    }

    while (needed >= 200 && bills200 > 0) {
        needed -= 200;
        bills200--;
        give200++;
    }

    while (needed >= 100 && bills100 > 0) {
        needed -= 100;
        bills100--;
        give100++;
    }

    if (needed == 0) {
        currentAccount->withdraw(amount);

        atmConfig["atm_cash"]["500"] = bills500;
        atmConfig["atm_cash"]["200"] = bills200;
        atmConfig["atm_cash"]["100"] = bills100;

        saveConfig();
        saveUsers();

        return "OK";
    } else {
        return "У банкоматі немає необхідних купюр\nдля видачі цієї суми (кратність 100, 200, 500).";
    }
}

void ATMController::deposit(int bills100, int bills200, int bills500)
{
    if (!currentAccount) return;

    double totalAmount = (bills100 * 100) + (bills200 * 200) + (bills500 * 500);

    currentAccount->deposit(totalAmount);

    atmConfig["atm_cash"]["100"] = atmConfig["atm_cash"]["100"].get<int>() + bills100;
    atmConfig["atm_cash"]["200"] = atmConfig["atm_cash"]["200"].get<int>() + bills200;
    atmConfig["atm_cash"]["500"] = atmConfig["atm_cash"]["500"].get<int>() + bills500;

    saveConfig();
    saveUsers();
}

void ATMController::logout()
{
    currentAccount = nullptr;
}


QString ATMController::transfer(QString recipientCard, double amount)
{
    if (!currentAccount) return "Помилка авторизації";

    if (currentAccount->getCardNumber() == recipientCard) {
        return "Неможливо переказати кошти на ту ж саму картку!";
    }

    BankAccount* recipient = nullptr;
    for (size_t i = 0; i < database.size(); ++i) {
        if (database[i].getCardNumber() == recipientCard) {
            recipient = &database[i];
            break;
        }
    }

    if (recipient == nullptr) {
        return "Картку отримувача не знайдено!";
    }

    if (currentAccount->getBalance() < amount) {
        return "Недостатньо коштів для переказу!";
    }

    currentAccount->withdraw(amount);
    recipient->deposit(amount);

    saveUsers();

    return "OK";
}

QString ATMController::createAccount(QString pin)
{
    QString newCard;
    bool exists = true;

    while (exists) {
        int randomNum = 1000 + (std::rand() % 9000);
        newCard = QString::number(randomNum);

        exists = false;
        for (const auto& acc : database) {
            if (acc.getCardNumber() == newCard) {
                exists = true;
                break;
            }
        }
    }

    database.push_back(BankAccount(newCard, pin, 0.0));

    saveUsers();

    return newCard;
}
