#ifndef _PACKET_H_
#define _PACKET_H_

#include <string>
#include "RCBase.h"

#define PACKET_MAGIC 0x17 /* 包魔术字 */
#define PACKET_MIN_LEN (sizeof(Packet) - sizeof(char*))

class Packet
{
public:
    Packet() :magic(PACKET_MAGIC), cmd(RC_UINT16_INVALID_VAL), length(RC_UINT32_INVALID_VAL), data(NULL)
    {
    }
    ~Packet()
    {
        if (data != NULL) {
            delete[] data;
            data = NULL;
        }
        magic = 0;
        cmd = RC_UINT16_INVALID_VAL;
        length = RC_UINT32_INVALID_VAL;
        hwnd = 0;
        param = 0;
    }
    Packet(const char* data, size_t size, bool& isCreated);
    Packet(RC_CMD cmd, char* data, uint32_t length, uint64_t hwnd = 0, uint64_t param = 0);
    Packet(Packet& inPacket, char* data, size_t length);
    void Data(char* pData);
    uint32_t Size();
public:
    uint16_t magic;     /* 魔术字*/
    uint16_t cmd;       /* 命令 */
    uint32_t length;    /* 内容长度 */
    uint64_t hwnd;      /* 窗口句柄 */
    uint64_t param;     /* 参数 */
    char* data;         /* 记录收到与发送的数据 */
};

#endif // !_PACKET_H_
