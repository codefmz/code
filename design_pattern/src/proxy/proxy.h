#pragma once
#include <string>
#include <iostream>

class Behaver {
public:
    virtual ~Behaver() = default;
    virtual void GiveFlowers() = 0;
};

class Pursuit : public Behaver {
private:
    std::string girlName;
    std::string name;

public:
    Pursuit(std::string girlName, std::string name)
        : girlName(girlName), name(name) {
    }
    virtual void GiveFlowers() override {
        std::cout << name << " 送花给 " << girlName << std::endl;
    }
};

class Proxy : public Behaver {
private:
    Behaver* behaver;
    std::string name;

public:
    Proxy(std::string _name, Behaver* _behaver) name(_name), behaver(_behaver) {
    }
    virtual void GiveFlowers() override {
        std::cout << name << " 代替 ";
        behaver->GiveFlowers();
    }
};