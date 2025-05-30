#include "log.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LOG_NAME "logger"
#define LOG_DIR_MAX_SIZE 128

#ifdef __cplusplus
extern "C" {
#endif

LOG_FUNC g_logFunc = Log2File;

const char* g_levelStr[2] = { "INFO", "ERROR" };

char g_logDir[LOG_DIR_MAX_SIZE] = { 0 };

/* 获取年月日，时分秒 */
char* GetCurTimeStr(void)
{
    time_t mytime = time(NULL);
    static char s[50];
    struct tm* curr = localtime(&mytime);

    if (curr->tm_year > 50) {
        sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d", curr->tm_year + 1900, curr->tm_mon + 1, curr->tm_mday,
            curr->tm_hour, curr->tm_min, curr->tm_sec);
    } else {
        sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d", curr->tm_year + 2000, curr->tm_mon + 1, curr->tm_mday,
            curr->tm_hour, curr->tm_min, curr->tm_sec);
    }
    return s;
}

/* 获取年月日 */
char* GetCurDateStr(void)
{
    time_t mytime = time(NULL);
    static char s[50];

    struct tm* curr = localtime(&mytime);
    if (curr->tm_year > 50) {
        sprintf(s, "%04d-%02d-%02d", curr->tm_year + 1900, curr->tm_mon + 1, curr->tm_mday);
    } else {
        sprintf(s, "%04d-%02d-%02d", curr->tm_year + 2000, curr->tm_mon + 1, curr->tm_mday);
    }
    return s;
}

void Log2File(int32_t level, int32_t line, const char* fun, const char* file, const char* fmt, ...)
{
    char logFileName[128] = { 0 };
    char log[256] = { 0 };

    va_list pArgs;
    va_start(pArgs, fmt);
    vsnprintf(log, 256, fmt, pArgs);
    va_end(pArgs);

    snprintf(logFileName, 128, "%s/%s.log", g_logDir, GetCurDateStr());
    FILE* pFile = fopen(logFileName, "a+");
    if (pFile != NULL) {
        fprintf(pFile, "[%s][%s]%s:%d[%s]:%s\n", GetCurTimeStr(), g_levelStr[level], file, line, fun, log);
        fflush(pFile);
        fclose(pFile);
    }
}

void Log2Control(int32_t level, int32_t line, const char* fun, const char* file, const char* fmt, ...)
{
    char log[256] = { 0 };

    va_list pArgs;
    va_start(pArgs, fmt);
    vsnprintf(log, 256, fmt, pArgs);
    va_end(pArgs);

    fprintf(stdout, "[%s][%s]%s:%d[%s]:%s\n", GetCurTimeStr(), g_levelStr[level], file, line, fun, log);
    fflush(stdout);
}

void SetLogFunc(LOG_FUNC logFunc)
{
    g_logFunc = logFunc;
}

void SetLogDir(const char* dir)
{
    strncpy_s(g_logDir, LOG_DIR_MAX_SIZE - 1, dir, LOG_DIR_MAX_SIZE - 1);
}

#ifdef __cplusplus
}
#endif
