#pragma once
#include <string>
#include <iostream>

class CashSuper {
public:
    virtual ~CashSuper() = default;
    virtual double calcuate(double money) = 0;
};

class CashNormal : public CashSuper {
public:
    double calcuate(double money) override {
        return money;
    }
};

class CashRebate : public CashSuper {
private:
    double rebate;

public:
    CashRebate(double rebate) : rebate(rebate) {
    }

    double calcuate(double money) override {
        return money * rebate;
    }
};

class CashReturn : public CashSuper {
private:
    double moneyCondition;
    double moneyReturn;

public:
    CashReturn(double moneyCondition, double moneyReturn)
        : moneyCondition(moneyCondition), moneyReturn(moneyReturn) {
    }

    double calcuate(double money) override {
        if (money >= moneyCondition) {
            return money - (int)(money / moneyCondition) * moneyReturn;
        }

        return money;
    }
};

typedef enum {
    CASH_RETURN,
    CASH_REBATE,
    CASH_NORMAL,
    STRATEGY_BUTT
} StrategyType;

class CashContent {
private:
    CashSuper *cashSuper;

public:
    CashContent(StrategyType type) {
        switch (type) {
            case CASH_RETURN:
                cashSuper = new CashReturn(300, 100);
                break;
            case CASH_REBATE:
                cashSuper = new CashRebate(0.8);
                break;
            case CASH_NORMAL:
                cashSuper = new CashNormal();
                break;
            default:
                std::cout << "error type : " << type << std::endl;
                break;
        }
    }

    double getResult(double money) {
        return cashSuper->calcuate(money);
    }
};