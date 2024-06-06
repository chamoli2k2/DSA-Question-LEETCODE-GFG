class Car:
    # constructor
    def __init__(self, name, model, year):
        self.name = name
        self.model = model
        self.year = year

    # method
    def display(self):
        print(self.name, self.model, self.year)


# Inheritance
class ElectricCar(Car):
    # constructor
    def __init__(self, name, model, year, battery):
        # super() is used to call the parent class constructor
        super().__init__(name, model, year)
        self.battery = battery

    # method
    def display(self):
        print(self.name, self.model, self.year, self.battery)


c1 = Car("Audi", "A8", 2020) # object creation
c1.display() # Audi A8 2020
print(c1.name, c1.model, c1.year) # Audi A8 2020


ec1 = ElectricCar("Tesla", "Model S", 2021, "100kWh")
ec1.display() # Tesla Model S 2021 100kWh
print(ec1.name, ec1.model, ec1.year, ec1.battery) # Tesla Model S 2021 100kWh


# Encapsulation
# private variables
class Bike:
    # constructor
    # __name, __model, __year are private variables (encapsulation)
    def __init__(self, name, model, year):
        self.__name = name    # __ is used to make a variable private
        self.__model = model
        self.__year = year

    # method
    def display(self):
        print(self.__name, self.__model, self.__year)

    # getter
    def get_name(self):
        return self.__name

    # setter
    def set_name(self, name):
        self.__name = name  

b1 = Bike("Harley", "Street 750", 2021)
b1.display() # Harley Street 750 2021
print(b1.get_name()) # Harley
b1.set_name("Royal Enfield")
print(b1.get_name()) # Royal Enfield


# Polymorphism
class Animal:
    def __init__(self, name):
        self.name = name

    def speak(self):
        raise NotImplementedError("Subclass must implement this abstract method")

class Dog(Animal):
    def speak(self):
        return self.name + " says Woof!"

class Cat(Animal):
    def speak(self):
        return self.name + " says Meow!"

a1 = Dog("Tommy")
a2 = Cat("Kitty")
print(a1.speak()) # Tommy says Woof!
print(a2.speak()) # Kitty says Meow!


# static method
# static method can be called without creating an object
class Math:
    @staticmethod
    def add(a, b):
        return a + b

    @staticmethod
    def sub(a, b):
        return a - b

print(Math.add(10, 20)) # 30
print(Math.sub(20, 10)) # 10
