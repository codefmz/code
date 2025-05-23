#ifndef _SEM_H_
#define _SEM_H_
#include <semaphore.h>

class Sem
{
public:
    static Sem* createNew(int val);

    Sem(int val);
    ~Sem();

    void post();
    void wait();

private:
    sem_t mSem;
};

#endif //_SEM_H_