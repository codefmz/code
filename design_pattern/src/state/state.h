#ifndef __STATE_H
#define __STATE_H

#include <iostream>
#include <memory>

class State;

class Work {
private:
    std::shared_ptr<State> state;
public:
    bool finished;
    int hour;
public:
    Work();

    void writeProgram();
    void setState(std::shared_ptr<State> state) {
        this->state = state;
    }
};

class State {
public:
    virtual ~State() = default;
    virtual void writeProgram(Work *work) = 0;
};

class RestState : public State {
public:
    void writeProgram(Work *work) override {
        std::cout << "当前时间：" << work->hour << " 点，下班回家 " << std::endl;
    }
};

class EveningState : public State {
public:
    void writeProgram(Work *work) override {
        if (work->finished) {
            work->setState(std::make_shared<RestState>());
            work->writeProgram();
            return;
        }

        if (work->hour < 21) {
            std::cout << "当前时间：" << work->hour << "点，加班中" << std::endl;
        } else {
            work->setState(std::make_shared<RestState>());
            work->writeProgram();
        }
    }
};

class AfternoonState : public State {
public:
    void writeProgram(Work *work) override {
        if (work->hour < 17) {
            std::cout << "当前时间：" << work->hour << "点，下午状态还不错，继续努力" << std::endl;
        } else {
            work->setState(std::make_shared<EveningState>());
            work->writeProgram();
        }
    }
};

class NoonState : public State {
public:
    void writeProgram(Work *work) override {
        if (work->hour < 13) {
            std::cout << "当前时间：" << work->hour << "点，饿了，午饭，犯困，午休" << std::endl;
        } else {
            work->setState(std::make_shared<AfternoonState>());
            work->writeProgram();
        }
    }
};

class ForenoonState : public State {
public:
    void writeProgram(Work *work) override {
        if (work->hour < 12) {
            std::cout << "当前时间：" << work->hour << "点，上午工作，精神百倍" << std::endl;
        } else {
            work->setState(std::make_shared<NoonState>());
            work->writeProgram();
        }
    }

    virtual ~ForenoonState() {
        std::cout << "~ForenoonState()" << std::endl;
    }
};

#endif /* __STATE_H */