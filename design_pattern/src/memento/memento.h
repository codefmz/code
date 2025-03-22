 #pragma once
 #include <iostream>
 #include <memory>
 #include <list>

class Memento {
public:
    int hp;
    int ack;
    int def;
    Memento(int hp, int ack, int def) :hp(hp), ack(ack), def(def) {}

    ~Memento() {
        std::cout << "~Memento()" << std::endl;
    }
};

class Role
{
private:
    int hp;
    int ack;
    int def;
public:
    Role(int hp, int ack, int def) :hp(hp), ack(ack), def(def) {}
    std::shared_ptr<Memento> save() {
        return std::make_shared<Memento>(hp, ack, def);
    }

    void recovery(std::shared_ptr<Memento> memento) {
        hp = memento->hp;
        ack = memento->ack;
        def = memento->def;
    }

    void fight() {
        hp -= 10;
        ack -= 5;
        def -= 2;
    }

    void show() {
        std::cout << "hp: " << hp << " ack: " << ack << " def: " << def << std::endl;
    }
};

class Caretaker
{
private:
    std::list<std::shared_ptr<Memento>> mementos;

public:
    void save(std::shared_ptr<Memento> memento) {
        mementos.push_back(memento);
    }

    std::shared_ptr<Memento> getMemento() {
        auto memento = mementos.back();
        mementos.pop_back();
        return memento;
    }
};

