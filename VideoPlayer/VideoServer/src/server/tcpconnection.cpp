#include "tcpconnection.h"
#include "socketops.h"
#include "log.h"

#include <unistd.h>
#include <stdlib.h>

TcpConnection::TcpConnection(Poller* poller, int sockfd) : mPoller(poller), mSocket(sockfd),
    mDisconnectionCallback(NULL), mArg(NULL)
{
    mTcpConnIOEvent = IOEvent::createNew(sockfd, this);
    mTcpConnIOEvent->setReadCallback(readCallback);
    mTcpConnIOEvent->setWriteCallback(writeCallback);
    mTcpConnIOEvent->setErrorCallback(errorCallback);
    mTcpConnIOEvent->enableReadHandling(); //默认只开启读
    mPoller->addIOEvent(mTcpConnIOEvent);
}

TcpConnection::~TcpConnection()
{
    mPoller->removeIOEvent(mTcpConnIOEvent);
    delete mTcpConnIOEvent;
}

void TcpConnection::setDisconnectionCallback(DisconnectionCallback cb, void* arg)
{
    mDisconnectionCallback = cb;
    mArg = arg;
}

void TcpConnection::enableReadHandling()
{
    if(mTcpConnIOEvent->isReadHandling()) {
        return;
    }

    mTcpConnIOEvent->enableReadHandling();
    mPoller->updateIOEvent(mTcpConnIOEvent);
}

void TcpConnection::enableWriteHandling()
{
    if(mTcpConnIOEvent->isWriteHandling()) {
        return;
    }

    mTcpConnIOEvent->enableWriteHandling();
    mPoller->updateIOEvent(mTcpConnIOEvent);
}

void TcpConnection::enableErrorHandling()
{
    if(mTcpConnIOEvent->isErrorHandling()) {
        return;
    }

    mTcpConnIOEvent->enableErrorHandling();
    mPoller->updateIOEvent(mTcpConnIOEvent);
}

void TcpConnection::disableReadeHandling()
{
    if(!mTcpConnIOEvent->isReadHandling()) {
        return;
    }

    mTcpConnIOEvent->disableReadeHandling();
    mPoller->updateIOEvent(mTcpConnIOEvent);
}

void TcpConnection::disableWriteHandling()
{
    if(!mTcpConnIOEvent->isWriteHandling()) {
        return;
    }

    mTcpConnIOEvent->disableWriteHandling();
    mPoller->updateIOEvent(mTcpConnIOEvent);
}

void TcpConnection::disableErrorHandling()
{
    if(!mTcpConnIOEvent->isErrorHandling()) {
        return;
    }

    mTcpConnIOEvent->disableErrorHandling();
    mPoller->updateIOEvent(mTcpConnIOEvent);
}

void TcpConnection::handleRead()
{
    int ret = mInputBuffer.read(mSocket.fd());

    if(ret == 0) {
        LOG_INFO("client disconnect\n");
        handleDisconnection();
        return;
    } else if(ret < 0) {
        LOG_ERR("read err\n");
        handleDisconnection();
        return;
    }

    /* 先取消读 */
    //this->disableReadeHandling();

    handleReadBytes();
}

void TcpConnection::handleReadBytes()
{
    LOG_INFO("default read handle， handle bufferz = %s \n", mInputBuffer.begin());
    mInputBuffer.retrieveAll();
}

void TcpConnection::handleWrite()
{
    LOG_INFO("default wirte handle\n");
    mOutBuffer.retrieveAll();
}

void TcpConnection::handleError()
{
    LOG_INFO("default error handle\n");
}

void TcpConnection::readCallback(void* arg)
{
    TcpConnection* tcpConnection = (TcpConnection*)arg;
    tcpConnection->handleRead();
}

void TcpConnection::writeCallback(void* arg)
{
    TcpConnection* tcpConnection = (TcpConnection*)arg;
    tcpConnection->handleWrite();
}

void TcpConnection::errorCallback(void* arg)
{
    TcpConnection* tcpConnection = (TcpConnection*)arg;
    tcpConnection->handleError();
}

void TcpConnection::handleDisconnection()
{
    if(mDisconnectionCallback) {
        mDisconnectionCallback(mArg, mSocket.fd());
    }
}