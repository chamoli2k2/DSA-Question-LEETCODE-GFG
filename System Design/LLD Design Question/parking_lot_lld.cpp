#include<bits/stdc++.h>
#include <iostream>           // for std::cout, std::endl
#include <string>             // for std::string
#include <memory>             // for std::shared_ptr, std::make_shared
#include <vector>             // for std::vector
#include <unordered_map>      // for std::unordered_map
#include <mutex>              // for std::mutex, std::lock_guard
#include <thread>             // for std::this_thread::sleep_for
#include <chrono>             // for std::chrono::seconds
#include <ctime>              // for std::time, std::difftime
#include <algorithm>          // for std::max
using namespace std;

enum class VehicleType { BIKE, CAR, TRUCK };
enum class SlotStatus { FREE, OCCUPIED };
enum class TicketStatus { ACTIVE, PAID };

class Vehicle {
public:
    string number;
    VehicleType type;
    Vehicle(string num, VehicleType t) : number(num), type(t) {}
};

class VehicleFactory {
public:
    static shared_ptr<Vehicle> create(string number, VehicleType type) {
        return make_shared<Vehicle>(number, type);
    }
};

class Slot {
    int slotId;
    VehicleType supportedType;
    SlotStatus status;
    mutex mtx;

public:
    Slot(int id, VehicleType type) : slotId(id), supportedType(type), status(SlotStatus::FREE) {}

    bool tryOccupy() {
        lock_guard<mutex> lock(mtx);
        if (status == SlotStatus::FREE) {
            status = SlotStatus::OCCUPIED;
            return true;
        }
        return false;
    }

    void freeSlot() {
        lock_guard<mutex> lock(mtx);
        status = SlotStatus::FREE;
    }

    VehicleType getType() const { return supportedType; }
    SlotStatus getStatus() const { return status; }
    int getId() const { return slotId; }
};

class ParkingFloor {
    int floorId;
    vector<shared_ptr<Slot>> slots;

public:
    ParkingFloor(int id) : floorId(id) {}

    void addSlot(shared_ptr<Slot> slot) {
        slots.push_back(slot);
    }

    shared_ptr<Slot> getFreeSlot(VehicleType type) {
        for (auto& slot : slots) {
            if (slot->getType() == type && slot->getStatus() == SlotStatus::FREE) {
                if (slot->tryOccupy()) return slot;
            }
        }
        return nullptr;
    }

    void showStatus() {
        for (auto& s : slots) {
            cout << "Slot " << s->getId() << ": "
                 << (s->getStatus() == SlotStatus::FREE ? "FREE" : "OCCUPIED") << "\n";
        }
    }
};

class PricingStrategy {
public:
    virtual double calculate(time_t start, time_t end, VehicleType type) = 0;
};

class FlatRate : public PricingStrategy {
public:
    double calculate(time_t, time_t, VehicleType) override {
        return 100.0;
    }
};

class HourlyRate : public PricingStrategy {
public:
    double calculate(time_t start, time_t end, VehicleType type) override {
        int hours = max(1, int(difftime(end, start) / 3600));
        switch (type) {
            case VehicleType::BIKE: return hours * 10.0;
            case VehicleType::CAR: return hours * 20.0;
            case VehicleType::TRUCK: return hours * 50.0;
        }
        return 0.0;
    }
};

class Ticket {
public:
    int ticketId;
    string vehicleNum;
    time_t entryTime, exitTime;
    int slotId;
    TicketStatus status = TicketStatus::ACTIVE;
    double amount = 0.0;

    Ticket(int id, string num, int slot) : ticketId(id), vehicleNum(num), slotId(slot) {
        entryTime = time(nullptr);
    }

    void closeTicket(shared_ptr<PricingStrategy> strat, VehicleType type) {
        exitTime = time(nullptr);
        amount = strat->calculate(entryTime, exitTime, type);
        status = TicketStatus::PAID;
    }
};

class TicketFactory {
    int nextId = 1;
public:
    shared_ptr<Ticket> generate(string vehicleNum, int slotId) {
        return make_shared<Ticket>(nextId++, vehicleNum, slotId);
    }
};


class Entrance {
public:
    int id;
    Entrance(int _id) : id(_id) {}
};

class Exit {
public:
    int id;
    Exit(int _id) : id(_id) {}
};


class ParkingLotSystem {
    static shared_ptr<ParkingLotSystem> instance;
    vector<shared_ptr<ParkingFloor>> floors;
    TicketFactory ticketFactory;
    unordered_map<string, shared_ptr<Ticket>> activeTickets;
    mutex sysMutex;

    ParkingLotSystem() {}

public:
    static shared_ptr<ParkingLotSystem> getInstance() {
        static shared_ptr<ParkingLotSystem> instance(new ParkingLotSystem());
        return instance;
    }

    void addFloor(shared_ptr<ParkingFloor> floor) {
        floors.push_back(floor);
    }

    shared_ptr<Ticket> enter(shared_ptr<Vehicle> v) {
        lock_guard<mutex> lock(sysMutex);
        for (auto& floor : floors) {
            auto slot = floor->getFreeSlot(v->type);
            if (slot) {
                auto ticket = ticketFactory.generate(v->number, slot->getId());
                activeTickets[v->number] = ticket;
                cout << "✅ Vehicle " << v->number << " entered. Ticket ID: " << ticket->ticketId << "\n";
                return ticket;
            }
        }
        cout << "❌ No Slot Available\n";
        return nullptr;
    }

    void exitVehicle(string vehicleNum, VehicleType type, shared_ptr<PricingStrategy> strategy) {
        lock_guard<mutex> lock(sysMutex);
        auto it = activeTickets.find(vehicleNum);
        if (it != activeTickets.end()) {
            auto ticket = it->second;
            ticket->closeTicket(strategy, type);
            cout << "💳 Amount to Pay for Vehicle " << vehicleNum << ": ₹" << ticket->amount << "\n";
            activeTickets.erase(it);
        } else {
            cout << "❌ Ticket not found!\n";
        }
    }

    void displayStatus() {
        for (auto& floor : floors) floor->showStatus();
    }
};

int main() {
    auto system = ParkingLotSystem::getInstance();

    // Setup
    auto floor1 = make_shared<ParkingFloor>(1);
    floor1->addSlot(make_shared<Slot>(101, VehicleType::CAR));
    floor1->addSlot(make_shared<Slot>(102, VehicleType::BIKE));
    floor1->addSlot(make_shared<Slot>(103, VehicleType::TRUCK));
    system->addFloor(floor1);

    // Vehicles
    auto car1 = VehicleFactory::create("DL01AB1234", VehicleType::CAR);
    auto bike1 = VehicleFactory::create("DL02YZ9876", VehicleType::BIKE);

    // Entry
    auto ticket1 = system->enter(car1);
    auto ticket2 = system->enter(bike1);

    system->displayStatus();

    // Simulate time
    this_thread::sleep_for(chrono::seconds(3));

    // Exit
    auto pricing = make_shared<HourlyRate>();
    system->exitVehicle(car1->number, car1->type, pricing);
    system->exitVehicle(bike1->number, bike1->type, pricing);

    return 0;
}
