#include "pch.h"
#include "Client.h"
#include "RCEvent.h"
#include "Network.h"
#include <cassert>

Client::Client() : state(RC_SOCKET_WAIT), ip(0), port(0), socket(INVALID_SOCKET), handleDataFunc(nullptr)
{
    recvBuf = new char[RC_RECV_BUF_SIZE];
    if (recvBuf == nullptr) {
        LOG(ERR, "new m_recvBuffer fail, size = %u", RC_RECV_BUF_SIZE);
    }
    memset(recvBuf, 0, RC_RECV_BUF_SIZE);
}

Client::~Client()
{
    if (recvBuf != nullptr) {
        delete[] recvBuf;
        recvBuf = nullptr;
    }
}

bool Client::Create()
{
    assert(socket == INVALID_SOCKET);
    assert(state == RC_SOCKET_WAIT);

    uint16_t port = PORT_START;
    // Create Socket
    socket = Network::CreateSocket(true, port);
    if (socket == INVALID_SOCKET) {
        return false;
    }

    // Make the address re-usable to re-run the same client instantly.
    bool reuseAddr = true;
    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&reuseAddr),
        sizeof(reuseAddr)) == SOCKET_ERROR) {
        LOG(ERR, "setsockopt() failed with SO_REUSEADDR，errno = %d.", WSAGetLastError());
        return false;
    }

    // Connect the socket to IOCP
    if (BindIoCompletionCallback(reinterpret_cast<HANDLE>(socket), Client::OnIOCompletion, 0) == false) {
        LOG(ERR, "BindIoCompletionCallback() failed，errno = %d.", WSAGetLastError());
        return false;
    }
    state = RC_SOCKET_CREATE;

    return true;
}

void Client::Destroy(void)
{
    if (state != RC_SOCKET_CLOSE) {
        CancelIoEx(reinterpret_cast<HANDLE>(socket), NULL); /* 取消所有关于该文件的io操作 */
        Network::CloseSocket(socket);
        socket = INVALID_SOCKET;
        state = RC_SOCKET_CLOSE;
    }
}

void WINAPI Client::OnIOCompletion(DWORD errCode, DWORD transNum, LPOVERLAPPED pOverLapped)
{
    RCEvent* event = CONTAINING_RECORD(pOverLapped, RCEvent, overlapped);
    assert(event);
    assert(event->client);

    //ERROR_SUCCESS success but not execute
    if (errCode != ERROR_SUCCESS) {
        LOG(ERR, "errCode = %d, I/O operation failed.", errCode);
        event->client->OnClose();
        RCEvent::Destroy(event);
        return;
    }

    switch (event->type) {
        case RC_EVENT_CONNECT:
            event->client->OnConnect();
            break;
        case RC_EVENT_RECV:
            if (transNum > 0) {
                static int recvTimes = 0;
                LOG(ERR, "client recv num, recvTimes = %d", ++recvTimes);
                event->client->OnRecv(transNum);
            } else {
                event->client->OnClose();
            }
            break;
        case RC_EVENT_SEND:
            event->client->OnSend(transNum);
            break;
        default:
            break;
    }

    RCEvent::Destroy(event);
}

void Client::OnConnect()
{
    /* The socket s does not enable previously set properties or options until
    SO_UPDATE_CONNECT_CONTEXT is set on the socket.*/
    if (setsockopt(socket, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 1) == SOCKET_ERROR) {
        LOG(ERR, "setsockopt() failed, errno = %d.", WSAGetLastError());
    } else {
        PostReceive();
    }
}

void Client::OnRecv(DWORD transNum)
{
    // Do not process packet received here.
    // Instead, publish event with the packet and call PostRecv()
    ProcessPacket(transNum);
    // To maximize performance, post recv request ASAP. 
    PostReceive();
}

void Client::OnSend(DWORD transNum)
{
    (void)transNum;
}

void Client::OnClose()
{
    Destroy();
}

