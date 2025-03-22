 #pragma once
 #include <iostream>

class AbstractTemplate
{
protected:
    void step1() {
        std::cout << "AbstractTemplate::step1" << std::endl;
    };
    void step3() {
        std::cout << "AbstractTemplate::step3" << std::endl;
    };
    void step5() {
        std::cout << "AbstractTemplate::step5" << std::endl;
    };
    virtual void step2() = 0;
    virtual void step4() = 0;
public:
    void run()
    {
        step1();
        step2();
        step3();
        step4();
        step5();
    }
    virtual ~AbstractTemplate() = default;
};


class Template : public AbstractTemplate
{
public:
    virtual void step2() override {
        std::cout << "Template::step2" << std::endl;
    };

    virtual void step4() override {
        std::cout << "Template::step4" << std::endl;
    };
};