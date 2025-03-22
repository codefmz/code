#include <fstream>
#include <string>
#include "gtest/gtest.h"

using namespace std;

class pointerTest : public ::testing::Test {
protected:
    pointerTest() {
    }
    ~pointerTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

class TestPointer
{
public:
    TestPointer() {
        cout << "TestPointer()" << endl;
    }

    ~TestPointer() {
        cout << "~TestPointer()" << endl;
    }
};

TEST_F(pointerTest, pointerTest1) {
    shared_ptr<int> p1 = make_shared<int>(1);
    shared_ptr<string> p2 = make_shared<string>(3, 'c');
    shared_ptr<TestPointer> p3 = make_shared<TestPointer>();

    EXPECT_EQ(*p2, "ccc");
    auto p4 = p1;
    EXPECT_TRUE(p2.unique());
    EXPECT_EQ(p1.use_count(), 2);
    EXPECT_EQ(p4.use_count(), 2);
}

class TestB;

class TestA
{
public:
    TestA() {
        cout << "TestA()" << endl;
    };

    ~TestA() {
        cout << "~TestA()" << endl;
    }

public:
    weak_ptr<TestB> pb;
};

class TestB
{
public:
    TestB() {
        cout << "TestB()" << endl;
    };

    ~TestB() {
        cout << "~TestB()" << endl;
    }

public:
    weak_ptr<TestA> pa;
};

TEST_F(pointerTest, pointerTest2) {
    shared_ptr<TestA> pa = make_shared<TestA>();
    shared_ptr<TestB> pb = make_shared<TestB>();
    unique_ptr<TestA> pa2 = make_unique<TestA>();
    pa->pb = pb;
    pb->pa = pa;

    EXPECT_EQ(pa.use_count(), 1);
    EXPECT_EQ(pb.use_count(), 1);
}

TEST_F(pointerTest, swapTest) {
    int a = 10, b = 20;

    int *p1 = &a;
    int *p2 = &b;

    swap(p1, p2);
    EXPECT_EQ(*p1, 20);
    EXPECT_EQ(*p2, 10);
}

class TESTD {
public:
    unique_ptr<TestA[]> pa;

    TESTD() : pa(make_unique<TestA[]>(4)) {
    }
};

TEST_F(pointerTest, unique_ptr_test) {
    TESTD td;
}
