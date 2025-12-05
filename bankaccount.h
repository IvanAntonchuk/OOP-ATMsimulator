#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include <QString>
#include <vector>
#include <string>

/**
 * @brief Клас, що представляє банківський рахунок клієнта.
 * * Цей клас зберігає приватні дані користувача (номер картки, PIN, баланс)
 * та історію транзакцій. Надає безпечні методи для фінансових операцій.
 */
class BankAccount
{
private:
    QString cardNumber; ///< Унікальний номер картки (4 цифри)
    QString pinCode;    ///< PIN-код для авторизації
    double balance;     ///< Поточний баланс рахунку

    std::vector<std::string> history; ///< Список виконаних транзакцій

public:
    /**
     * @brief Конструктор рахунку.
     * @param card Номер картки.
     * @param pin PIN-код.
     * @param initialBalance Початковий баланс (за замовчуванням 0).
     */
    BankAccount(QString card, QString pin, double initialBalance);

    /**
     * @brief Отримати номер картки.
     * @return Рядок з номером картки.
     */
    QString getCardNumber() const;

    /**
     * @brief Отримати PIN-код.
     * @return Рядок з PIN-кодом.
     */
    QString getPin() const { return pinCode; }

    /**
     * @brief Перевірити баланс.
     * @return Поточна сума на рахунку.
     */
    double getBalance() const;

    /**
     * @brief Перевірка правильності введеного PIN-коду.
     * @param inputPin PIN, введений користувачем.
     * @return true, якщо коди співпадають, інакше false.
     */
    bool checkPin(QString inputPin) const;

    /**
     * @brief Поповнення рахунку.
     * @param amount Сума для зарахування.
     */
    void deposit(double amount);

    /**
     * @brief Зняття коштів з рахунку.
     * @param amount Сума для зняття.
     * @return true, якщо операція успішна (достатньо коштів), інакше false.
     */
    bool withdraw(double amount);

    /**
     * @brief Додає запис в історію транзакцій.
     * @param record Текстовий опис операції (дата, тип, сума).
     */
    void addHistory(std::string record);

    /**
     * @brief Отримує повну історію транзакцій.
     * @return Вектор рядків з історією.
     */
    std::vector<std::string> getHistory() const;
};

#endif // BANKACCOUNT_H
