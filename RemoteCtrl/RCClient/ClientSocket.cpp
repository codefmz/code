#include "pch.h"
#include "ClientSocket.h"
#include "Controller.h"
#include "Util.h"
#include <MSWSock.h>

#pragma warning(disable:4996);

bool CClientSock::ConnectServerSocket()
{
    int ret = 0;

    if (sock != INVALID_SOCKET) {
        return true;
    }
    LOG(Log::INFO, "begin connect server socket.");
    sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (sock == INVALID_SOCKET) {
        LOG(Log::ERR, "serSock error");
        return false;
    }

    int opt = 1;
    ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
    if (ret != 0) {
        LOG(Log::ERR, "set socket reuse failed.");
        return false;
    }

    sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = htonl(nIP);
    serAddr.sin_port = htons(nPort);
    if (serAddr.sin_addr.s_addr == INADDR_NONE) {
        LOG(Log::ERR, "ip addr is not exit!");
        closesocket(sock);
        return false;
    }

    GUID connectExGuid = WSAID_CONNECTEX;
    LPFN_CONNECTEX lpfnConnectEx = NULL;
    DWORD bytesReturned = 0;

    ret = WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER, &connectExGuid, sizeof(connectExGuid),
        &lpfnConnectEx, sizeof(lpfnConnectEx), &bytesReturned, NULL, NULL);

    if (ret == SOCKET_ERROR) {
        LOG(Log::ERR, "WSAIoctl failed: %ld\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return false;
    }

    sockaddr_in localAddr;
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = INADDR_ANY;
    localAddr.sin_port = 0; // 让系统自动选择一个端口

    ret = bind(sock, (sockaddr*)&localAddr, sizeof(localAddr));
    if (ret == SOCKET_ERROR) {
        printf("bind failed: %ld\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return false;
    }

    OVERLAPPED overlapped = {};
    overlapped.Offset = 111;

    BOOL connectResult = lpfnConnectEx(
        sock,
        (sockaddr*)&serAddr,
        sizeof(serAddr),
        NULL,
        0,
        NULL,
        &overlapped
    );

    if (connectResult == FALSE && WSAGetLastError() != ERROR_IO_PENDING) {
        LOG(Log::ERR, "client connect error， errno = %d", WSAGetLastError());
        closesocket(sock);
        return false;
    }
    return true;
}


bool CClientSock::Send(const char* pData, int nSize)
{
    if (sock == INVALID_SOCKET) {
        return false;
    }
    return send(sock, pData, nSize, 0) > 0;
}

void CClientSock::ReceivePack()
{
    //size_t index = 0, alreadyUseLen = 0;
    //bool isReOk;
    //DWORD beforeAckNo = -1, curAckNo;
    //std::string strBuffer;
    //PACKET_DATA* data = NULL;
    //strBuffer.resize(BUFFER_SIZE);
    //char* pBuffer = (char*)strBuffer.c_str();

    //while (isRecvRuning) {
    //    if (sock == INVALID_SOCKET) {
    //        Sleep(1);
    //        continue;
    //    }

    //    while (true) {
    //        int recevLen = recv(sock, pBuffer + index, BUFFER_SIZE - index, 0);
    //        if (recevLen <= 0) {
    //            LOG(Log::ERR, "recv error len < 0, errno = %d", errno);
    //            sock = INVALID_SOCKET;
    //            break;
    //        }

    //        if (recevLen > BUFFER_SIZE) {
    //            LOG(Log::ERR, "recv error len(%d) > BUFFER_SIZE, errno = %d", recevLen, errno);
    //            break;
    //        }

    //        index += recevLen;
    //        do {
    //            CPacket pack((BYTE*)pBuffer + alreadyUseLen, index - alreadyUseLen, isReOk);
    //            if (!isReOk) {
    //                break;
    //            }

    //            curAckNo = pack.ackNo;
    //            auto iter = ackDataMap.find(curAckNo);
    //            if (iter == ackDataMap.end()) {
    //                LOG(Log::ERR, "can't find curAckNo(%lu) in ackDataMap.", curAckNo);
    //                continue;
    //            }
    //            data = iter->second;
    //            CController::instance()->ResolveRecv(pack, data->hwnd, data->wParam);
    //            if (curAckNo != beforeAckNo) {
    //                /* 当前ackNo 不等于上一个ackNo, 说明是另外一个命令的返回值，需要删一下前一个的包 */
    //                auto beforeIter = ackDataMap.find(beforeAckNo);
    //                if (beforeIter != ackDataMap.end()) {
    //                    delete beforeIter->second;
    //                    ackDataMap.erase(beforeIter);
    //                }
    //            }
    //            beforeAckNo = curAckNo;
    //            alreadyUseLen += pack.Size();
    //        } while (true);
    //        memmove(pBuffer, pBuffer + alreadyUseLen, index - alreadyUseLen);
    //        index -= alreadyUseLen;
    //        alreadyUseLen = 0;
    //    }
    //}
}

void CClientSock::SendPacket(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
    PACKET_DATA* data = (PACKET_DATA*)wParam;
    if (ConnectServerSocket() == false) {
        CloseSocket();
        CPacket packet;
        packet.cmd = 9528;
        ::SendMessage(data->hwnd, WM_RESPONSE_PACK, (WPARAM)&packet, 0); /* 发送连接失败指令 */
        delete data;
        return;
    }
    ackDataMap.insert({ data->mPacket.ackNo, data });
    //int ret = send(sock, data->mPacket.Data(), data->mPacket.Size(), 0);
    //if (ret < 0) {
    //    LOG(Log::ERR, "send packet fail!");
    //    ackDataMap.erase(data->mPacket.ackNo);
    //    delete data;
    //    return;
    //}
}

bool CClientSock::SendPacket(HWND hWnd, CPacket& pack, bool isAutoClosed, WPARAM wParam)
{
    UINT nMode = isAutoClosed ? CSM_AUTOCLOSE : 0;
    PACKET_DATA* pData = new PACKET_DATA(pack, nMode, hWnd, wParam);
    bool ret = PostThreadMessage(nThreadID, WM_SEND_PACK, (WPARAM)pData, NULL);
    if (ret == false) {
        delete pData;
    }
    return ret;
}


CPacket& CClientSock::GetPacket()
{
    return packet;
}

void CClientSock::CloseSocket()
{
    closesocket(sock);
    sock = INVALID_SOCKET;
}

unsigned __stdcall CClientSock::threadEntry(void* arg)
{
    CClientSock* thiz = (CClientSock*)arg;
    thiz->threadFunc2();
    _endthreadex(0);
    return 0;
}

unsigned __stdcall CClientSock::recvThreadEntry(void* arg)
{
    CClientSock* thiz = (CClientSock*)arg;
    thiz->ReceivePack();
    _endthreadex(0);
    return 0;
}

void CClientSock::threadFunc2()
{
    SetEvent(eventInvoke);
    MSG msg;
    while (::GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (umapFunc.find(msg.message) != umapFunc.end()) {
            (this->*umapFunc[msg.message])(msg.message, msg.wParam, msg.lParam);
        }
    }
}

void CClientSock::UpdateAddress(DWORD nIP, WORD nPort)
{
    if ((this->nIP != nIP) || (this->nPort != nPort)) {
        this->nIP = nIP;
        this->nPort = nPort;
    }
}

CClientSock::CClientSock()
    :nIP(INADDR_ANY), nPort(0), sock(INVALID_SOCKET), bAutoClose(true), hThread(INVALID_HANDLE_VALUE), isRecvRuning(true)
{
    LOG(Log::INFO, "client sock begin create instance.");
    if (InitSockEnv() == false) {
        LOG(Log::LOG_LEVEL::ERR, "init sockenv fail.");
        return;
    }

    /* 创建发线程 */
    eventInvoke = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (eventInvoke == NULL) {
        LOG(Log::ERR, "Create send event fail.");
        return;
    }
    hThread = (HANDLE)_beginthreadex(NULL, 0, &CClientSock::threadEntry, this, 0, &nThreadID);
    if (WaitForSingleObject(eventInvoke, 100) == WAIT_TIMEOUT) {
        LOG(Log::ERR, "网络消息处理线程启动失败了！\r\n");
    }
    CloseHandle(eventInvoke);

    /* 创建收包线程 */
    recvThreadHd = (HANDLE)_beginthreadex(NULL, 0, &CClientSock::recvThreadEntry, this, 0, &recvThreadId);

    vctBuf.resize(BUFFER_SIZE);
    memset(vctBuf.data(), 0, BUFFER_SIZE);
    struct {
        UINT message;
        MSGFUNC func;
    }funcs[] = {
        {WM_SEND_PACK, &CClientSock::SendPacket},
        {0,NULL}
    };
    for (int i = 0; funcs[i].message != 0; i++) {
        if (umapFunc.insert(std::pair<UINT, MSGFUNC>(funcs[i].message, funcs[i].func)).second == false) {
            LOG(Log::ERR, "插入失败，消息值：%d 函数值:%08X 序号:%d\r\n", funcs[i].message, funcs[i].func, i);
        }
    };
}
