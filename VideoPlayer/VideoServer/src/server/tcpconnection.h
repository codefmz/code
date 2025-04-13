#ifndef _TCP_CONNECTION_H_
#define _TCP_CONNECTION_H_
#include "poller.h"
#include "event.h"
#include "tcpsocket.h"
#include "buffer.h"

class TcpConnection
{
public:
    typedef void (*DisconnectionCallback)(void*, int);

    TcpConnection(Poller* poller, int sockfd);
    virtual ~TcpConnection();

    void setDisconnectionCallback(DisconnectionCallback cb, void* arg);

protected:
    void enableReadHandling();
    void enableWriteHandling();
    void enableErrorHandling();
    void disableReadeHandling();
    void disableWriteHandling();
    void disableErrorHandling();

    void handleRead();
    virtual void handleReadBytes();
    virtual void handleWrite();
    virtual void handleError();

    void handleDisconnection();

private:
    static void readCallback(void* arg);
    static void writeCallback(void* arg);
    static void errorCallback(void* arg);

protected:
    Poller* mPoller;
    TcpSocket mSocket;
    IOEvent* mTcpConnIOEvent;
    DisconnectionCallback mDisconnectionCallback;
    void* mArg;
    Buffer mInputBuffer;
    Buffer mOutBuffer;
    char mBuffer[2048];
};

#endif //_TCP_CONNECTION_H_