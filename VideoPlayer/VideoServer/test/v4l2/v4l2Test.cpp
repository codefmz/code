#include "gtest/gtest.h"
#include "Log.h"
#include "ThreadPool.h"
#include "Scheduler.h"
#include "Event.h"
#include "RtspServer.h"
#include "MediaSession.h"
#include "Address.h"
#include "H264FileMediaSource.h"
#include "H264RtpSink.h"
#include "V4l2MediaSource.h"

class v4l2Test : public ::testing::Test {
public:
    v4l2Test() {
    }
    ~v4l2Test() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST(v4l2Test, h264Server)
{
    /*1. Epoll 管理4个文件描述符
    * (1). eventFd
    * (2). timerFd
    * (3). acceptor poller 创建 connect event
    * (4). connect 后会创建一个 read write event
    */

    //问题： 1. 控制连接与数据连接
    EventScheduler* scheduler = EventScheduler::createNew(EventScheduler::POLLER_SELECT);
    ThreadPool* threadPool = ThreadPool::createNew(2);

    Ipv4Address ipAddr("0.0.0.0", 8554);

    RtspServer* server = RtspServer::createNew(scheduler, ipAddr);
    MediaSession* session = MediaSession::createNew("live");
    //1. 两个线程从 test.h264 中读取文件数据放到缓冲队列, 4个任务
    //2. scheduler 定时器任务， 从缓冲区读取数据，发送数据，新加任务
    // MediaSource* mediaSource = H264FileMediaSource::createNew(threadPool, "test.h264");
    MediaSource* videoSource = V4l2MediaSource::createNew(threadPool, "/dev/video0");
    RtpSink* rtpSink = H264RtpSink::createNew(scheduler, videoSource);

    session->addRtpSink(TrackId0, rtpSink);
    //session->startMulticast(); //多播

    server->addMeidaSession(session);
    server->start();

    std::cout<<"Play the media using the URL \"" << server->getUrl(session) << "\"" <<std::endl;

    scheduler->loop();
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}