#include "interpreter.h"
#include "gtest/gtest.h"

Expression* analyse(string expStr)
{
    stack<Expression*> expStack;
    Expression* left = nullptr;
    Expression* right = nullptr;
    for (string::size_type i = 0; i < expStr.size(); i++) {
        switch (expStr[i]) {
        case '+':
            left = expStack.top();
            right = new VarExpression(expStr[++i]);
            expStack.push(new AddExpression(left, right));
            break;
        case '-':
            left = expStack.top();
            right = new VarExpression(expStr[++i]);
            expStack.push(new SubExpression(left, right));
            break;
        default:
            expStack.push(new VarExpression(expStr[i]));
            break;
        }
    }
    Expression* expression = expStack.top();
    expStack.pop();
    release(expStack);
    return expression;
}

void release(stack<Expression*>& se)
{
    // for (auto &iter: se) {
    //     delete iter;
    // }
}

class interpreterTest : public ::testing::Test {
protected:
    interpreterTest() {
    }
    ~interpreterTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST_F(interpreterTest, test1)
{
    string expStr = "a+b-c+d";
    map<char, int> var;
    var.insert({ 'a',5 });
    var.insert({ 'b',2 });
    var.insert({ 'c',7 });
    var.insert({ 'd',9 });
    Expression* e = analyse(expStr);
    EXPECT_EQ(e->interpreter(var), 9);
    delete e;
}
