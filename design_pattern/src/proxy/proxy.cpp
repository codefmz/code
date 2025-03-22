#include "proxy.h"
#include "gtest/gtest.h"

class proxyTest : public ::testing::Test {
protected:
    proxyTest() {
    }
    ~proxyTest() override {
    }
    void SetUp() override {
        pursuit = new Pursuit("jiaojiao", "lixiang");
        proxy = new Proxy("dailei", pursuit);
    }
    void TearDown() override {
        delete pursuit;
        delete proxy;
    }

    Proxy *proxy;
    Pursuit *pursuit;
};

TEST_F(proxyTest, normal) {
    pursuit->GiveFlowers();
}

TEST_F(proxyTest, proxy_get) {
    proxy->GiveFlowers();
}
