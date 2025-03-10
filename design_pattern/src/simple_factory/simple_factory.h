#ifndef SIMPLE_FACTORY
#define SIMPLE_FACTORY

class Operator {
public:
    virtual int calculate(int num1, int num2) = 0;
    virtual ~Operator() {
    }
};

class AddOper : public Operator {
public:
    int calculate(int num1, int num2) {
        return num1 + num2;
    }
};

class SubOper : public Operator {
public:
    int calculate(int num1, int num2) {
        return num1 - num2;
    }
};

class MultiOper : public Operator {
public:
    int calculate(int num1, int num2) {
        return num1 * num2;
    }
};

class DivOper : public Operator {
public:
    int calculate(int num1, int num2) {
        return num1 / num2;
    }
};

class ModOper : public Operator {
public:
    int calculate(int num1, int num2) {
        return num1 % num2;
    }
};

class OperFactory {
public:
    static Operator *CreateOper(char symbol) {
        Operator *oper = nullptr;
        switch (symbol) {
            case '+':
                oper = new AddOper;
                break;
            case '-':
                oper = new SubOper;
                break;
            case '*':
                oper = new MultiOper;
                break;
            case '/':
                oper = new DivOper;
                break;
            case '%':
                oper = new ModOper;
                break;
            default:
                break;
        }
        return oper;
    }
};

#endif /* SIMPLE_FACTORY */