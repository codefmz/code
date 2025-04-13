#include "acceptor.h"
#include "socketops.h"
#include "log.h"
Acceptor* Acceptor::createNew(Poller* poller, const Ipv4Address& addr)
{
    return new Acceptor(poller, addr);
}

Acceptor::Acceptor(Poller* poller, const Ipv4Address& addr) : mPoller(poller), mAddr(addr),
    mSocket(sockets::createTcpSock()), mNewConnectionCallback(nullptr)
{
    mSocket.setReuseAddr(1);
    mSocket.bind(mAddr);
    mAcceptIOEvent = IOEvent::createNew(mSocket.fd(), this);
    mAcceptIOEvent->setReadCallback(readCallback);
    mAcceptIOEvent->enableReadHandling();
}

Acceptor::~Acceptor()
{
    if(mListenning) {
        mPoller->removeIOEvent(mAcceptIOEvent);
    }

    delete mAcceptIOEvent;
}

void Acceptor::listen()
{
    mListenning = true;
    mSocket.listen(1024);
    mPoller->addIOEvent(mAcceptIOEvent);
}

void Acceptor::setNewConnectionCallback(NewConnectionCallback cb, void* arg)
{
    mNewConnectionCallback = cb;
    mArg = arg;
}

void Acceptor::readCallback(void* arg)
{
    Acceptor* acceptor = (Acceptor*)arg;
    acceptor->handleRead();
}

void Acceptor::handleRead()
{
    int connfd = mSocket.accept();
    LOG_INFO("client connect: %d\n", connfd);
    if(mNewConnectionCallback) {
        mNewConnectionCallback(mArg, connfd);
    }
}
