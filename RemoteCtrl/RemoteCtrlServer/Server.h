#ifndef _SERVER_H_
#define _SERVER_H_

#include <winsock2.h>
#include <list>
#include <atomic>

class Client;
class Packet;
class RCEvent;

#define RC_MAX_LISTEN_NUM 10 /* 最多监听10个客户端 */

typedef void (*PROC_RECV_FUNC)(Packet* packet, std::list<Packet>& packList);

class Server
{
private:
    SOCKET serSock;
    std::list<Client*> clients;
    std::atomic<int> postAcceptNum;
    CRITICAL_SECTION critSection;
    PROC_RECV_FUNC procRecvFunc;

public:
    Server();
    virtual ~Server();
    static Server* instance();

    bool Create(short port, PROC_RECV_FUNC procRecvFunc);
    void Destroy();
private:
    static void WINAPI OnIOCompletion(DWORD errCode, DWORD byteNum, LPOVERLAPPED lpOverlapped);
    // Worker Thread Functions
    static DWORD WINAPI WorkerPostAccept(LPVOID lpParam);
    static DWORD WINAPI WorkerAddClient(LPVOID lpParam);
    static DWORD WINAPI WorkerRemoveClient(LPVOID lpParam);

    void OnAccept(RCEvent* event);
    void OnRecv(RCEvent* event, DWORD byteNum);
    void OnSend(RCEvent* event, DWORD byteNum);
    void OnClose(RCEvent* event);

    void PostAccept();
    void PostRecv(Client* client);
    void PostSend(Client* client, Packet* packet);

    void ProcessRecvPacket(Client* client, Packet* packet);
    void AddClient(Client* client);
    void RemoveClient(Client* client);
};

#endif /* _SERVER_H_ */
