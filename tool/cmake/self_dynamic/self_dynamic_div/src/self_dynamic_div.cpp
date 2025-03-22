#include "self_dynamic_div.h"
#include "dynamic_comm.h"
#include <stdint.h>
#include <stdio.h>
int self_div(int a, int b)
{
    if (!isValid(b)) {
        printf("%d is not a num!\r\n", b);
        return INT32_MAX;
    }
    return a / b;
}
