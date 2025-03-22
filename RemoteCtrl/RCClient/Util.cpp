#include "pch.h"
#include "Util.h"
#include "stdio.h"
#include "io.h"
#include "Packet.h"
#include "Resource.h"
#include <atlimage.h>
#define LOGPATH R"(E:/log/client.log)" 

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

void Log::strLog(int level, const char* file, const char* func, int line, const char* fmt, ...)
{
    char myLog[256] = { 0 };

    va_list pArgs;
    va_start(pArgs, fmt);
    vsnprintf(myLog, 256, fmt, pArgs);
    va_end(pArgs);

    cout << file << "(" << line << ") [" << func << "] [";
    switch (level) {
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

int Util::Bytes2Image(CImage& image, const std::string& strBuffer)
{
    BYTE* data = (BYTE*)(strBuffer.c_str());
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, 0); //分配全局内存
    if (hMem == NULL) {
        LOG(ERR, "Global alloc fail.");
        return -1;
    }
    IStream* pStream = NULL;
    HRESULT result = CreateStreamOnHGlobal(hMem, TRUE, &pStream);
    if (result != S_OK) {
        LOG(ERR, "Create global stream fail.");
        return -1;
    }
    ULONG len = 0;
    pStream->Write(data, strBuffer.size(), &len);
    LARGE_INTEGER bg = { 0 };
    pStream->Seek(bg, STREAM_SEEK_SET, NULL);
    if (!image.IsNull()) {
        image.Destroy();
    }
    image.Load(pStream);
    pStream->Release();
    GlobalFree(hMem);
    return 0;
}

bool ipv4ToStr(DWORD ip, char* buf, uint32_t len)
{
    int ret = sprintf_s(buf, len, "%u.%u.%u.%u", ip >> 24, (ip & 0x00ff0000) >> 16, (ip & 0x0000ff00) >> 8, (ip & 0x000000ff));
    if (ret < 0) {
        return false;
    }
    return true;
}
