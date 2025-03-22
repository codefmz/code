#include "pch.h"
#include "Util.h"
#include "Resource.h"
//#pragma warning(disable:4996);

#define LOGPATH R"(E:/log/server.log)"

char Util::RC_Server_MKLink_Exe_Path[MAX_PATH] = { 0 };
char Util::RC_Server_Exe_Path[MAX_PATH] = { 0 };

void Util::Dump(BYTE* pData, size_t nSize)
{
    std::string strOut;
    for (size_t i = 0; i < nSize; ++i)
    {
        char buf[8] = "";
        if (i > 0 && (i % 16 == 0))
        {
            strOut += "\n";
        }
        snprintf(buf, sizeof(buf), "%02X ", pData[i] & 0xFF);
        strOut += buf;
    }
    strOut += "\n";
    OutputDebugStringA(strOut.c_str());
}

void Util::ShowError()
{
    LPSTR lpMessageBuf = NULL;
    strerror(errno);//标准C语音库
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL,
        GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), lpMessageBuf, 0, NULL);
    OutputDebugString(lpMessageBuf);
    MessageBox(NULL, lpMessageBuf, _T("发生错误"), 0);
    LocalFree(lpMessageBuf);
}

bool Util::RegistryCheck()
{
    //检验一下 c 盘中link 文件是否存在
    char exePath[MAX_PATH]{ 0 };
    char value[MAX_PATH] = { 0 };
    DWORD valueSize = MAX_PATH * sizeof(BYTE);
    LONG ret;

    GetSystemDirectory(exePath, MAX_PATH);
    strncat_s(exePath, RC_SERVER_EXE_NAME, strlen(RC_SERVER_EXE_NAME) + 1);
    strcpy_s(Util::RC_Server_MKLink_Exe_Path, sizeof(Util::RC_Server_MKLink_Exe_Path), exePath);
    if (PathFileExists(exePath) == FALSE) {
        return false;
    }

    HKEY hKey = NULL;
    ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, RC_SERVER_REGISTER_KEY_PATH, 0, KEY_READ, &hKey);
    if (ret != ERROR_SUCCESS) {
        return false;
    }

    ret = RegQueryValueExA(hKey, RC_SERVER_REGISTER_KEY, NULL, NULL, (BYTE*)value, &valueSize);
    if (ret != ERROR_SUCCESS) {
        return false;
    }

    if (strcmp(value, exePath) != 0) {
        return false;
    }

    return true;
}

bool Util::AutoExeRegister()
{
    int ret;
    LONG lRet;
    HKEY hKey;
    string cmd;
    CString strInfo = _T("该程序只允许用于合法的用途！\n");

    strInfo += _T("继续运行该程序，将使得这台机器处于被监控状态！\n");
    strInfo += _T("如果你不希望这样，请按“取消”按钮，退出程序。\n");
    strInfo += _T("按下“是”按钮，该程序将被复制到你的机器上，并随系统启动而自动运行！\n");
    strInfo += _T("按下“否”按钮，程序只运行一次，不会在系统内留下任何东西！\n");
    ret = MessageBox(NULL, strInfo, _T("警告"), MB_YESNOCANCEL | MB_ICONWARNING | MB_TOPMOST);
    if (ret != IDYES) {
        return false;
    }

    lRet = RegCreateKeyExA(HKEY_LOCAL_MACHINE, RC_SERVER_REGISTER_KEY_PATH, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL);
    if (lRet != ERROR_SUCCESS) {
        return false;
    }

    lRet = RegSetValueExA(hKey, RC_SERVER_REGISTER_KEY, 0, REG_EXPAND_SZ, (BYTE*)(RC_Server_MKLink_Exe_Path),
        sizeof(RC_Server_MKLink_Exe_Path));
    if (lRet != ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return false;
    }

    RegCloseKey(hKey);
    GetModuleFileName(NULL, RC_Server_Exe_Path, MAX_PATH);

    cmd = "mklink " + string(RC_Server_MKLink_Exe_Path) + " " + string(RC_Server_Exe_Path);
    (void)system(cmd.c_str());
    return true;
}

bool Util::Init()
{
    bool ret;

    ret = Util::InitMFC();
    if (!ret) {
        return false;
    }

    //ret = Util::CheckExcuteLevel();
    //if (!ret) {
    //	LOG_INFO("evelate as admin.");
    //	Util::RunAsAdmin();
    //	return 0;
    //}

    //ret = Util::RegistryCheck();
    //if (!ret) {
    //	ret = Util::AutoExeRegister();
    //	if (!ret) {
    //		return false;
    //	}
    //}

    return ret;
}

bool Util::InitMFC()
{
    HMODULE hModule = ::GetModuleHandle(nullptr);
    if (hModule == nullptr) {
        return false;
    }
    //console application, which doesn't use MFC supplied WinMain Function, must call afxWinInit to initialize MFC
    if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0)) {
        return false;
    }
    return true;
}

bool Util::CheckExcuteLevel()
{
    HANDLE hToken = NULL;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        ShowError();
        return false;
    }
    TOKEN_ELEVATION eve;
    DWORD len = 0;
    if (GetTokenInformation(hToken, TokenElevation, &eve, sizeof(eve), &len) == FALSE) {
        ShowError();
        return false;
    }
    CloseHandle(hToken);
    if (len == sizeof(eve)) {
        return eve.TokenIsElevated; //判断是否已经提升来确定是否是用管理者权限启动
    }
    return false;
}

void Util::RunAsAdmin()
{
    STARTUPINFOW si = { 0 }; /* 新进程相关信息,下同 */
    PROCESS_INFORMATION pi = { 0 };
    WCHAR curExePath[MAX_PATH] = { 0 };
    BOOL ret;

    GetModuleFileNameW(NULL, curExePath, MAX_PATH);
    ret = CreateProcessWithLogonW(L"Administrator", NULL, L"", LOGON_WITH_PROFILE, NULL,
        curExePath, CREATE_UNICODE_ENVIRONMENT, NULL, NULL, &si, &pi);
    if (!ret)
    {
        ShowError();
        MessageBoxW(NULL, curExePath, L"创建进程失败", 0);
        return;
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

Log Log::log;
Log::Log() :outputFile(LOGPATH, std::ios::out | std::ios::trunc)
{
    if (outputFile.is_open()) {
        std::cout.rdbuf(outputFile.rdbuf());
    }
}

Log::~Log()
{
    outputFile.close();
}

void Log::strLog(LOG_LEVEL level, const char* file, const char* func, int line, const char* fmt, ...)
{
    char myLog[256] = { 0 };

    va_list pArgs;
    va_start(pArgs, fmt);
    vsnprintf(myLog, 256, fmt, pArgs);
    va_end(pArgs);

    cout << file << "(" << line << ") [" << func << "] [";
    switch (level)
    {
        case ERR:
            cout << "Error";
            break;
        case INFO:
            cout << "Info";
            break;
        default:
            break;
    }
    cout << "] : " << myLog << endl;
    outputFile.flush();
}

bool ipv4ToStr(DWORD ip, char* buf, uint32_t len)
{
    int ret = sprintf_s(buf, len, "%u.%u.%u.%u", ip >> 24, (ip & 0x00ff0000) >> 16, (ip & 0x0000ff00) >> 8, (ip & 0x000000ff));
    if (ret < 0) {
        return false;
    }
    return true;
}
