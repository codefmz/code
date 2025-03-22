#ifndef _LOG_H_
#define _LOG_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum logLevel {
    INFO_LEVEL = 0,
    LOG_ERROR = 1,
} LOG_LEVEL;

typedef void (*LogImp)(LOG_LEVEL type, int32_t line, const char* fun, const char* file, char* fmt, ...);

void Log2File(LOG_LEVEL level, int32_t line, const char* fun, const char* file, char* fmt, ...);

void Log2Control(LOG_LEVEL level, int32_t line, const char* fun, const char* file, char* fmt, ...);

extern LogImp g_logImp;

#define LOG_INFO(fmt, ...)                                                          \
    {                                                                               \
        g_logImp(INFO_LEVEL, __LINE__, __FUNCTION__, __FILE__, fmt, ##__VA_ARGS__); \
    }

#define LOG_ERROR(fmt, ...)                                                        \
    {                                                                              \
        g_logImp(LOG_ERROR, __LINE__, __FUNCTION__, __FILE__, fmt, ##__VA_ARGS__); \
    }

#ifdef __cplusplus
}
#endif

#endif // _LOG_H_