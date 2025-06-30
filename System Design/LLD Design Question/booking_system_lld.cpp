#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

enum class BookingStatus { PENDING, CONFIRMED, CANCELLED, COMPLETED };

/// 🧾 Payment Strategy Pattern
class PaymentStrategy {
public:
    virtual void pay(double amount) = 0;
    virtual ~PaymentStrategy() = default;
};

class UPI : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "UPI se ₹" << amount << " ka payment ho gaya.\n";
    }
};

class CreditCard : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Credit card se ₹" << amount << " ka payment ho gaya.\n";
    }
};

/// 💰 Pricing Strategy Pattern
class PricingStrategy {
public:
    virtual double calculatePrice(int duration) = 0;
    virtual ~PricingStrategy() = default;
};

class FlatRatePricing : public PricingStrategy {
public:
    double calculatePrice(int duration) override {
        return 500.0 * duration; // Har din ₹500
    }
};

/// 🧱 Abstract Bookable Entity - Jaise Room, Cab, Seat etc.
class BookableEntity {
protected:
    string id;
    bool available = true;

public:
    BookableEntity(string _id) : id(_id) {}
    virtual ~BookableEntity() = default;

    string getId() const { return id; }
    bool isAvailable() const { return available; }
    void setAvailability(bool val) { available = val; }

    virtual string getEntityType() const = 0;
};

/// 🏨 Room Entity - Hotel booking ke liye
class Room : public BookableEntity {
public:
    Room(string _id) : BookableEntity(_id) {}
    string getEntityType() const override { return "Room"; }
};

/// ✈️ Seat Entity - Flight seat ke liye
class Seat : public BookableEntity {
public:
    Seat(string _id) : BookableEntity(_id) {}
    string getEntityType() const override { return "Seat"; }
};

/// 🏭 Factory Pattern - Entity create karne ke liye
class EntityFactory {
public:
    static shared_ptr<BookableEntity> createEntity(string type, string id) {
        if (type == "Room") return make_shared<Room>(id);
        if (type == "Seat") return make_shared<Seat>(id);
        throw invalid_argument("Invalid entity type");
    }
};

/// 👤 User class - Booking karne wala user
class User {
    int id;
    string name;
public:
    User(int _id, string _name) : id(_id), name(_name) {}
    int getId() const { return id; }
    string getName() const { return name; }
};

/// 📅 Booking class - Har ek booking ko represent karta hai
class Booking {
    int id;
    shared_ptr<User> user;
    shared_ptr<BookableEntity> entity;
    time_t startDate, endDate;
    BookingStatus status = BookingStatus::PENDING;
    double price;

public:
    Booking(int _id, shared_ptr<User> _user, shared_ptr<BookableEntity> _entity,
            time_t _start, time_t _end, double _price)
        : id(_id), user(_user), entity(_entity), startDate(_start),
          endDate(_end), price(_price) {}

    int getId() const { return id; }

    void confirm() {
        if (status == BookingStatus::PENDING) {
            status = BookingStatus::CONFIRMED;
            entity->setAvailability(false);
        }
    }

    void cancel() {
        if (status == BookingStatus::PENDING || status == BookingStatus::CONFIRMED) {
            status = BookingStatus::CANCELLED;
            entity->setAvailability(true);
        }
    }

    void complete() {
        if (status == BookingStatus::CONFIRMED) {
            status = BookingStatus::COMPLETED;
            entity->setAvailability(true);
        }
    }
};

/// 🧠 Booking Manager - Booking banata hai aur manage karta hai
class BookingManager {
    int nextId = 1;
    unordered_map<int, shared_ptr<Booking>> bookings;

public:
    shared_ptr<Booking> createBooking(shared_ptr<User> user,
                                      shared_ptr<BookableEntity> entity,
                                      time_t startDate, time_t endDate,
                                      shared_ptr<PricingStrategy> pricing) {
        if (!entity->isAvailable()) return nullptr;

        int duration = max(1, int(difftime(endDate, startDate) / (60 * 60 * 24)));
        double price = pricing->calculatePrice(duration);

        auto booking = make_shared<Booking>(nextId++, user, entity, startDate, endDate, price);
        bookings[booking->getId()] = booking;
        return booking;
    }

