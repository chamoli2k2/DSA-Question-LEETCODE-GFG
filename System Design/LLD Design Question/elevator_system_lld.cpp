#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <mutex>
#include <thread>
#include <chrono>
#include <memory>
#include <condition_variable>
#include <algorithm>
#include <bits/stdc++.h>
using namespace std;

enum class Direction { UP, DOWN, IDLE };

class Request {
public:
    int floor;
    Direction direction;

    Request(int f, Direction d) : floor(f), direction(d) {}
};

class Elevator {
    int id;
    int currentFloor;
    Direction currentDirection;
    set<int> requests; // SCAN: ordered set of floors
    mutex mtx;

public:
    Elevator(int _id) : id(_id), currentFloor(0), currentDirection(Direction::IDLE) {}

    void addRequest(int floor) {
        lock_guard<mutex> lock(mtx);
        requests.insert(floor);
    }

    void run() {
        while (true) {
            this_thread::sleep_for(chrono::seconds(1));
            lock_guard<mutex> lock(mtx);

            if (requests.empty()) {
                currentDirection = Direction::IDLE;
                continue;
            }

            if (currentDirection == Direction::IDLE) {
                currentDirection = *requests.begin() > currentFloor ? Direction::UP : Direction::DOWN;
            }

            if (currentDirection == Direction::UP) {
                auto it = requests.lower_bound(currentFloor);
                if (it == requests.end()) {
                    currentDirection = Direction::DOWN;
                    continue;
                }
                currentFloor = *it;
                cout << "[Elevator " << id << "] Reached floor " << currentFloor << " (UP)\n";
                requests.erase(it);
            } else {
                auto it = requests.upper_bound(currentFloor);
                if (it == requests.begin()) {
                    currentDirection = Direction::UP;
                    continue;
                }
                --it;
                currentFloor = *it;
                cout << "[Elevator " << id << "] Reached floor " << currentFloor << " (DOWN)\n";
                requests.erase(it);
            }
        }
    }

    int getCurrentFloor() {
        lock_guard<mutex> lock(mtx);
        return currentFloor;
    }

    Direction getDirection() {
        lock_guard<mutex> lock(mtx);
        return currentDirection;
    }
};


class ElevatorSystem {
    vector<shared_ptr<Elevator>> elevators;

public:
    ElevatorSystem(int count) {
        for (int i = 0; i < count; ++i) {
            auto e = make_shared<Elevator>(i);
            elevators.push_back(e);
            thread([e]() { e->run(); }).detach();
        }
    }

    void requestElevator(int floor, Direction direction) {
        // Choose best elevator using a simple strategy: closest idle or in same direction
        shared_ptr<Elevator> selected = nullptr;
        int minDistance = INT_MAX;

        for (auto& e : elevators) {
            int dist = abs(e->getCurrentFloor() - floor);
            Direction dir = e->getDirection();

            if (dir == Direction::IDLE || dir == direction) {
                if (dist < minDistance) {
                    minDistance = dist;
                    selected = e;
                }
            }
        }

        if (selected) {
            selected->addRequest(floor);
            cout << "Request for floor " << floor << " assigned to Elevator " << selected->getCurrentFloor() << "\n";
        } else {
            cout << "No available elevator for request to floor " << floor << "\n";
        }
    }
};

int main() {
    ElevatorSystem system(3); // 3 elevators

    // Simulate external requests
    system.requestElevator(5, Direction::UP);
    this_thread::sleep_for(chrono::seconds(1));

    system.requestElevator(2, Direction::DOWN);
    this_thread::sleep_for(chrono::seconds(1));

    system.requestElevator(8, Direction::UP);
    this_thread::sleep_for(chrono::seconds(1));

    system.requestElevator(3, Direction::DOWN);

    // Keep main thread alive
    while (true) {
        this_thread::sleep_for(chrono::seconds(10));
    }

    return 0;
}
