#include "config_my89757.h"
#include "out_include_pow.h"
#include "self_dynamic_div.h"
#include "self_dynamic_mul.h"
#include "self_static_add.h"
#include "self_static_sub.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv)
{
    if (argc != 3) {
        return 0;
    }
    printf("%s + %s = %d\r\n", argv[1], argv[2], self_add(atoi(argv[1]), atoi(argv[2])));
    printf("%s - %s = %d\r\n", argv[1], argv[2], self_sub(atoi(argv[1]), atoi(argv[2])));
    printf("%s * %s = %d\r\n", argv[1], argv[2], self_mul(atoi(argv[1]), atoi(argv[2])));
    printf("%s / %s = %d\r\n", argv[1], argv[2], self_div(atoi(argv[1]), atoi(argv[2])));
    printf("%s ^ %s = %d\r\n", argv[1], argv[2], pow(atoi(argv[1]), atoi(argv[2])));
    printf("TEST_VARIABLE_NAME = %s\r\n", TEST_VARIABLE_NAME);
#ifdef USE_MY_STATIC_OUT
    printf("defined USE_MY_STATIC_OUT\r\n");
#endif

    printf("Dir = %s", FILE_DIR);
    return 1;
}