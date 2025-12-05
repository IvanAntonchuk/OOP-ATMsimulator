#ifndef ATMCONTROLLER_H
#define ATMCONTROLLER_H

#include "bankaccount.h"
#include <vector>
#include <fstream>
#include "json.hpp"
#include <QCoreApplication>

using json = nlohmann::json;

/**
 * @brief Головний клас-контролер банкомата.
 * * Цей клас відповідає за бізнес-логіку: керування базою користувачів,
 * обробку транзакцій, роботу з файловою системою та алгоритми видачі готівки.
 * Реалізує патерн Controller у архітектурі MVC.
 */
class ATMController
{
private:
    std::vector<BankAccount> database;  ///< Список всіх завантажених карток
    BankAccount* currentAccount;        ///< Вказівник на поточного авторизованого користувача

    json atmConfig;         ///< JSON-об'єкт для зберігання конфігурації (купюр)
    std::string configPath; ///< Шлях до файлу atm_config.json
    std::string usersPath;  ///< Шлях до файлу users.json

    /**
     * @brief Завантажує конфігурацію купюр з файлу.
     */
    void loadConfig();

    /**
     * @brief Зберігає поточний стан купюр у файл.
     */
    void saveConfig();

    /**
     * @brief Завантажує базу клієнтів та їх баланси.
     */
    void loadUsers();

    /**
     * @brief Зберігає оновлені дані клієнтів (баланс, історію) у файл.
     */
    void saveUsers();

public:
    /**
     * @brief Конструктор. Ініціалізує шляхи до файлів та завантажує дані.
     */
    ATMController();

    /**
     * @brief Перевіряє наявність картки в базі.
     * @param cardNumber Номер картки для перевірки.
     * @return true, якщо картка знайдена, інакше false.
     */
    bool insertCard(QString cardNumber);

    /**
     * @brief Перевіряє PIN-код для поточної картки.
     * @param pin Введений користувачем PIN.
     * @return true, якщо PIN вірний.
     */
    bool checkPin(QString pin);

    /**
     * @brief Повертає баланс поточного користувача.
     * @return Сума коштів (double).
     */
    double getBalance();

    /**
     * @brief Спроба зняття готівки.
     * * Використовує жадібний алгоритм для перевірки наявності купюр
     * номіналом 500, 200 та 100 грн.
     * * @param amount Сума для зняття.
     * @return Рядок "OK" у разі успіху або текст помилки.
     */
    QString tryWithdraw(double amount);

    /**
     * @brief Поповнення рахунку.
     * @param bills100 Кількість купюр по 100 грн.
     * @param bills200 Кількість купюр по 200 грн.
     * @param bills500 Кількість купюр по 500 грн.
     */
    void deposit(int bills100, int bills200, int bills500);

    /**
     * @brief Переказ коштів іншому клієнту.
     * @param recipientCard Номер картки отримувача.
     * @param amount Сума переказу.
     * @return "OK" або текст помилки (наприклад, "Недостатньо коштів").
     */
    QString transfer(QString recipientCard, double amount);

    /**
     * @brief Створює новий банківський рахунок.
     * @param pin Бажаний PIN-код.
     * @return Згенерований унікальний номер картки (4 цифри).
     */
    QString createAccount(QString pin);

    /**
     * @brief Отримує історію транзакцій поточного користувача.
     * @return Вектор рядків з описом операцій.
     */
    std::vector<std::string> getMyHistory();

    /**
     * @brief Завершення сеансу (вийняти картку).
     */
    void logout();
};

#endif // ATMCONTROLLER_H
