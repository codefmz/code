#include <gtest/gtest.h>

int g_argc =0;
char **g_argv = nullptr;

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    g_argc = argc;
    g_argv = argv;
    return RUN_ALL_TESTS();
}