bool Client::PostConnect(void)
{
    assert(socket != INVALID_SOCKET);
    assert(state == RC_SOCKET_CREATE);

    /* Get Address Info */
    addrinfo hints;
    ZeroMemory(&hints, sizeof(addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = 0;

    char portStr[16] = { 0 };
    char ipStr[16] = { 0 };
    (void)sprintf_s(portStr, sizeof(portStr), "%d", port);
    ipv4ToStr(ip, ipStr, sizeof(ipStr));

    struct addrinfo* info = NULL;
    if (getaddrinfo(ipStr, portStr, &hints, &info) != 0) {
        LOG(ERR, "getaddrinfo() failed, errno = %d.", WSAGetLastError());
        return false;
    }

    RCEvent* event = RCEvent::Create(this, RC_EVENT_CONNECT);
    if (!Network::ConnectEx(socket, info->ai_addr, info->ai_addrlen, &event->overlapped)) {
        int error = WSAGetLastError();
        if (error != ERROR_IO_PENDING) {
            LOG(ERR, "ConnectEx() failed, errno = %d.", error);
            RCEvent::Destroy(event);
            return false;
        }
        return true;
    }

    OnConnect();
    RCEvent::Destroy(event);
    return true;
}

bool Client::PostReceive()
{
    assert(state == RC_SOCKET_CREATE || state == RC_SOCKET_CONNECT);

    WSABUF recvBufferDescriptor;
    recvBufferDescriptor.buf = recvBuf;
    recvBufferDescriptor.len = RC_RECV_BUF_SIZE;

    DWORD byteNum = 0;
    DWORD recvFlags = 0;
    RCEvent* event = RCEvent::Create(this, RC_EVENT_RECV);

    if (WSARecv(socket, &recvBufferDescriptor, 1, &byteNum, &recvFlags,
        &event->overlapped, NULL) == SOCKET_ERROR) {
        int error = WSAGetLastError();
        if (error != ERROR_IO_PENDING) {
            if (state == RC_SOCKET_CREATE) {
                /* Even though we get successful connection event,
                if our first call of WSARecv failed,
                it means we failed in connecting. */
                LOG(ERR, "Server cannot accept this connection.", error);
            } else {
                LOG(ERR, "WSARecv() failed.", error);
            }
            Destroy();
            return false;
        }
    }
    /* In this case, the completion routine will have already been scheduled to be
    called once the calling thread is in the alertable state.
    I strongly believe it will trigger my OnIOCompletion() since there is no completion routine.
    If this is the first call of WSARecv, we can now set the state CONNECTE.*/
    if (state == RC_SOCKET_CREATE) {
        //PrintConnectionInfo(m_Socket);
        state = RC_SOCKET_CONNECT;
    }
    // TODO, 这里如果是立刻返回这里是否会走到iocp里面去
}

bool Client::PostSend(const char* buffer, uint32_t size)
{
    if (state != RC_SOCKET_CONNECT) {
        return false;
    }

    if (size >= RC_SEND_BUF_SIZE) {
        LOG(ERR, "Post send size(%u) is bigger then max send buf(%u).", size, RC_SEND_BUF_SIZE);
        return false;
    }

    WSABUF recvBufferDescriptor;
    recvBufferDescriptor.buf = reinterpret_cast<char*>(sendBuf);
    recvBufferDescriptor.len = size;

    CopyMemory(sendBuf, buffer, size);

    DWORD byteNum = size;
    DWORD sendFlags = 0;
    RCEvent* event = RCEvent::Create(this, RC_EVENT_SEND);

    /* If no error occurs and the send operation has completed immediately, WSASend returns zero.
    Otherwise, a value of SOCKET_ERROR is returned, and a specific error code can be retrieved by calling WSAGetLastError.
    The error code WSA_IO_PENDING indicates that the overlapped operation has been successfully initiated and
    that completion will be indicated at a later time.
    Any other error code indicates that the overlapped operation was not successfully initiated
    and no completion indication will occur */
    if (WSASend(socket, &recvBufferDescriptor, 1, &byteNum, sendFlags,
        &event->overlapped, NULL) == SOCKET_ERROR) {
        int error = WSAGetLastError();
        if (error != ERROR_IO_PENDING) {
            LOG(ERR, "WSASend() failed, errno = %d.", error);
            RCEvent::Destroy(event);
            Destroy();
            return false;
        }
        return true;
    }
    //TODO， 如果立刻返回是否会进入iocp
    return true;
}

bool Client::Shutdown()
{
    assert(socket != INVALID_SOCKET);
    if (state != RC_SOCKET_CONNECT) {
        return false;
    }

    /* shutdown 在网络编程中用于部分关闭或者完全关闭一个已经连接的套接字，下面的作用就是关闭发送 */
    if (shutdown(socket, SD_SEND) == SOCKET_ERROR) {
        LOG(ERR, "shutdown() failed, errno = %d.", WSAGetLastError());
        return false;
    }

    return true;
}

void Client::ProcessPacket(DWORD recvLen)
{
    if (handleDataFunc != nullptr) {
        handleDataFunc(recvBuf, recvLen);
    }
}
