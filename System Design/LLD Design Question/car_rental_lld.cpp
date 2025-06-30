#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm>
using namespace std;

// Ye ek Vehicle Rental System hai jisme:
// * Users vehicles rent karte hain.
// * RentalStores vehicles ko manage karte hain.
// * Reservations banayi jaati hain between dates.
// * Payment different strategies (Credit/Cash/PayPal etc.) se ki ja sakti hai.
// * RentalSystem singleton hai, jo central controller hai.

// Flow of Reservation Process:
// ✅ User registers.
// ✅ User selects vehicle, rental period, and strategy.
// ✅ Reservation is created (ReservationManager::createReservation).
// ✅ Payment is processed using PaymentProcessor.
// ✅ If payment is successful, reservation status becomes CONFIRMED.
// ✅ Rental starts: vehicle status becomes RENTED.
// ✅ Rental completes: vehicle status becomes AVAILABLE.

// ✅ Strengths of Your Code
// * Clean modular OOP design.
// * Easily extensible (add new vehicles, payment types, pricing logic).
// * Memory-safe (due to shared_ptr).
// * Business logic properly abstracted.

// 🔒 Agar concurrency nahi daali kaise karoge code mein toh ye Potential Race Conditions bata diyo aur boldiyo ki because of time constraint I did not implement it:
// * Two users try to reserve the same vehicle simultaneously.
// * Payment processed for same reservation in two threads.
// * Vehicle availability changes in between check and reservation.

// 🛡️ Possible Solutions:
// Use std::mutex for locking in RentalStore, ReservationManager, and RentalSystem to lock critical sections
// Use atomic counters for reservation IDs.
// Implement thread-safe queues or reservation pools for large-scale systems.
// atomic counter aisa variable hota hai jiska access (read/write/increment) ek atomic operation ke through hota hai,
// matlab ye operations interrupt nahi ho sakte aur thread-safe hote hain bina kisi mutex lock ke. For example: atomic<int> counter(0);

// Enums: Inhe sabse top par rakhenge
enum class VehicleStatus { AVAILABLE, RESERVED, RENTED, MAINTENANCE, OUT_OF_SERVICE };
enum class VehicleType { ECONOMY, COMPACT, SEDAN, SUV, LUXURY, VAN, TRUCK, BIKE, AUTO };
enum class ReservationStatus { PENDING, CONFIRMED, IN_PROGRESS, COMPLETED, CANCELED };

// Location : Normal class hai user ka address store karne ke liye
class Location {
    string address, city, state, zip;
public:
    Location(string addr, string c, string s, string z) : address(addr), city(c), state(s), zip(z) {}
};

// Abstract Vehicle: Vehicle ek abstract class hai jisme calculateRentalFee virtual hai taaki concrete vehicles apne according price decide kar paaye
class Vehicle {
protected:
    string registrationNumber, model;
    VehicleType type;
    VehicleStatus status;
    double baseRentalPrice;

public:
    Vehicle(string reg, string mdl, VehicleType t, double price)
        : registrationNumber(reg), model(mdl), type(t), baseRentalPrice(price), status(VehicleStatus::AVAILABLE) {}

    virtual double calculateRentalFee(int days) = 0;
    VehicleType getType() const { return type; }
    string getRegistrationNumber() const { return registrationNumber; }
    VehicleStatus getStatus() const { return status; }
    void setStatus(VehicleStatus s) { status = s; }
    double getBaseRentalPrice() const { return baseRentalPrice; }
};

// Concrete Vehicles
class EconomyVehicle : public Vehicle {
public:
    EconomyVehicle(string r, string m, VehicleType t, double p) : Vehicle(r, m, t, p) {}
    double calculateRentalFee(int days) override { return baseRentalPrice * days; }
};

class LuxuryVehicle : public Vehicle {
public:
    LuxuryVehicle(string r, string m, VehicleType t, double p) : Vehicle(r, m, t, p) {}
    double calculateRentalFee(int days) override { return baseRentalPrice * days * 2.5 + 50.0; }
};

class SUVVehicle : public Vehicle {
public:
    SUVVehicle(string r, string m, VehicleType t, double p) : Vehicle(r, m, t, p) {}
    double calculateRentalFee(int days) override { return baseRentalPrice * days * 1.5; }
};

// Vehicle Factory: ye simplify kardeta hai vehicle creation based on VehicleType.
class VehicleFactory {
public:
    static shared_ptr<Vehicle> createVehicle(VehicleType type, string reg, string model, double price) {
        switch (type) {
            case VehicleType::ECONOMY: return make_shared<EconomyVehicle>(reg, model, type, price);
            case VehicleType::LUXURY: return make_shared<LuxuryVehicle>(reg, model, type, price);
            case VehicleType::SUV: return make_shared<SUVVehicle>(reg, model, type, price);
            default: throw invalid_argument("Unsupported vehicle type");
        }
    }
};

