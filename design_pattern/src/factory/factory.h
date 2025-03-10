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

class IFactory{
public:
    virtual Operator *createOper() = 0;
};

class AddFactory : public IFactory {
public:
    Operator *createOper() {
        return new AddOper();
    }
};

class SubFactory : public IFactory {
public:
    Operator *createOper() {
        return new SubOper();
    }
};

class MultiFactory : public IFactory {
public:
    Operator *createOper() {
        return new MultiOper();
    }
};

class DivFactory : public IFactory {
public:
    Operator *createOper() {
        return new DivOper();
    }
};

#endif /* SIMPLE_FACTORY */