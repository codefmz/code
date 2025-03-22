#pragma once

#include <list>
#include <string>

class Subject;
class Observer
{
protected:
    std::string name;
    std::shared_ptr<Subject> sub;
public:
    Observer(std::string name, std::shared_ptr<Subject>& sub) {
        this->name = name;
        this->sub = sub;
    }

public:
    virtual void update() = 0;
};

class Subject
{
protected:
    std::string state;
    //TODO: 可以不绑定成员，绑定函数对象，然后notify去调用函数对象即可
    //std::list<std::function<void()>> m_views;
    std::list<std::shared_ptr<Observer>> m_views;
public:
    void attach(std::shared_ptr<Observer> obs) {
        m_views.push_back(obs);
    }

    void detach(std::shared_ptr<Observer> obs) {
        m_views.remove(obs);
    }

    void notify() {
        for (auto it = m_views.begin(); it != m_views.end(); it++) {
            (*it)->update();
        }
    }

    void set_state(std::string state) {
        this->state = state;
    }

    std::string get_state() {
        return state;
    }
};

class StockObserver : public Observer
{
public:
    StockObserver(std::string name, std::shared_ptr<Subject>& sub) : Observer(name, sub) {}

public:
    void update() {
        std::cout << "StockObserver " << name << " update, state: " << sub->get_state() << std::endl;
    }
};

class NBAObserver : public Observer
{
public:
    NBAObserver(std::string name, std::shared_ptr<Subject>& sub) : Observer(name, sub) {}

    public:
    void update() {
        std::cout << "NBAObserver " << name << " update, state: " << sub->get_state() << std::endl;
    }
};