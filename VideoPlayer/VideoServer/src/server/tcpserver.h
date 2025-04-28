#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_
#include <map>

#include "Acceptor.h"
#include "Poller.h"
#include "Address.h"
#include "TcpConnection.h"

class TcpServer
{
public:
    TcpServer(Poller *poller, const Ipv4Address& addr);
    virtual ~TcpServer();
    void start();

protected:

    virtual void handleNewConnection(int connfd);
    virtual void handleDisconnection(int sockfd);
private:
    static void newConnectionCallback(void* arg, int connfd);
    static void disconnectionCallback(void* arg, int sockfd);

protected:
    Poller* mPoller;
    Acceptor* mAcceptor;
    Ipv4Address mAddr;
    std::map<int, TcpConnection*> mTcpConnections;
};

#endif //_TCP_SERVER_H_