// Forward declarations: Inhe karna padega because user and RentalStore class ko hum unki definition se pehle declare karna hoga taaki baaki classes unhe refer kar sake
class User;
class RentalStore;

// Pricing Strategy
class PricingStrategy {
public:
    virtual double calculateRentalPrice(shared_ptr<Vehicle> vehicle, int rentalPeriod) = 0;
    virtual ~PricingStrategy() = default;
};

// Concrete Pricing strategies
class DailyPricingStrategy : public PricingStrategy {
public:
    double calculateRentalPrice(shared_ptr<Vehicle> vehicle, int rentalPeriod) override {
        return vehicle->getBaseRentalPrice() * rentalPeriod;
    }
};

class WeeklyPricingStrategy : public PricingStrategy {
public:
    double calculateRentalPrice(shared_ptr<Vehicle> vehicle, int rentalPeriod) override {
        const double WEEKLY_DISCOUNT = 0.8;
        double dailyRate = vehicle->getBaseRentalPrice();
        int weeks = rentalPeriod / 7;
        int remainingDays = rentalPeriod % 7;

        double weeklyPrice = dailyRate * 7 * WEEKLY_DISCOUNT * weeks;
        double remainingDaysPrice = dailyRate * remainingDays;

        return weeklyPrice + remainingDaysPrice;
    }
};


// Reservation: Isme hum confirm(), start(), complete(), cancel() ko use karke reservation and vehicle status ko update karte rehte hai.
class Reservation {
    int id;
    shared_ptr<User> user;
    shared_ptr<Vehicle> vehicle;
    shared_ptr<RentalStore> pickupStore, returnStore;
    time_t startDate, endDate;
    ReservationStatus status;
    double totalAmount;
    shared_ptr<PricingStrategy> strategy;

public:
    Reservation(int i, shared_ptr<User> u, shared_ptr<Vehicle> v,
        shared_ptr<RentalStore> ps, shared_ptr<RentalStore> rs,
        time_t sd, time_t ed, shared_ptr<PricingStrategy> s)
        : id(i), user(u), vehicle(v), pickupStore(ps), returnStore(rs),
          startDate(sd), endDate(ed), strategy(s), status(ReservationStatus::PENDING) {

        int days = max(1, int(difftime(endDate, startDate) / (60 * 60 * 24)));
        totalAmount = strategy->calculateRentalPrice(vehicle, days);
    }

    int getId() const { return id; }
    double getTotalAmount() const { return totalAmount; }

    void confirm() {
        if (status == ReservationStatus::PENDING) {
            status = ReservationStatus::CONFIRMED;
            vehicle->setStatus(VehicleStatus::RESERVED);
        }
    }

    void start() {
        if (status == ReservationStatus::CONFIRMED) {
            status = ReservationStatus::IN_PROGRESS;
            vehicle->setStatus(VehicleStatus::RENTED);
        }
    }

    void complete() {
        if (status == ReservationStatus::IN_PROGRESS) {
            status = ReservationStatus::COMPLETED;
            vehicle->setStatus(VehicleStatus::AVAILABLE);
        }
    }

    void cancel() {
        if (status == ReservationStatus::PENDING || status == ReservationStatus::CONFIRMED) {
            status = ReservationStatus::CANCELED;
            vehicle->setStatus(VehicleStatus::AVAILABLE);
        }
    }
};


// User: Isme hum reservations ko add and remove kar sakte hai
class User {
    int id;
    string name, email;
    vector<shared_ptr<Reservation>> reservations;

public:
    User(int i, string n, string e) : id(i), name(n), email(e) {}

    int getId() const { return id; }

    void addReservation(shared_ptr<Reservation> r) {
        reservations.push_back(r);
    }

    void deleteReservation(shared_ptr<Reservation> r) {
        auto it = remove_if(reservations.begin(), reservations.end(),
            [&](const shared_ptr<Reservation>& res) {
                return res->getId() == r->getId();
            });
        if (it != reservations.end()) {
            reservations.erase(it, reservations.end());
        }
    }
};


// Rental Store
class RentalStore {
    int id;
    string name;
    Location location;
    unordered_map<string, shared_ptr<Vehicle>> vehicles;

public:
    RentalStore(int i, string n, Location l) : id(i), name(n), location(l) {}

    int getId() const { return id; }

    void addVehicle(shared_ptr<Vehicle> v) {
        vehicles[v->getRegistrationNumber()] = v;
    }

