#pragma once
#include "pch.h"
#include "framework.h"
#include "Packet.h"
#include <vector>
#include <list>
#define BUFFER_SIZE 4096

typedef void (*CALLBACKFUN)(void* arg, int sCmd, std::list<CPacket>& packList, CPacket& packet);
class CServerSock
{
public:
    static CServerSock* instance() {
        static CServerSock* m_instance = new CServerSock;
        return m_instance;
    }
    bool AcceptClient();
    bool Send(const char* pData, int nSize);
    bool Send(CPacket& pack);
    bool InitSocket(short port);
    int  DealCommand(WORD& cmd);
    CPacket& GetPacket();
    void CloseClient();
    int Run(CALLBACKFUN callBack, void* arg, short port = 9527);

private:
    SOCKET serverSock;
    SOCKET clientSock;
    CPacket packet;
    std::vector<char> vctBuf;
    CALLBACKFUN mCallBack;
    void* mArg;

private:
    CServerSock& operator=(const CServerSock& serSock) = delete;

    CServerSock(const CServerSock& serSock) = delete;

    CServerSock() :serverSock(INVALID_SOCKET), clientSock(INVALID_SOCKET) {
        if (InitSockEnv() == false) {
            LOG(Log::ERR, "InitSockEnv fail!");
            MessageBox(NULL, _T("无法初始化环境变量，请检查网络设置!"), _T("初始化错误"), MB_OK | MB_ICONERROR);
            exit(0);
        }
        vctBuf.resize(BUFFER_SIZE);
        LOG(Log::ERR, "Create server socket success.");
    };

    ~CServerSock() {
        WSACleanup();
    };

    bool InitSockEnv() {
        WSADATA data; /* 存储winsock库中的信息和版本信息 */
        /* 使用windows套解字库中的函数来初始化网络套接字环境 */
        if (WSAStartup(MAKEWORD(1, 1), &data) != 0) {
            return false;
        }
        return true;
    };
};
