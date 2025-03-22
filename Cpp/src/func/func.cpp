#include <fstream>
#include <string>
#include <algorithm>
#include <functional>
#include "gtest/gtest.h"

using namespace std;

class funcTest : public ::testing::Test {
protected:
    funcTest() {
    }
    ~funcTest() override {
    }
    void SetUp() override {
        words = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j",
            "k", "l", "me444", "n", "o", "p", "q", "r", "s", "t", "u",
            "v", "w", "x", "y", "z" };
    }
    void TearDown() override {
    }

    vector<string> words;
};

bool check_size(const string &s, size_t sz)
{
    return s.size() >= sz;
}

TEST_F(funcTest, funcTest1) {
    size_t sz = 4;
    auto wc = count_if(words.begin(), words.end(), [=](const string &s)-> bool {
        return s.size() >= sz;
    });
    cout << wc << endl;
    EXPECT_EQ(wc, 1);
}

TEST_F(funcTest, funcTest2) {
    size_t sz = 4;
    auto wc = count_if(words.begin(), words.end(), bind(check_size, placeholders::_1, sz));
    cout << wc << endl;
    EXPECT_EQ(wc, 1);
}

class MyClass {
public:
    void myMemberFunction(int x) {
        std::cout << "myMemberFunction called with value: " << x << std::endl;
    }
};

TEST_F(funcTest, funcTest3) {
    MyClass obj;
    // 使用 std::function 来包装普通成员函数
    std::function<void(int)> func = std::bind(&MyClass::myMemberFunction, &obj, std::placeholders::_1);

    // 调用成员函数
    func(42);  // 输出: myMemberFunction called with value: 42
    std::string s(3, 'S');
    std::cout << s << std::endl;
}

void testFunc() {
    std::cout << "testFunc()!" << std::endl;
}

void testFunc(int a, int b) {
    std::cout << "testFunc(int a)!" << std::endl;
}

void testFunc(int a, int b, char c = 10) {
    std::cout << "testFunc(int a)!" << std::endl;
}

TEST_F(funcTest, funcTest4) {
    testFunc(2, 4); //不能使用
}