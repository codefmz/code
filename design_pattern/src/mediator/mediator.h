#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <iostream>
#include <memory>

class Country;

class UnitedNations {
protected:
    std::shared_ptr<Country> usa;
    std::shared_ptr<Country> iraq;
public:
    virtual void declare(const std::string & message, const std::string & name) = 0;
    virtual ~UnitedNations() = default;
    void setUsa(std::shared_ptr<Country> &usa) {
        this->usa = usa;
    }

    void setIraq(std::shared_ptr<Country> &iraq) {
        this->iraq = iraq;
    }
};

class Country {
public:
    Country(std::string name, std::shared_ptr<UnitedNations> mediator)
        :name(name), mediator(mediator) {}
    virtual void declare(const std::string & message) = 0;

    virtual void receive(const std::string & message, const std::string & countryName) = 0;
    virtual ~Country() = default;

    const std::string& getName() const {
        return name;
    }

protected:
    std::string name;
    std::shared_ptr<UnitedNations> mediator;
};

class USA : public Country {
public:
    USA(std::shared_ptr<UnitedNations> mediator)
        :Country("USA", mediator) {}
    virtual void declare(const std::string & message) {
        mediator->declare(message, this->getName());
    }

    virtual void receive(const std::string & message, const std::string & countryName) {
        std::cout << name << " receive message from " << countryName << ": " << message << std::endl;
    }
    virtual ~USA() = default;
};

class Iraq : public Country {
public:
    Iraq(std::shared_ptr<UnitedNations> mediator)
        :Country("Iraq", mediator) {}
    virtual void declare(const std::string & message) {
        mediator->declare(message, this->getName());
    }

    virtual void receive(const std::string & message, const std::string & countryName) {
        std::cout << name << " receive message from " << countryName << ": " << message << std::endl;
    }

    virtual ~Iraq() = default;
};

class UnitedNationsSecurityCouncil : public UnitedNations {
public:
    UnitedNationsSecurityCouncil() {
    }

    virtual void declare(const std::string & message, const std::string & name ) override {
        if (name == "USA") {
            iraq->receive(message, name);
        } else {
            usa->receive(message, name);
        }
    }
};

#endif