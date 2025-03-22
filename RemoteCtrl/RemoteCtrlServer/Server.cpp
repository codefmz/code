#include "pch.h"
#include "Packet.h"
#include "Server.h"
#include "Client.h"
#include "Network.h"
#include "RCEvent.h"
#include <cassert>

Server::Server() : serSock(INVALID_SOCKET), postAcceptNum(0), procRecvFunc(nullptr)
{
    InitializeCriticalSection(&critSection);
}

Server::~Server()
{
    DeleteCriticalSection(&critSection);
}

Server* Server::instance()
{
    static Server* server = new Server();
    return server;
}

bool Server::Create(short port, PROC_RECV_FUNC procRecvFunc)
{
    serSock = Network::CreateSocket(true, port);
    if (serSock == INVALID_SOCKET) {
        return false;
    }

    bool reuseAddr = true;
    if (setsockopt(serSock, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&reuseAddr),
        sizeof(reuseAddr)) == SOCKET_ERROR) {
        LOG(ERR, "setsocket reuseaddr failed, errno = %d", WSAGetLastError());
        Destroy();
        return false;
    }

    if (BindIoCompletionCallback(reinterpret_cast<HANDLE>(serSock), Server::OnIOCompletion, 0) == false) {
        LOG(ERR, "BindIoCompletionCallback failed, errno = %d", WSAGetLastError());
        Destroy();
        return false;
    }

    if (listen(serSock, SOMAXCONN) == SOCKET_ERROR) {
        LOG(ERR, "listen failed, errno = %d", WSAGetLastError());
        Destroy();
        return false;
    }

    if (!QueueUserWorkItem(Server::WorkerPostAccept, this, WT_EXECUTEDEFAULT)) {
        LOG(ERR, "QueueUserWorkItem failed, errno = %d", WSAGetLastError());
        Destroy();
        return false;
    }
    this->procRecvFunc = procRecvFunc;

    return true;
}

void Server::Destroy()
{
    EnterCriticalSection(&critSection);
    for (auto iter : clients) {
        iter->Destroy();
    }
    clients.clear();
    LeaveCriticalSection(&critSection);
}

void WINAPI Server::OnIOCompletion(DWORD errCode, DWORD byteNum, LPOVERLAPPED lpOverlapped)
{
    RCEvent* event = CONTAINING_RECORD(lpOverlapped, RCEvent, GetOverlapped());
    assert(event);

    if (errCode != ERROR_SUCCESS) {
        LOG(ERR, "OnIOCompletion failed, errno = %d, type", WSAGetLastError());
        return;
    }

    switch (event->type) {
        case RC_EVENT_ACCEPT:
            Server::instance()->OnAccept(event);
            break;
        case RC_EVENT_RECV:
            if (byteNum > 0) {
                Server::instance()->OnRecv(event, byteNum);
            } else {
                Server::instance()->OnClose(event);
            }
            break;
        case RC_EVENT_SEND: {
            static int sendTimes = 0;
            LOG(ERR, "client send num, sendTimes = %d", ++sendTimes);
            Server::instance()->OnSend(event, byteNum);
            break;
        }
        default:
            assert(false);
            break;
    }

    RCEvent::Destroy(event);
}

DWORD WINAPI Server::WorkerPostAccept(LPVOID lpParam)
{
    Server* server = static_cast<Server*>(lpParam);
    assert(server);

    bool loop = true;
    while (loop) {
        server->PostAccept();
    }
    return 0;
}

DWORD WINAPI Server::WorkerAddClient(LPVOID lpParam)
{
    Client* client = static_cast<Client*>(lpParam);
    assert(client);

    Server::instance()->AddClient(client);
    return 0;
}

void Server::OnAccept(RCEvent* event)
{
    assert(event);

    LOG(ERR, "[%d] Enter OnAccept()", GetCurrentThreadId());
    assert(event->type == RC_EVENT_ACCEPT);

    // Add client in a different thread.
    // It is because we need to return this function ASAP so that this IO worker thread can process the other IO notifications.
    // If adding client is fast enough, we can call it here but I assume it's slow.	
    if (!QueueUserWorkItem(Server::WorkerAddClient, event->client, WT_EXECUTEDEFAULT)) {
        LOG(ERR, "Could not start WorkerAddClient, errno = %d.", WSAGetLastError());
        AddClient(event->client);
    }

    LOG(ERR, "[%d] Leave OnAccept()", GetCurrentThreadId());
}