    void removeVehicle(const string& registrationNumber) {
        vehicles.erase(registrationNumber);
    }

    shared_ptr<Vehicle> getVehicle(const string& registrationNumber) {
        auto it = vehicles.find(registrationNumber);
        return (it != vehicles.end()) ? it->second : nullptr;
    }

    bool isVehicleAvailable(const string& registrationNumber) {
        auto it = vehicles.find(registrationNumber);
        return it != vehicles.end() && it->second->getStatus() == VehicleStatus::AVAILABLE;
    }

    vector<shared_ptr<Vehicle>> getAvailableVehicles() {
        vector<shared_ptr<Vehicle>> availableVehicles;
        for (const auto& pair : vehicles) {
            if (pair.second->getStatus() == VehicleStatus::AVAILABLE) {
                availableVehicles.push_back(pair.second);
            }
        }
        return availableVehicles;
    }

    const unordered_map<string, shared_ptr<Vehicle>>& getAllVehicles() const {
        return vehicles;
    }
};


// Reservation Manager: Saari reservations maintain karega like unhe create karega and Allows confirming, starting, completing, or canceling a reservation.
class ReservationManager {
    map<int, shared_ptr<Reservation>> reservations;
    int nextId = 1;

public:
    shared_ptr<Reservation> createReservation(shared_ptr<User> user, shared_ptr<Vehicle> vehicle,
        shared_ptr<RentalStore> pickup, shared_ptr<RentalStore> ret, time_t start, time_t end,
        shared_ptr<PricingStrategy> strategy) {

        auto r = make_shared<Reservation>(nextId++, user, vehicle, pickup, ret, start, end, strategy);
        reservations[r->getId()] = r;
        user->addReservation(r);
        return r;
    }

    shared_ptr<Reservation> getReservation(int id) {
        return reservations.count(id) ? reservations[id] : nullptr;
    }

    void confirmReservation(int id) { if (reservations.count(id)) reservations[id]->confirm(); }
    void startRental(int id) { if (reservations.count(id)) reservations[id]->start(); }
    void completeRental(int id) { if (reservations.count(id)) reservations[id]->complete(); }
    void cancelReservation(int id) { if (reservations.count(id)) reservations[id]->cancel(); }
};

// Payment Strategy
class PaymentStrategy {
public:
    virtual void processPayment(double amount) = 0;
    virtual ~PaymentStrategy() = default;
};

class CreditCardPayment : public PaymentStrategy {
public:
    void processPayment(double amount) override {
        cout << "Processing credit card payment of $" << amount << endl;
    }
};

class DebitCardPayment : public PaymentStrategy {
public:
    void processPayment(double amount) override {
        cout << "Processing debit card payment of $" << amount << endl;
    }
};

class CashPayment : public PaymentStrategy {
public:
    void processPayment(double amount) override {
        cout << "Processing cash payment of $" << amount << endl;
    }
};

class PaypalPayment : public PaymentStrategy {
public:
    void processPayment(double amount) override {
        cout << "Processing PayPal payment of $" << amount << endl;
    }
};


class PaymentProcessor {
public:
    bool processPayment(double amount, shared_ptr<PaymentStrategy> strategy) {
        strategy->processPayment(amount);
        return true;
    }
};

// Singleton Rental System: Ye manage karta hai saare rental stores, saare users, reservation and payment processing
class RentalSystem {
    static shared_ptr<RentalSystem> instance;
    vector<shared_ptr<RentalStore>> stores;
    ReservationManager resManager;
    PaymentProcessor paymentProcessor;
    map<int, shared_ptr<User>> users;

    RentalSystem() {}

public:
    static shared_ptr<RentalSystem> getInstance() {
        if (!instance)
            instance = shared_ptr<RentalSystem>(new RentalSystem());
        return instance;
    }

    void addStore(shared_ptr<RentalStore> s) { stores.push_back(s); }
    shared_ptr<RentalStore> getStore(int id) {
        for (auto& s : stores)
            if (s->getId() == id) return s;
        return nullptr;
    }

    void registerUser(shared_ptr<User> u) { users[u->getId()] = u; }

    shared_ptr<Reservation> createReservation(int uid, string reg, int ps, int rs,
        time_t sd, time_t ed, shared_ptr<PricingStrategy> strat) {

        auto u = users[uid];
        auto store = getStore(ps);
        auto retStore = getStore(rs);
        auto v = store ? store->getVehicle(reg) : nullptr;
        if (u && store && v)
            return resManager.createReservation(u, v, store, retStore, sd, ed, strat);
        return nullptr;
    }

