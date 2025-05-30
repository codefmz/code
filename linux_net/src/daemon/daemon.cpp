#include "gtest/gtest.h"
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <syslog.h>

class daemonTest : public ::testing::Test {
public:
    daemonTest() {
        std::cout << "daemonTest" << std::endl;
    }
    ~daemonTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};


 #define MAXFD 64
void daemon_init(const char *pname, int facility) {
    int i:
    pid_t pid;
    /* fork,终止父进程 */
    if (pid=fork()) {
        exit(0);
    }

    /* 第一子进程 */
    setsid();
    signal(SIGHUP,SIG_IGN);

    /* fork,终止第一子进程 */
    if (pid=fork()) {
        exit(0);
    }

    /* 第二子进程 */
    daemon_proc=1;
    /* 将工作目录设定为"/" */
    chdir("/");
    /* 清除文件掩码 */
    umask(0);
    /* 关闭所有文件句柄 */
    for (i=0;i<MAXFD;i++) {
        close(i);
    }
    /* 打开 log */
    openlog(pname,LOG_PID,facility);
 }

TEST_F(daemonTest, test1) {
    printf("hello world!");

    EXPECT_EQ(1, 1);
}