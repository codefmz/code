#include <string>
#include <algorithm>
#include <functional>
#include "gtest/gtest.h"

using namespace std;

class templateTest : public ::testing::Test {
protected:
    templateTest() {
    }
    ~templateTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }

    std::string str = "the more handsome";
};

/* 类型模板参数 */
template <typename T>
int compare(const T& a, const T& b)
{
    if (a < b) {
        return -1;
    } else if (a > b) {
        return 1;
    }

    return 0;
}

class TestA {
public:
    TestA(int num) : num(num) {
    }
    int operator<(const TestA& a) const {
        return num < a.num;
    }
    int operator>(const TestA& a) const {
        return num > a.num;
    }

private:
    int num;
};

TEST_F(templateTest, templateTest1) {
    EXPECT_EQ(compare(1, 2), -1);
    TestA a1(1), a2(2);
    EXPECT_EQ(compare(a1, a2), -1);
}

/* 非类型模板参数 */
template<int N, int M>
int compare(const char (&p1)[N], const char (&p2)[M])
{
    return strcmp(p1, p2);
}

