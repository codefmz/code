#include "out_include_pow.h"

int pow(int a, int b)
{
    if (b == 0) {
        return 1;
    }
    int result = pow(a, b / 2);
    if (b % 2 == 1) {
        return result * result * a;
    } else {
        return result * result;
    }
}