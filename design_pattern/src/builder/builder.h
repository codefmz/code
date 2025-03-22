 #pragma once
 #include <iostream>
 #include <memory>

class PersonBuilder
{
public:
    virtual void buildHead() = 0;
    virtual void buildHand() = 0;
    virtual void buildBody() = 0;
    virtual void buildLeg() = 0;

public:
    virtual ~PersonBuilder() {
        std::cout << "~PersonBuilder" << std::endl;
    };
};

class ThinPersonBuilder : public PersonBuilder
{
public:
    virtual void buildHead() {
        std::cout << "thin head" << std::endl;
    }
    virtual void buildHand() {
        std::cout << "thin hand" << std::endl;
    }
    virtual void buildBody() {
        std::cout << "thin body" << std::endl;
    }
    virtual void buildLeg() {
        std::cout << "thin leg" << std::endl;
    }
};

class FatPersonBuilder : public PersonBuilder {
public:
    virtual void buildHead() override {
        std::cout << "big head" << std::endl;
    }
    virtual void buildHand() override {
        std::cout << "big hand" << std::endl;
    }
    virtual void buildBody() override {
        std::cout << "big body" << std::endl;
    }
    virtual void buildLeg() override {
        std::cout << "big leg" << std::endl;
   }
};

class Director {
private:
    std::shared_ptr<PersonBuilder> builder;
public:
    Director() = default;
    void setBuilder(std::shared_ptr<PersonBuilder> &builder) {
        this->builder = builder;
    }

    void construct() {
        builder->buildHead();
        builder->buildHand();
        builder->buildBody();
        builder->buildLeg();
    }
};