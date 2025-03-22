#ifndef _UTIL_H_
#define _UTIL_H_

#include <iostream>
#include <fstream> // 包含文件流头文件
#include <string>

using std::cout;
using std::endl;
using std::string;

#define RC_FILE_NAME_MAX_SIZE 256

typedef struct file_info
{
    file_info() {
        isDir = false;
        hasNext = false;
        memset(name, 0, RC_FILE_NAME_MAX_SIZE);
    }

    bool isDir;
    bool hasNext;
    char name[RC_FILE_NAME_MAX_SIZE];
}FILE_INFO;

typedef struct MOUSEEV
{
    MOUSEEV()
    {
        nAction = 0;
        nButton = -1;
        ptxy.x = 0;
        ptxy.y = 0;
    }
    WORD nAction; //点击、移动、双击
    WORD nButton; //左键、右键、中键
    POINT ptxy;  //坐标
} *PMOUSEEV;


class Util
{
public:
    static void Dump(BYTE* pData, size_t nSize);
    /**
    * @brief  :获取最近一次的错误，使用MessageBox显示出来
    **/
    static void ShowError();
    /*
    * @brief : 检查注册表信息
    */
    static bool RegistryCheck();
    /*
    * @brief : 向注册表注册自启动项
    */
    static bool AutoExeRegister();

    static bool Init();
    /*
    * @brief : 初始化MFC
    */
    static bool InitMFC();

    /**
    * @brief :检查程序运行级别，判断是否是以管理者权限启动
    **/
    static bool CheckExcuteLevel();

    static void RunAsAdmin();

    static char RC_Server_Exe_Path[MAX_PATH];
    static char RC_Server_MKLink_Exe_Path[MAX_PATH]; /* 服务端的软连接地址 */
};

#define RC_SERVER_REGISTER_KEY_PATH "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"
#define RC_SERVER_REGISTER_KEY "RemoteCtrlServer"
#define RC_SERVER_EXE_NAME "\\RemoteCtrlServer.exe"
#define RC_ERROR (~0)
#define RC_OK 0

typedef enum {
    INFO,
    ERR
} LOG_LEVEL;

class Log
{
private:
    std::ofstream outputFile;
private:
    Log();
    ~Log();
public:
    static Log log;
    void strLog(LOG_LEVEL level, const char* file, const char* func, int line, const char* str, ...);
};

#define LOG(level, str, ...) \
    do { \
        Log::log.strLog((level), __FILE__, __func__, __LINE__, (str), ##__VA_ARGS__); \
    } while (0); \

bool ipv4ToStr(DWORD ip, char* buf, uint32_t len);

#endif
