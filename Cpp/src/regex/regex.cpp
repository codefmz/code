#include <fstream>
#include <string>
#include <regex>
#include "gtest/gtest.h"

using namespace std;

class regexTest : public ::testing::Test {
protected:
    regexTest() {
    }
    ~regexTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST_F(regexTest, regexTest1)
{
    string pattern("[^c]ei");
    pattern = "[[:alpha:]]*" + pattern + "[[:alpha:]]*";
    regex r(pattern);
    smatch results;
    string test_str = "receipt freind theif receive";
    if (regex_search(test_str, results, r)) {
        cout << results.str() << endl;
    }
}

TEST_F(regexTest, regexTest2)
{
    string pattern("[^c]ei");
    pattern = "[[:alpha:]]*" + pattern + "[[:alpha:]]*";
    regex r(pattern);
    string file = "receipt freind theif receive";
    for (sregex_iterator it(file.begin(), file.end(), r), end_it; it != end_it; ++it) {
        cout << it->str() << endl;
    }
}

#define TEST_PATH R"(E:\Code\Cpp\cppcore\src\regex\test.gcode)"

TEST_F(regexTest, regexTest3)
{
    string pattern("[^c]ei");
    regex r(pattern);
    string file = "receipt freind theif receive";
    for (sregex_iterator it(file.begin(), file.end(), r), end_it; it != end_it; ++it) {
        cout << it->str() << endl;
    }
}