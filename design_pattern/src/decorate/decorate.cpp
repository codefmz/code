#include "decorate.h"
#include "iostream"
#include "gtest/gtest.h"
using namespace std;

class decorateTest : public ::testing::Test {
protected:
    decorateTest() {
    }
    ~decorateTest() override {
    }
    void SetUp() override {
        person = new Person("小明");
        shirt = new TShirt();
        nike = new NikeShoes();
        anta = new AntaShoes();
        adidas = new Adadas();
        jeans = new Jeans();
    }
    void TearDown() override {
        delete person;
        delete shirt;
        delete nike;
        delete anta;
        delete adidas;
        delete jeans;
    }

    Person *person;
    TShirt *shirt;
    Adadas *adidas;
    NikeShoes *nike;
    AntaShoes *anta;
    Jeans *jeans;
};

TEST_F(decorateTest, normal) {
    Person *shirtPerson = shirt->Decorate(person);
    Person *adidasShirtPerson = adidas->Decorate(shirtPerson);
    Person *nikeAdidasShirtPerson = nike->Decorate(adidasShirtPerson);
    nikeAdidasShirtPerson->show();
}

/* 可以选择随意拼接装饰类，解决类爆炸的问题 */
TEST_F(decorateTest, shoes) {
    Person *antaPerson = anta->Decorate(person);
    Person *jeansAntaPer = jeans->Decorate(antaPerson);
    Person *shirtJeansAntaPer = shirt->Decorate(jeansAntaPer);
    shirtJeansAntaPer->show();
}