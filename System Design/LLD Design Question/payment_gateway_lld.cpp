#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <thread>
#include <chrono>
using namespace std;

enum class TransactionStatus { INITIATED, AUTHENTICATED, SUCCESS, FAILED };

// 👤 User class
class User {
    int id;
    string name;
public:
    User(int _id, string _name) : id(_id), name(_name) {}
    int getId() const { return id; }
    string getName() const { return name; }
};

// 💳 PaymentStrategy Interface (Strategy Pattern)
class PaymentMethod {
public:
    virtual bool authenticate() = 0;
    virtual bool pay(double amount) = 0;
    virtual ~PaymentMethod() = default;
};

// 💳 Credit Card Payment
class CreditCard : public PaymentMethod {
public:
    bool authenticate() override {
        cout << "Credit Card authentication done.\n";
        return true; // simulate success
    }
    bool pay(double amount) override {
        cout << "Paid ₹" << amount << " via Credit Card.\n";
        return true; // simulate success
    }
};

// 🪙 UPI Payment
class UPI : public PaymentMethod {
public:
    bool authenticate() override {
        cout << "UPI authentication done.\n";
        return true; // simulate success
    }
    bool pay(double amount) override {
        cout << "Paid ₹" << amount << " via UPI.\n";
        return true; // simulate success
    }
};

// 📦 Transaction Class
class Transaction {
    int id;
    double amount;
    TransactionStatus status;
public:
    Transaction(int _id, double amt)
        : id(_id), amount(amt), status(TransactionStatus::INITIATED) {}

    int getId() const { return id; }
    TransactionStatus getStatus() const { return status; }

    void setStatus(TransactionStatus s) {
        status = s;
        cout << "Transaction #" << id << " status -> " << static_cast<int>(status) << "\n";
    }

    double getAmount() const { return amount; }
};

// 🧠 Payment Processor
class PaymentProcessor {
    int nextTxnId = 1;
    unordered_map<int, shared_ptr<Transaction>> txnDB;

public:
    shared_ptr<Transaction> initiateTransaction(double amount) {
        auto txn = make_shared<Transaction>(nextTxnId++, amount);
        txnDB[txn->getId()] = txn;
        return txn;
    }

    // 🔁 Retry mechanism added
    bool processPayment(shared_ptr<Transaction> txn, shared_ptr<PaymentMethod> method, int retryLimit = 3) {
        if (!txn) return false;

        txn->setStatus(TransactionStatus::INITIATED);
        int attempts = 0;

        while (attempts < retryLimit) {
            cout << "Attempt " << (attempts + 1) << ":\n";

            if (!method->authenticate()) {
                cout << "Authentication failed.\n";
                attempts++;
                continue;
            }

            txn->setStatus(TransactionStatus::AUTHENTICATED);

            if (!method->pay(txn->getAmount())) {
                cout << "Payment failed.\n";
                attempts++;
                continue;
            }

            txn->setStatus(TransactionStatus::SUCCESS);
            return true;
        }

        txn->setStatus(TransactionStatus::FAILED);
        return false;
    }

    shared_ptr<Transaction> getTransaction(int id) {
        return txnDB.count(id) ? txnDB[id] : nullptr;
    }
};

// 🌐 Payment Gateway - Merchant isse use karega
class PaymentGateway {
    PaymentProcessor processor;
    unordered_map<int, shared_ptr<User>> users;

public:
    void registerUser(shared_ptr<User> user) {
        users[user->getId()] = user;
    }

    bool makePayment(int userId, double amount, shared_ptr<PaymentMethod> method) {
        if (!users.count(userId)) {
            cout << "❌ User not found.\n";
            return false;
        }

        auto txn = processor.initiateTransaction(amount);
        cout << "\n🟢 Transaction #" << txn->getId() << " started for ₹" << amount << "\n";

        if (processor.processPayment(txn, method)) {
            cout << "✅ Transaction #" << txn->getId() << " successful.\n";
            return true;
        } else {
            cout << "❌ Transaction #" << txn->getId() << " failed after retries.\n";
            return false;
        }
    }
};

// 🔁 Singleton instance can also be added if needed

// 🚀 main()
int main() {
    PaymentGateway gateway;

    // 👤 Register user
    auto user = make_shared<User>(101, "Gaurav");
    gateway.registerUser(user);

    // 💰 Pay with Credit Card
    auto creditCard = make_shared<CreditCard>();
    gateway.makePayment(user->getId(), 1000.0, creditCard);

    // 💰 Pay with UPI
    auto upi = make_shared<UPI>();
    gateway.makePayment(user->getId(), 500.0, upi);

    return 0;
}
