#ifndef _RC_CONTROL_H_
#define _RC_CONTROL_H_

#include <mutex>
#include "RCClientDlg.h"
#include "Packet.h"

class Client;

class RCControl
{
private:
    std::mutex mtx;         /* 保护收包buf， 后续考虑使用免锁队列实现 */
    std::string recvBuf;    /* 收包buf */
    HANDLE recvSem;         /* 收包线程信号量, 后续使用免锁队列可以删除 */
    HANDLE recvThd;         /* 收包线程句柄 */
    HANDLE watchThd;        /* 显示线程句柄 */
    Client* client;         /* 客户端网络层 */
    bool isRecving;         /* 是否继续收包 */

public:
    static RCControl* instance(void);

private:
    RCControl(void);
    ~RCControl(void);

public:
    static void RecvData(char* data, uint32_t len) {
        RCControl::instance()->RecvDataInner(data, len);
    }
    int StartRecv(void);
    int StopRecv(void);
    bool StartConnect();
    bool SendPacket(Packet* packet);
    void UpdateAddress(DWORD nIP, WORD nPort);
private:
    static unsigned RecvEntry(void* arg);
    void RecvDataInner(char* data, uint32_t len);
    void ProcessData(void);
};
#endif /* _RC_CONTROL_H_ */
