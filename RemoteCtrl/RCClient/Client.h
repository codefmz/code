#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <winsock2.h>
#include <string>
#include "RCBase.h"

typedef void (*HANDLE_RECV_DATA_FUNC)(char* byte, uint32_t len);

//The IANA suggests 49152 to 65535 as "dynamic and/or private ports".
const short PORT_START = 51653;

class Client
{
private:
    RC_SOCKET_STATE state;
    uint32_t ip;
    uint16_t port;
    char* recvBuf;
    SOCKET socket;
    char sendBuf[RC_SEND_BUF_SIZE];
    HANDLE_RECV_DATA_FUNC handleDataFunc; /* 收包回调函数 */

public:
    Client();
    ~Client();
    bool Create(void);
    void Destroy(void);

private:
    static void WINAPI OnIOCompletion(DWORD errCode, DWORD transNum, LPOVERLAPPED pOverLapped);
    void OnConnect();
    void OnRecv(DWORD transNum);
    void OnSend(DWORD transNum);
    void OnClose();
    void ProcessPacket(DWORD receivedLen);
public:
    bool PostConnect(void);
    bool PostReceive(void);
    bool PostSend(const char* buffer, uint32_t size);
    bool Shutdown();

    RC_SOCKET_STATE GetState() {
        return state;
    }

    void UpdateAddress(DWORD nIP, WORD nPort) {
        this->ip = nIP;
        this->port = nPort;
    }

    SOCKET GetSocket() {
        return socket;
    }

    void SetHandleRecvDataFunc(HANDLE_RECV_DATA_FUNC nHandleRecvFunc) {
        handleDataFunc = nHandleRecvFunc;
    }
};

#endif // !_CLIENT_H_
