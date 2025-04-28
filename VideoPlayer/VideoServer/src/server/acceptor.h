#ifndef _ACCEPTOR_H_
#define _ACCEPTOR_H_
#include "Event.h"
#include "Address.h"
#include "Poller.h"
#include "TcpSocket.h"

class Acceptor
{
public:
    typedef void(*NewConnectionCallback)(void* data, int connfd);

    static Acceptor* createNew(Poller* poller, const Ipv4Address& addr);

    Acceptor(Poller* poller, const Ipv4Address& addr);
    ~Acceptor();

    bool listenning() const {
        return mListenning;
    }

    void listen();
    void setNewConnectionCallback(NewConnectionCallback cb, void* arg);

private:
    static void readCallback(void*);
    void handleRead();

private:
    Poller* mPoller;
    Ipv4Address mAddr;
    IOEvent* mAcceptIOEvent;
    TcpSocket mSocket;
    bool mListenning;
    NewConnectionCallback mNewConnectionCallback;
    void* mArg;
};

#endif //_ACCEPTOR_H_