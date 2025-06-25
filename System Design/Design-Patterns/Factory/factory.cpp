#include <bits/stdc++.h>
using namespace std;

//////////////////////////////////////////////////////
// Abstract Base Class: Animal
//////////////////////////////////////////////////////
class Animal {
public:
    // Pure virtual function (must be overridden by derived classes)
    virtual void sound() = 0;

    // A normal function - just a utility to demonstrate behavior
    bool isAnimal() {
        return true;
    }

    // Virtual destructor for safe deletion of derived class objects
    virtual ~Animal() {}
};

//////////////////////////////////////////////////////
// Derived Class: Cat
//////////////////////////////////////////////////////
class Cat : public Animal {
private:
    string name;
public:
    // Constructor to initialize name
    Cat(string name) {
        this->name = name;
    }

    // Overriding the sound method
    void sound() override {
        cout << "Name: " << name << " ";
        cout << "Meow" << endl;
    }
};

//////////////////////////////////////////////////////
// Derived Class: Dog
//////////////////////////////////////////////////////
class Dog : public Animal {
private:
    string name;
public:
    // Constructor to initialize name
    Dog(string name) {
        this->name = name;
    }

    // Overriding the sound method
    void sound() override {
        cout << "Name: " << name << " ";
        cout << "Bark" << endl;
    }
};

//////////////////////////////////////////////////////
// Factory Class: AnimalFactory
//////////////////////////////////////////////////////
class AnimalFactory {
public:
    // Factory method to create and return an Animal pointer
    static Animal* createAnimal(int typeNum, string name) {
        if (typeNum == 1) {
            return new Cat(name);   // Return pointer to Cat
        }
        else if (typeNum == 2) {
            return new Dog(name);   // Return pointer to Dog
        }
        else {
            return nullptr;         // Invalid type
        }
    }
};

//////////////////////////////////////////////////////
// Main function
//////////////////////////////////////////////////////
int main() {
    // ❌ Logical mistake:
    // You were trying to assign Animal* to Cat* and Dog*, which is unsafe.
    // You should use Animal* to safely store the returned object.

    // ✅ Correct: Use Animal* to store both
    Animal* d = AnimalFactory::createAnimal(2, "Dogesh");  // Dog
    Animal* c = AnimalFactory::createAnimal(1, "Billu");   // Cat

    // Call sound function via base class pointer
    cout << "This is dog: ";
    d->sound();  // Calls Dog's sound

    cout << "This is cat: ";
    c->sound();  // Calls Cat's sound

    // Always delete heap-allocated objects to avoid memory leaks
    delete d;
    delete c;

    return 0;
}
