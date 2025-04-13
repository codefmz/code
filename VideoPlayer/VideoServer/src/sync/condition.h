#ifndef _CONDITION_H_
#define _CONDITION_H_
#include <pthread.h>

#include "mutex.h"

class Condition
{
public:
    static Condition* createNew();

    Condition();
    ~Condition();

    void wait(Mutex* mutex);
    bool waitTimeout(Mutex* mutex, int ms);
    void signal();
    void broadcast();

private:
    pthread_cond_t mCond;
};

#endif //_CONDITION_H_