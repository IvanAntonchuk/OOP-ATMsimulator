#include "atmcontroller.h"

ATMController::ATMController()
{
    currentAccount = nullptr;

    database.push_back(BankAccount("1111", "1111", 5000.0));

    database.push_back(BankAccount("2222", "0000", 10000.0));

    database.push_back(BankAccount("1234", "1234", 250.0));
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

bool ATMController::withdrawAmount(double amount)
{
    if (currentAccount != nullptr) {
        return currentAccount->withdraw(amount);
    }
    return false;
}

void ATMController::logout()
{
    currentAccount = nullptr;
}
