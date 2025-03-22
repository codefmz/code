#include "pch.h"
#include "Packet.h"

//收包解析数据包
Packet::Packet(const char* data, size_t size, bool& isCreated)
{
    isCreated = false;
    if (size < PACKET_MIN_LEN) {
        return;
    }

    if (*(uint16_t*)(data) != PACKET_MAGIC) {
        return;
    }
    memcpy(this, data, PACKET_MIN_LEN);
    isCreated = true;
    if (length == 0) {
        return;
    }

    this->data = new char[length];
    memcpy(this->data, data + PACKET_MIN_LEN, length);
}

//发送消息时构建的包
Packet::Packet(RC_CMD cmd, char* data, uint32_t length, uint64_t hwnd, uint64_t param)
{
    magic = PACKET_MAGIC;
    this->cmd = cmd;
    this->hwnd = hwnd;
    this->param = param;
    this->length = length;
    if (length == 0) {
        return;
    }
    this->data = new char[length];
    memcpy(this->data, data, length);
}

Packet::Packet(Packet& inPacket, char* data, size_t length)
{
    this->magic = inPacket.magic;
    this->cmd = inPacket.cmd;
    this->hwnd = inPacket.hwnd;
    this->param = inPacket.param;
    this->length = length;
    if (length == 0) {
        return;
    }
    this->data = new char[length];
    memcpy(this->data, data, length);
}

void Packet::Data(char* pData)
{
    memcpy(pData, this, PACKET_MIN_LEN);
    if (length > 0) {
        memcpy(pData + PACKET_MIN_LEN, this->data, length);
    }
}

uint32_t Packet::Size(void)
{
    return length + PACKET_MIN_LEN;
}
