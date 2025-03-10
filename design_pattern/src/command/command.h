#pragma once

#include <list>
#include <iostream>
#include <memory>

class Barbecuer {
public:
    ~Barbecuer() {
        std::cout << "~Barbecuer" << std::endl;
    }

    void bakeMutton() {
        std::cout << "烤羊肉串" << std::endl;
    }

    void bakeChickenWing() {
        std::cout << "烤鸡翅" << std::endl;
    }
};

class Command {
protected:
    std::shared_ptr<Barbecuer> receiver;
public:
    Command(std::shared_ptr<Barbecuer> barbecuer) :receiver(barbecuer) {
    }

    virtual void excute() = 0;
};

class BakeMuttonCommand : public Command {
public:
    BakeMuttonCommand(std::shared_ptr<Barbecuer> barbecuer) : Command(barbecuer) {
    }

    void excute() {
        receiver->bakeMutton();
    }
};

class BakeChickenWingCommand : public Command
{
public:
    BakeChickenWingCommand(std::shared_ptr<Barbecuer> barbecuer) : Command(barbecuer) {
    }

    void excute() {
        receiver->bakeChickenWing();
    }
};

class Waiter {
private:
    std::list<std::shared_ptr<Command>> orders;

public:
    void setOrder(std::shared_ptr<Command> cmd) {
        orders.push_back(cmd);
    }

    void cancelOrder(std::shared_ptr<Command> cmd) {
        orders.remove(cmd);
    }

    void notify() {
        for (auto& cmd : orders) {
            cmd->excute();
        }
        orders.clear();
    }
};