void Server::OnSend(RCEvent* event, DWORD byteNum)
{
    assert(event);
    LOG(ERR, "[%d] Enter OnSend()", GetCurrentThreadId());

    // This should be fast enough to do in this I/O thread.
    // if not, we need to queue it like what we do in OnRecv().
}

void Server::OnRecv(RCEvent* event, DWORD byteNum)
{
    assert(event);

    LOG(INFO, "[%d] Enter OnRecv()", GetCurrentThreadId());
    Client* client = event->client;
    char* buff = client->GetRecvBuffer();
    buff[byteNum] = '\0';

    //// Create packet by copying recv buff.
    //Packet* packet = Packet::Create(event->GetClient(), event->GetClient()->GetRecvBuff(), dwNumberOfBytesTransfered);
    bool isCreateOk = false;
    Packet* packet = new Packet(buff, byteNum, isCreateOk);
    if (!isCreateOk || packet == nullptr) {
        LOG(INFO, "prarse packet failed.");
        //TODO 失败的时候应该继续postRecv
        return;
    }

    // If whatever game logics relying on the packet are fast enough, we can manage them here but I assume they are slow.	
    // I think it's better to request receiving ASAP and handle packets received in another thread.
    //if (QueueUserWorkItem(Server::WorkerProcessRecvPacket, client, WT_EXECUTEDEFAULT) == false) {
    //    LOG(Log::ERR, "Could not start WorkerProcessRecvPacket. call it directly, errno = %d.", GetLastError());
    //    WorkerProcessRecvPacket(packet);
    //}
    ProcessRecvPacket(client, packet); /* 暂时放在这里进行处理，后续需要放到线程里进行处理 */
    PostRecv(client);
    LOG(INFO, "[%d] Leave OnRecv()", GetCurrentThreadId());
}

DWORD WINAPI Server::WorkerRemoveClient(LPVOID lpParam)
{
    Client* client = static_cast<Client*>(lpParam);
    assert(client);

    Server::instance()->RemoveClient(client);
    return 0;
}

void Server::OnClose(RCEvent* event)
{
    assert(event);

    TRACE("Client's socket has been closed.");

    // If whatever game logics about this event are fast enough, we can manage them here but I assume they are slow.	
    if (QueueUserWorkItem(Server::WorkerRemoveClient, event->client, WT_EXECUTEDEFAULT) == false) {
        LOG(INFO, "can't start WorkerRemoveClient. call it directly, errno = %d", WSAGetLastError());
        WorkerRemoveClient(event->client);
    }
}

void Server::AddClient(Client* client)
{
    assert(client);

    /* The socket sAcceptSocket does not inherit the properties of the socket
    associated with sListenSocket parameter until SO_UPDATE_ACCEPT_CONTEXT is set on the socket.
    socket create by AcceptEx must be linked to listen socket, so that is can inherit the context of listen socket */
    if (setsockopt(client->GetSocket(), SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
        reinterpret_cast<const char*>(&serSock), sizeof(serSock)) == SOCKET_ERROR) {
        LOG(ERR, "[%d] Leave OnAccept()", GetCurrentThreadId());
        return;
    }

    client->SetState(RC_SOCKET_ACCEPT);
    if (!BindIoCompletionCallback(reinterpret_cast<HANDLE>(client->GetSocket()), Server::OnIOCompletion, 0)) {
        LOG(ERR, "BindIoCompletionCallback soecket, errno = %d.", WSAGetLastError());
        return;
    }

    std::string ip;
    u_short port = 0;
    Network::GetRemoteAddress(client->GetSocket(), ip, port);
    LOG(ERR, "[%d] Accept succeeded. client address : ip[%s], port[%d]", GetCurrentThreadId(), ip.c_str(), port);

    EnterCriticalSection(&critSection);
    clients.push_back(client);
    LeaveCriticalSection(&critSection);

    PostRecv(client);
}

