#include <QtTest>
#include <QCoreApplication>
#include "atmcontroller.h"

class ATM_Tests : public QObject
{
    Q_OBJECT

public:
    ATM_Tests();
    ~ATM_Tests();

private slots:

    void initTestCase();
    void cleanupTestCase();

    void testRegistration();
    void testLogin();
    void testDeposit();
    void testWithdraw();
    void testTransfer();
};

ATM_Tests::ATM_Tests() {}
ATM_Tests::~ATM_Tests() {}

void ATM_Tests::initTestCase()
{
    QFile::remove("users.json");
    QFile::remove("atm_config.json");
}

void ATM_Tests::cleanupTestCase()
{
}

void ATM_Tests::testRegistration()
{
    ATMController atm;

    QString newCard = atm.createAccount("5555");

    QVERIFY(newCard.length() == 4);

    bool inserted = atm.insertCard(newCard);
    QVERIFY(inserted == true);

    QVERIFY(atm.checkPin("5555") == true);
    QVERIFY(atm.checkPin("0000") == false);
}

void ATM_Tests::testLogin()
{
    ATMController atm;

    QString card = atm.createAccount("1234");

    QVERIFY(atm.insertCard("9999") == false);

    QVERIFY(atm.insertCard(card) == true);
    QVERIFY(atm.checkPin("1234") == true);
}

void ATM_Tests::testDeposit()
{
    ATMController atm;
    QString card = atm.createAccount("1111");
    atm.insertCard(card);
    atm.checkPin("1111");

    double initialBalance = atm.getBalance();
    QCOMPARE(initialBalance, 0.0);

    atm.deposit(2, 0, 1);

    double newBalance = atm.getBalance();
    QCOMPARE(newBalance, 700.0);
}

void ATM_Tests::testWithdraw()
{
    ATMController atm;
    QString card = atm.createAccount("2222");
    atm.insertCard(card);
    atm.checkPin("2222");

    atm.deposit(0, 0, 2);

    QString res1 = atm.tryWithdraw(5000);
    QVERIFY(res1 != "OK");

    QString res2 = atm.tryWithdraw(500);
    QCOMPARE(res2, QString("OK"));

    QCOMPARE(atm.getBalance(), 500.0);
}

void ATM_Tests::testTransfer()
{
    ATMController atm;

    QString sender = atm.createAccount("1111");
    QString receiver = atm.createAccount("2222");

    atm.insertCard(sender);
    atm.checkPin("1111");
    atm.deposit(10, 0, 0);

    QString result = atm.transfer(receiver, 300.0);
    QCOMPARE(result, QString("OK"));

    QCOMPARE(atm.getBalance(), 700.0);

    atm.logout();
    atm.insertCard(receiver);
    atm.checkPin("2222");
    QCOMPARE(atm.getBalance(), 300.0);
}

QTEST_MAIN(ATM_Tests)

#include "tst_atm_tests.moc"
