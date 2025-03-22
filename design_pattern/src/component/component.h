#ifndef COMPONENT_H
#define COMPONENT_H

#include <iostream>
#include <string>
#include <list>

class Company {
public:
    Company(std::string name) :name(name) {}
    virtual void add(Company* c) = 0;
    virtual void remove(Company* c) = 0;
    virtual void display(int depth) = 0;
    virtual void lineOfDuty() = 0;
protected:
    std::string name;
};

class ConcreteCompany : public Company {
private:
    std::list<Company*> children;

public:
    ConcreteCompany(std::string name) :Company(name) {}
    virtual void add(Company* c) {
        children.push_back(c);
    }
    virtual void remove(Company* c) {
        children.remove(c);
    }
    virtual void display(int depth) {
        std::cout << std::string(depth, '-') << name << std::endl;
        for (auto &c : children) {
            c->display(depth + 2);
        }
    }

    virtual void lineOfDuty() override{
        for (auto &c : children) {
            c->lineOfDuty();
        }
    }
};

class HRDepartment : public Company {
public:
    HRDepartment(std::string name) :Company(name) {}
    virtual void add(Company* c) override {
    }

    virtual void remove(Company* c) override {
    }

    virtual void display(int depth) override {
        std::cout << std::string(depth, '-') << name << std::endl;
    }

    virtual void lineOfDuty() override {
        std::cout << name << ": 招聘员工" << std::endl;
    }
};

class FinanceDepartment : public Company {
public:
    FinanceDepartment(std::string name) :Company(name) {}

    virtual void add(Company* c) override {
    }

    virtual void remove(Company* c) override {
    }

    virtual void display(int depth) override {
        std::cout << std::string(depth, '-') << name << std::endl;
    }
    virtual void lineOfDuty() override {
        std::cout << name << ": 公司财务管理" <<std::endl;
    }
};

#endif
