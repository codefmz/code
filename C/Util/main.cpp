#include "log.h"
#include <string>

void main(int argc, char** argv)
{
    // SetLogFunc(Log2File);
    // SetLogDir(R"(E:/Code/C/Util/bin/Debug)");
    LOG_INFO("exe filePath = %s", argv[0]);
    LOG_ERR("hello %d", 2024);
    LOG_ERR("hello %d", 2024);
    LOG_ERR("hello %d", 2024);
    LOG_ERR("hello %d", 2024);
    LOG_INFO("info log %d, %s", 2024, "haha");
}