    bool processPayment(int rid, shared_ptr<PaymentStrategy> strat) {
        auto r = resManager.getReservation(rid);
        if (r) {
            paymentProcessor.processPayment(r->getTotalAmount(), strat);
            resManager.confirmReservation(rid);
            return true;
        }static shared_ptr<RentalSystem> getInstance() {
        if (!instance)
            instance = shared_ptr<RentalSystem>(new RentalSystem());
        return instance;
    }
        return false;
    }

    void startRental(int rid) { resManager.startRental(rid); }
    void completeRental(int rid) { resManager.completeRental(rid); }
};

shared_ptr<RentalSystem> RentalSystem::instance = nullptr;

time_t parseDate(const string& dateStr) {
    tm t = {};
    istringstream ss(dateStr);
    ss >> get_time(&t, "%m/%d/%Y");
    return mktime(&t);
}

// MAIN: Yahan hum Sets up stores, vehicles, users and Create karega reservations and Simulate karega payment and rental flow.
int main() {
    auto rentalSystem = RentalSystem::getInstance();

    // Create rental stores
    auto store1 = std::make_shared<RentalStore>(1, "Downtown Rentals", Location("123 Main St", "New York", "NY", "10001"));
    auto store2 = std::make_shared<RentalStore>(2, "Airport Rentals", Location("456 Airport Rd", "Los Angeles", "CA", "90045"));
    rentalSystem->addStore(store1);
    rentalSystem->addStore(store2);

    // Create vehicles using Factory Pattern
    auto economyCar = VehicleFactory::createVehicle(VehicleType::ECONOMY, "EC001", "Toyota", 50.0);
    auto luxuryCar = VehicleFactory::createVehicle(VehicleType::LUXURY, "LX001", "Mercedes", 200.0);
    auto suvCar = VehicleFactory::createVehicle(VehicleType::SUV, "SV001", "Honda", 75.0);

    // Add vehicles to stores
    store1->addVehicle(economyCar);
    store1->addVehicle(luxuryCar);
    store2->addVehicle(suvCar);

    std::cout << "Vehicles in Store 1:\n";
    for (const auto& pair : store1->getAllVehicles()) {
        std::cout << "Vehicle ID: " << pair.first
                  << ", Type: " << static_cast<int>(pair.second->getType())
                  << ", Reg#: " << pair.second->getRegistrationNumber() << std::endl;
    }

    // Register users
    auto user1 = std::make_shared<User>(123, "ABC", "abc@gmail.com");
    auto user2 = std::make_shared<User>(345, "BCD", "bcd@yahoo.com");
    rentalSystem->registerUser(user1);
    rentalSystem->registerUser(user2);

    // Create reservations
    auto startDate1 = parseDate("03/01/2025");
    auto endDate1 = parseDate("03/05/2025");
    auto res1 = rentalSystem->createReservation(user1->getId(), "EC001", store1->getId(), store1->getId(), startDate1, endDate1, std::make_shared<DailyPricingStrategy>());

    auto startDate2 = parseDate("03/01/2025");
    auto endDate2 = parseDate("03/22/2025");
    auto res2 = rentalSystem->createReservation(user1->getId(), "LX001", store1->getId(), store1->getId(), startDate2, endDate2, std::make_shared<WeeklyPricingStrategy>());

    // Simulate payment
    auto reservation = res2;
    if (reservation) {
        std::cout << "\nProcessing payment for reservation #" << reservation->getId() << "\n";
        std::cout << "Total amount: $" << reservation->getTotalAmount() << "\n";
        std::cout << "Select payment method:\n1. Credit Card\n2. Cash\n3. PayPal\n4. Debit Card\n";

        int choice = 1; // Simulated user input
        std::shared_ptr<PaymentStrategy> paymentStrategy;
        switch (choice) {
            case 1: paymentStrategy = std::make_shared<CreditCardPayment>(); break;
            case 2: paymentStrategy = std::make_shared<CashPayment>(); break;
            case 3: paymentStrategy = std::make_shared<PaypalPayment>(); break;
            case 4: paymentStrategy = std::make_shared<DebitCardPayment>(); break;
            default:
                std::cout << "Invalid choice! Using Credit Card by default.\n";
                paymentStrategy = std::make_shared<CreditCardPayment>(); break;
        }

        if (rentalSystem->processPayment(reservation->getId(), paymentStrategy)) {
            std::cout << "Payment successful!\nSimulating rental period...\n";
            rentalSystem->startRental(reservation->getId());
            rentalSystem->completeRental(reservation->getId());
        } else {
            std::cout << "Payment failed!\n";
        }
    }

    return 0;
}