void Server::RemoveClient(Client* client)
{
    assert(client);
    EnterCriticalSection(&critSection);
    std::list<Client*>::iterator itor = std::remove(clients.begin(), clients.end(), client);

    if (itor != clients.end()) {
        LOG(ERR, "[%d] RemoveClient succeeded.", GetCurrentThreadId());
        client->Destroy();
    }
    LeaveCriticalSection(&critSection);
}

void Server::ProcessRecvPacket(Client* client, Packet* packet)
{
    std::list<Packet> lstPackets;
    procRecvFunc(packet, lstPackets);
    while (!lstPackets.empty()) {
        PostSend(client, &(lstPackets.front()));
        Sleep(1);
        lstPackets.pop_front();
    }
}

void Server::PostAccept()
{
    if (postAcceptNum < RC_MAX_LISTEN_NUM) {
        Client* client = Client::ServerCreate();
        if (!client) {
            return;
        }

        RCEvent* event = RCEvent::Create(client, RC_EVENT_ACCEPT);
        if (!event) {
            client->Destroy();
            delete client;
            return;
        }

        if (!Network::AcceptEx(serSock, client->GetSocket(), &event->GetOverlapped())) {
            int errNum = WSAGetLastError();
            if (errNum != ERROR_IO_PENDING) {
                LOG(ERR, "AcceptEx fail, errNum = %d", errNum);
                return;
            }
        } else {
            /* 如果立刻返回，表示它已经accept完成，并且不会放到iocp里面去，需要手动将它添加到m_clients */
            OnAccept(event);
            RCEvent::Destroy(event);
        }
        postAcceptNum++;
    } else {
        Sleep(10 * 1000);
    }
}

void Server::PostRecv(Client* client)
{
    assert(client);

    WSABUF recvBufferDescriptor;
    recvBufferDescriptor.buf = client->GetRecvBuffer();
    recvBufferDescriptor.len = RC_RECV_BUF_SIZE;

    DWORD numberOfBytes = 0;
    DWORD recvFlags = 0;

    RCEvent* event = RCEvent::Create(client, RC_EVENT_RECV);
    if (!event) {
        return;
    }

    if (WSARecv(client->GetSocket(), &recvBufferDescriptor, 1, &numberOfBytes, &recvFlags,
        &event->GetOverlapped(), NULL) == SOCKET_ERROR) {
        int error = WSAGetLastError();
        if (error != ERROR_IO_PENDING) {
            LOG(ERR, "WSARecv() failed., errno = %d", error);
            OnClose(event);
            RCEvent::Destroy(event);
        }
    } else {
        LOG(ERR, "WSARecv() return without iocp.");
        // MSDN
        // In this case, the completion routine will have already been scheduled to be called once the calling thread is in the alertable state.
    }
}

void Server::PostSend(Client* client, Packet* packet)
{
    assert(client);
    assert(packet);

    uint32_t size = packet->Size();
    if (size > RC_SEND_BUF_SIZE) {
        LOG(ERR, "post send msg size : % u is bigger than sendBuf : %u", size, RC_SEND_BUF_SIZE);
        return;
    }
    char* packetData = new char[packet->Size()];
    packet->Data(packetData);
    WSABUF recvBufferDescriptor;
    recvBufferDescriptor.buf = packetData;
    recvBufferDescriptor.len = packet->Size();

    DWORD sendFlags = 0;
    RCEvent* event = RCEvent::Create(client, RC_EVENT_SEND);
    assert(event);
    event->packetData = packetData;
    if (WSASend(client->GetSocket(), &recvBufferDescriptor, 1, NULL, sendFlags, &event->GetOverlapped(), NULL) != RC_OK) {
        int error = WSAGetLastError();
        if (error != ERROR_IO_PENDING) {
            LOG(ERR, "WSASend() failed., errno = %d", error);
            RemoveClient(client);
        }
    } else {

        // MSDN
        // In this case, the completion routine will have already been scheduled to be called once the calling thread is in the alertable state.
    }
}
