#include <bits/stdc++.h>
using namespace std;

////////////////////////////////////////////////////
// Abstract Observer Interface
////////////////////////////////////////////////////
class Observer {
public:
    virtual void update(int value) = 0;
    virtual void showState() = 0;
    virtual ~Observer() {}  // ✅ Virtual destructor
};

////////////////////////////////////////////////////
// Concrete Observer
////////////////////////////////////////////////////
class Subscriber : public Observer {
private:
    int state;
public:
    Subscriber() : state(0) {}

    void update(int value) override {
        state = value;
    }

    void showState() override {
        cout << "Current State Value: " << state << endl;
    }
};

////////////////////////////////////////////////////
// Subject / Publisher Class
////////////////////////////////////////////////////
class State {
private:
    int state;
    vector<Observer*> observers;
public:
    // Add an observer to the list
    void subscribe(Observer* o) {
        observers.push_back(o);
    }

    // Change the internal state and notify observers
    void updateState(int newVal) {
        state = newVal;
        notifyAll();
    }

    // Notify all observers about the state change
    void notifyAll() {
        for (auto& obs : observers) {
            obs->update(state);
        }
    }
};

////////////////////////////////////////////////////
// Main: Usage
////////////////////////////////////////////////////
int main() {
    // Creating Subscribers
    Observer* s1 = new Subscriber();
    Observer* s2 = new Subscriber();
    Observer* s3 = new Subscriber();

    // Creating Publisher (Subject)
    State* myChannel = new State();

    // Subscribing observers
    myChannel->subscribe(s1);
    myChannel->subscribe(s3);

    // Updating the state — observers should be notified
    myChannel->updateState(69);

    // Displaying internal state of all subscribers
    cout << "Subscriber 1: "; s1->showState();  // gets update
    cout << "Subscriber 2: "; s2->showState();  // not subscribed, state = 0
    cout << "Subscriber 3: "; s3->showState();  // gets update

    // ✅ Cleaning up to avoid memory leak
    delete s1;
    delete s2;
    delete s3;
    delete myChannel;

    return 0;
}
