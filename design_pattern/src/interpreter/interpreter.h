#pragma once
#include <iostream>
#include <map>
#include <string>
#include <stack>

using namespace std;

class Expression {
public:
    virtual ~Expression() = default;
    virtual int interpreter(map<char, int>& var) = 0;
};

class VarExpression : public Expression {
private:
	char key;
public:
    VarExpression(const char& key) {
        this->key = key;
    }
    int interpreter(map<char, int>& var) override {
        return var[key];
    }
};

class SymbolExpression : public Expression
{
protected:
    Expression* left;
    Expression* right;
public:
    SymbolExpression(Expression* _left, Expression* _right) :left(_left), right(_right) {};
    virtual ~SymbolExpression() {
        if (left) {
            delete left;
            left = nullptr;
        }
        if (right) {
            delete right;
            right = nullptr;
        }
    }
};

class AddExpression : public SymbolExpression
{
public:
    AddExpression(Expression* left, Expression* right) :SymbolExpression(left, right) {};
    // 通过 SymbolExpression 继承
    virtual int interpreter(map<char, int>& var) override {
        return left->interpreter(var) + right->interpreter(var);
    }
};

class SubExpression : public SymbolExpression
{
public:
    SubExpression(Expression* left, Expression* right) :SymbolExpression(left, right) {};
    virtual int interpreter(map<char, int>& var) override {
        return left->interpreter(var) - right->interpreter(var);
    }
};

Expression* analyse(string expStr);
void release(stack<Expression*>& se);

void testInterpreter();