    shared_ptr<Booking> getBooking(int id) {
        return bookings.count(id) ? bookings[id] : nullptr;
    }
};

/// 🔄 Payment Processor
class PaymentProcessor {
public:
    bool process(double amount, shared_ptr<PaymentStrategy> strategy) {
        strategy->pay(amount);
        return true;
    }
};

/// 🧩 Singleton Booking System - Ye central controller hai
class BookingSystem {
    static shared_ptr<BookingSystem> instance;
    BookingManager bookingManager;
    PaymentProcessor paymentProcessor;
    unordered_map<int, shared_ptr<User>> users;
    vector<shared_ptr<BookableEntity>> allEntities;

    BookingSystem() {}

public:
    static shared_ptr<BookingSystem> getInstance() {
        if (!instance)
            instance = shared_ptr<BookingSystem>(new BookingSystem());
        return instance;
    }

    void registerUser(shared_ptr<User> user) {
        users[user->getId()] = user;
    }

    void addEntity(shared_ptr<BookableEntity> entity) {
        allEntities.push_back(entity);
    }

    shared_ptr<Booking> book(int uid, string entityId, time_t start, time_t end,
                              shared_ptr<PricingStrategy> pricing) {
        auto user = users[uid];
        auto it = find_if(allEntities.begin(), allEntities.end(),
                          [&](shared_ptr<BookableEntity> e) {
                              return e->getId() == entityId && e->isAvailable();
                          });
        if (user && it != allEntities.end()) {
            return bookingManager.createBooking(user, *it, start, end, pricing);
        }
        return nullptr;
    }

    bool pay(int bookingId, shared_ptr<PaymentStrategy> strategy) {
        auto booking = bookingManager.getBooking(bookingId);
        if (booking) {
            paymentProcessor.process(500.0, strategy); // Dummy price
            booking->confirm();
            return true;
        }
        return false;
    }

    void completeBooking(int bookingId) {
        auto booking = bookingManager.getBooking(bookingId);
        if (booking) booking->complete();
    }

    void cancelBooking(int bookingId) {
        auto booking = bookingManager.getBooking(bookingId);
        if (booking) booking->cancel();
    }
};

shared_ptr<BookingSystem> BookingSystem::instance = nullptr;

time_t parseDate(const string& dateStr) {
    tm t = {};
    istringstream ss(dateStr);
    ss >> get_time(&t, "%m/%d/%Y");
    return mktime(&t);
}

int main() {
    auto system = BookingSystem::getInstance();

    // Step 1: Register users
    auto user1 = make_shared<User>(101, "Gaurav");
    system->registerUser(user1);

    // Step 2: Add bookable entities (Room and Seat)
    auto room = EntityFactory::createEntity("Room", "RM101");
    auto seat = EntityFactory::createEntity("Seat", "SEAT1");

    system->addEntity(room);
    system->addEntity(seat);

    // Step 3: Book the Room
    time_t startDate = parseDate("07/01/2025");
    time_t endDate = parseDate("07/05/2025");

    auto booking = system->book(user1->getId(), "RM101", startDate, endDate, make_shared<FlatRatePricing>());
    if (booking) {
        cout << "\nBooking ID: " << booking->getId() << " created successfully.\n";
    } else {
        cout << "\nBooking failed. Room might be unavailable.\n";
        return 1;
    }

    // Step 4: Process payment
    auto paymentMethod = make_shared<CreditCard>();
    if (system->pay(booking->getId(), paymentMethod)) {
        cout << "Payment successful and booking confirmed.\n";
    }

    // Step 5: Complete the booking
    system->completeBooking(booking->getId());
    cout << "Booking completed.\n";

    return 0;
}
