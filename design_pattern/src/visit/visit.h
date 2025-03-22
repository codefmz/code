#pragma once
#include <iostream>
#include <string>

using namespace std;

class Person;

class Action {
protected:
    std::string type;
public:
    Action(const std::string& type) : type(type) { }
    virtual ~Action() = default;
    virtual void manAction(Person* person) = 0;
    virtual void womanAction(Person* person) = 0;
};

class Person {
protected:
    string type;
public:
    Person(const std::string& type) : type(type) { }
    virtual ~Person() = default;
    virtual void accept(Action* action) = 0;

    std::string & getType() {
        return type;
    }
};

class Woman : public Person {
public:
    Woman(const std::string& type) : Person(type) { }
    virtual void accept(Action* action) override {
        action->womanAction(this);
    }
};

class Man : public Person {
public:
    Man(const std::string& type) : Person(type) { }
    virtual void accept(Action* action) override {
        action->manAction(this);
    }
};

class Success : public Action
{
public:
    Success(const std::string& type) : Action(type) { }
    virtual void manAction(Person* person) override {
        std::cout << person->getType() << type << " 时，背后有一个伟大的女人。" << std::endl;
    }
    virtual void womanAction(Person* person) override {
        std::cout << person->getType() << type << " 时，背后有一个失败的男人。" << std::endl;
    }
};

class Failing : public Action
{
public:
    Failing(const std::string& type) : Action(type) { }
    virtual void manAction(Person* person) override {
        std::cout << person->getType() << type << " 时，闷头喝酒，谁也不用劝。" << std::endl;
    }
    virtual void womanAction(Person* person) override {
        std::cout << person->getType() << type << " 时，眼泪汪汪，谁也劝不了。" << std::endl;
    }
};