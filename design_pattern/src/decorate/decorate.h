#pragma once
#include <iostream>

class Person {
private:
    std::string name;

public:
    Person() {
    }
    Person(std::string name) : name(name) {
    }
    virtual ~Person() = default;
    virtual void show() {
        std::cout << name << " wear: ";
    };
};

class Finery : public Person {
protected:
    Person* person;

public:
    Finery() {
        person = nullptr;
    }

    virtual ~Finery() = default;
    virtual void show() {
        if (person != nullptr) {
            person->show();
        }
    };
    virtual Person* Decorate(Person* person) {
        this->person = person;
        return this;
    }
};

class TShirt : public Finery {
public:
    void show() override {
        Finery::show();
        std::cout << "TShirt ";
    };
};

class Jeans : public Finery {
public:
    void show() override {
        Finery::show();
        std::cout << "Jeans ";
    };
};

class NikeShoes : public Finery {
public:
    void show() override {
        Finery::show();
        std::cout << "NikeShoes ";
    };
};

class AntaShoes : public Finery {
public:
    void show() override {
        Finery::show();
        std::cout << "AntaShoes ";
    };
};

class Adadas : public Finery {
public:
    void show() override {
        Finery::show();
        std::cout << "Adadas ";
    };
};