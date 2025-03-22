#include "pch.h"
#include "Client.h"
#include "RCEvent.h"
#include "Network.h"
#include <cassert>

void Client::Destroy(void)
{
    if (state != RC_SOCKET_CLOSE) {
        CancelIoEx(reinterpret_cast<HANDLE>(socket), NULL); /* 取消所有关于该文件的io操作 */
        Network::CloseSocket(socket);
        socket = INVALID_SOCKET;
        state = RC_SOCKET_CLOSE;
    }
}

Client* Client::ServerCreate(void)
{
    SOCKET clientSock = INVALID_SOCKET;

    clientSock = Network::CreateSocket(false, 0);
    if (clientSock == INVALID_SOCKET) {
        LOG(ERR, "create client socket fail");
        return nullptr;
    }
    Client* client = new Client;
    client->socket = clientSock;

    return client;
}
