#include "gtest/gtest.h"
#include <log.h>
#include "socketops.h"
#include "tcpsocket.h"
#include "address.h"
#include "event.h"
#include "tcpserver.h"
#include "selectpoller.h"
#include "epollpoller.h"

class socketTest : public ::testing::Test {
public:
    socketTest() {
    }
    ~socketTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};


TEST_F(socketTest, selectserver)
{
    Ipv4Address ipAddr("0.0.0.0", 8554);
    Poller* poller = SelectPoller::createNew();
    TcpServer* server = new TcpServer(poller, ipAddr);
    server->start();

    while (true) {
        poller->handleEvent();
    }

    delete server;
    delete poller;
}

TEST_F(socketTest, epollserver)
{
    Ipv4Address ipAddr("0.0.0.0", 8554);
    Poller* poller = EPollPoller::createNew();
    TcpServer* server = new TcpServer(poller, ipAddr);
    server->start();

    while (true) {
        poller->handleEvent();
    }

    delete server;
    delete poller;
}


TEST_F(socketTest, client)
{
    Ipv4Address ipAddr("127.0.0.1", 8554);
    int sockfd = sockets::createTcpSock();

    sockets::connect(sockfd, ipAddr.getIp(), ipAddr.getPort(), 1000);
    sockets::write(sockfd, "hello", 5);
    sockets::close(sockfd);
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}