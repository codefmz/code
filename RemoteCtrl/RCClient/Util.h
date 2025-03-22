#ifndef _UTIL_H_
#define _UTIL_H_
#include <iostream>
#include <direct.h>
#include <fstream> // 包含文件流头文件
using std::cout;
using std::endl;

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

namespace Util
{
    int Bytes2Image(CImage& image, const std::string& strBuffer);

    void Dump(BYTE* pData, size_t nSize);
}

bool ipv4ToStr(DWORD ip, char* buf, uint32_t len);

#endif /* _UTIL_H_ */
