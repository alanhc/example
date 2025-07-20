#include <iostream>

class BankAccount
{
private:
    double balance;

public:
    BankAccount(double initial) : balance(initial) {}

    void deposit(double amount)
    {
        if (amount > 0)
            balance += amount;
    }

    void withdraw(double amount)
    {
        if (amount > 0 && amount <= balance)
            balance -= amount;
    }

    double getBalance() const
    {
        return balance;
    }
};

int main()
{
    BankAccount account(1000.0);

    std::cout << "Initial balance: " << account.getBalance() << std::endl;

    account.deposit(500.0);
    std::cout << "After deposit: " << account.getBalance() << std::endl;

    account.withdraw(200.0);
    std::cout << "After withdrawal: " << account.getBalance() << std::endl;

    return 0;
}
