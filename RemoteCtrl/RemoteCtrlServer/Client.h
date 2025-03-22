#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <winsock2.h>
#include <string>
#include "Packet.h"
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
    SOCKET socket;
    Packet* packet;
    char recvBuf[RC_RECV_BUF_SIZE];
    char sendBuf[RC_SEND_BUF_SIZE];
public:
    Client() : state(RC_SOCKET_WAIT), ip(0), port(0), socket(INVALID_SOCKET) {
    };
    ~Client() {
    }

    void Destroy(void);
    static Client* ServerCreate(void);

    RC_SOCKET_STATE GetState(void) {
        return state;
    }

    void SetState(RC_SOCKET_STATE aState) {
        state = aState;
    }

    char* GetRecvBuffer(void) {
        return recvBuf;
    }

    Packet* GetPacket() {
        return packet;
    }

    void SetPacket(Packet* packet) {
        this->packet = packet;
    }

    SOCKET GetSocket() {
        return socket;
    }
};

#endif /* _CLIENT_H_ */
