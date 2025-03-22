#ifndef _MY_LOG_H_
#define _MY_LOG_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    INFO_LEVEL = 0,
    ERROR_LEVEL
} LOG_LEVEL;

typedef void (*LOG_FUNC)(int32_t level, int32_t line, const char* fun, const char* file, const char* fmt, ...);

void Log2File(int32_t level, int32_t line, const char* fun, const char* file, const char* fmt, ...);

void Log2Control(int32_t level, int32_t line, const char* fun, const char* file, const char* fmt, ...);

void SetLogFunc(LOG_FUNC logFunc);

/* 输出到自定义文件夹，需要搭配 Log2File 使用 */
void SetLogDir(const char* dir);

extern LOG_FUNC g_logFunc;

#define LOG_INFO(fmt, ...)                                                           \
    do {                                                                             \
        g_logFunc(INFO_LEVEL, __LINE__, __FUNCTION__, __FILE__, fmt, ##__VA_ARGS__); \
    } while (0)

#define LOG_ERR(fmt, ...)                                                             \
    do {                                                                              \
        g_logFunc(ERROR_LEVEL, __LINE__, __FUNCTION__, __FILE__, fmt, ##__VA_ARGS__); \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif // _MY_LOG_H_