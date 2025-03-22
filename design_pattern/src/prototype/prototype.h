#ifndef SIMPLE_FACTORY
#define SIMPLE_FACTORY

#include <memory>

class Shape {
public:
    virtual std::shared_ptr<Shape> clone() = 0;

    virtual void draw() = 0;
    virtual ~Shape() {
    }
};

class Circle : public Shape {
public:
    std::shared_ptr<Shape> clone() override {
        return std::make_shared<Circle>(*this);
    }

    void draw() override {
        std::cout << "Circle::draw()" << std::endl;
    }
};

class Square : public Shape {
public:
    std::shared_ptr<Shape> clone() override {
        return std::make_shared<Square>(*this);
    }

    void draw() override {
        std::cout << "Square::draw()" << std::endl;
    }
};

#endif /* SIMPLE_FACTORY */