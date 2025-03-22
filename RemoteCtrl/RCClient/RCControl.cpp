#include "pch.h"
#include "resource.h"
#include "RCControl.h"
#include "Client.h"
#include "Network.h"

RCControl* RCControl::instance(void)
{
    static RCControl* pInstance = new RCControl;
    return pInstance;
}

RCControl::RCControl(void)
{
    recvBuf.reserve(RC_RECV_BUF_SIZE);
    recvSem = INVALID_HANDLE_VALUE;
    recvThd = INVALID_HANDLE_VALUE;
    watchThd = INVALID_HANDLE_VALUE;
    isRecving = false;
    client = new Client;
    if (client == nullptr) {
        LOG(RC_ERR, "new client fail");
    }
}

RCControl::~RCControl(void)
{
}

int RCControl::StartRecv(void)
{
    if (!Network::Initialize()) {
        LOG(ERR, "Network initialize fail!");
        return RC_ERR;
    }

    if (!client->Create()) {
        LOG(ERR, "pClient create fail!");
        return RC_ERR;
    }
    client->SetHandleRecvDataFunc(RCControl::RecvData);

    recvSem = CreateSemaphore(NULL, 0, 3, NULL);
    if (recvSem == NULL) {
        client->Destroy();
        LOG(ERR, "create semphare fail!");
        return RC_ERR;
    }

    isRecving = true;
    unsigned int tid;
    recvThd = (HANDLE)_beginthreadex(NULL, 0, &RCControl::RecvEntry, this, 0, &tid);
    if (recvThd == NULL) {
        client->Destroy();
        CloseHandle(recvSem);
        LOG(ERR, "create semphare fail!");
        return RC_ERR;
    }

    return RC_OK;
}

int RCControl::StopRecv(void)
{
    isRecving = false;
    ReleaseSemaphore(recvSem, 1, NULL);
    WaitForSingleObject(recvThd, 2000);
    CloseHandle(recvThd);
    CloseHandle(recvSem);
    if (client != nullptr) {
        client->Destroy();
        delete client;
        client = nullptr;
    }
    Network::Deinitialize();

    return RC_OK;
}

bool RCControl::SendPacket(Packet* packet)
{
    bool ret;
    uint32_t size = packet->Size();
    char* data = new char[size];
    packet->Data(data);

    ret = client->PostSend(data, size);
    delete[] data;
    return ret;
}

void RCControl::UpdateAddress(DWORD nIP, WORD nPort)
{
    client->UpdateAddress(nIP, nPort);
}

void RCControl::RecvDataInner(char* data, uint32_t len)
{
    mtx.lock();
    while (recvBuf.size() + len >= RC_RECV_BUF_SIZE) {
        mtx.unlock();
        Sleep(1); /* 放权1s */
        mtx.lock();
    }
    recvBuf.append((const char*)data, len);
    while (!ReleaseSemaphore(recvSem, 1, NULL)) {}
    mtx.unlock();
}

bool RCControl::StartConnect()
{
    uint32_t waitTime = 3;

    if (client->GetState() == RC_SOCKET_CONNECT) {
        return true;
    }

    bool ret = client->PostConnect();
    if (!ret) {
        LOG(ERR, "client post connect fail.");
        return ret;
    }
    while (client->GetState() != RC_SOCKET_CONNECT && waitTime > 0) {
        Sleep(1000); /* 3s 等待连接*/
        waitTime--;
    }

    return client->GetState() == RC_SOCKET_CONNECT;
}

void RCControl::ProcessData(void)
{
    while (isRecving) {
        WaitForSingleObject(recvSem, INFINITE);
        mtx.lock();

        bool isCreated = false;
        Packet packet(recvBuf.c_str(), recvBuf.size(), isCreated);
        if (!isCreated) {
            mtx.unlock();
            continue;
        }

        ::SendMessage((HWND)(uintptr_t)packet.hwnd, WM_RESPONSE_PACK, (WPARAM)&packet, 0);

        recvBuf.erase(0, packet.Size());
        mtx.unlock();
    }
}

unsigned RCControl::RecvEntry(void* arg)
{
    RCControl* thiz = (RCControl*)arg;
    thiz->ProcessData();
    return RC_OK;
}
