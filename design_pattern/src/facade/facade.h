 #pragma once
 #include <iostream>

class SubSystemOne
{
public:
    void MethodOne() {
        std::cout << "SubSystemOne MethodOne" << std::endl;
    }
};


class SubSystemTwo
{
public:
    void MethodTwo() {
        std::cout << "SubSystemTwo MethodTwo" << std::endl;
    }
};

class SubSystemThree
{
public:
    void MethodThree() {
        std::cout << "SubSystemThree MethodThree" << std::endl;
    }
};


class Facade
{
private:
    SubSystemOne *m_subSystemOne;
    SubSystemTwo *m_subSystemTwo;
    SubSystemThree *m_subSystemThree;
public:
    Facade() {
        m_subSystemOne = new SubSystemOne();
        m_subSystemTwo = new SubSystemTwo();
        m_subSystemThree = new SubSystemThree();
    }

    ~Facade() {
        delete m_subSystemOne;
        delete m_subSystemTwo;
        delete m_subSystemThree;
    }

   void MethodA() {
        std::cout << "Facade MethodA" << std::endl;
        m_subSystemOne->MethodOne();
        m_subSystemTwo->MethodTwo();
        m_subSystemThree->MethodThree();
   }

   void MethodB() {
        std::cout << "Facade MethodB" << std::endl;
        m_subSystemOne->MethodOne();
        m_subSystemTwo->MethodTwo();
   }
};