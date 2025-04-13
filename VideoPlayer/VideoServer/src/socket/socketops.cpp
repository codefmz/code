#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <string.h>
#include "log.h"
#include "socketops.h"

const std::string INVALID_ADDR = "0.0.0.0";

int sockets::createTcpSock()
{
    //SOCK_CLOEXEC 保证不会被子进程继承，exec
    return ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
}

int sockets::createUdpSock()
{
    return ::socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
}

bool sockets::bind(int sockfd, std::string ip, uint16_t port)
{
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);

    if(::bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        return false;
    }

    return true;
}

bool sockets::listen(int sockfd, int backlog)
{
    if(::listen(sockfd, backlog) < 0) {
        return false;
    }

    return true;
}

int sockets::accept(int sockfd)
{
    struct sockaddr_in addr = { 0 };
    socklen_t addrlen = sizeof(struct sockaddr_in);

    int connfd = ::accept(sockfd, (struct sockaddr*)&addr, &addrlen);
    setNonBlockAndCloseOnExec(connfd);
    ignoreSigPipeOnSocket(connfd);

    return connfd;
}

int sockets::readv(int sockfd, const struct iovec *iov, int iovcnt)
{
    //iovec 是一个包含缓冲区的结构体
    return ::readv(sockfd, iov, iovcnt);
}

int sockets::write(int sockfd, const void* buf, int size)
{
    return ::write(sockfd, buf, size);
}

int sockets::sendto(int sockfd, const void* buf, int len, const struct sockaddr *destAddr)
{
    socklen_t addrLen = sizeof(struct sockaddr);
    //中间0表示：发送操作的标志，可以是零或多个标志的按位或组合
    return ::sendto(sockfd, buf, len, 0, destAddr, addrLen);
}

void sockets::setNonBlock(int sockfd)
{
    int flags = ::fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        LOG_ERR("get fcntl error, errno = %d", errno);
        return;
    }

    flags |= (O_NONBLOCK);
    int ret = ::fcntl(sockfd, F_SETFD, flags);
    if (ret == -1) {
        LOG_ERR("set fcntl error, errno = %d", errno);
        return;
    }
}

void sockets::setBlock(int sockfd, int writeTimeout)
{
    int flags = ::fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        LOG_ERR("get fcntl error, errno = %d", errno);
        return;
    }

    flags = flags & (~O_NONBLOCK);
    int ret = ::fcntl(sockfd, F_SETFD, flags);
    if (ret == -1) {
        LOG_ERR("set fcntl error, errno = %d", errno);
        return;
    }

    if (writeTimeout > 0) {
        struct timeval tv = {writeTimeout / 1000, (writeTimeout % 1000) * 1000};
        //设置套接字的发送超时时间
        setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, sizeof(tv));
    }
}

void sockets::setReuseAddr(int sockfd, int on)
{
    int optval = on ? 1 : 0;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval));
}

void sockets::setReusePort(int sockfd)
{
#ifdef SO_REUSEPORT
    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, (const char*)&on, sizeof(on));
#endif
}

void sockets::setNonBlockAndCloseOnExec(int sockfd)
{
    int flags = ::fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        LOG_ERR("get fcntl error, errno = %d", errno);
        return;
    }

    flags |= (O_NONBLOCK | O_CLOEXEC);
    int ret = ::fcntl(sockfd, F_SETFD, flags);
    if (ret == -1) {
        LOG_ERR("set fcntl error, errno = %d", errno);
        return;
    }
}

void sockets::ignoreSigPipeOnSocket(int socketfd)
{
    int option = 1;
    /* 应用向一个已经关闭的连接发送数据时，会收到SIGPIPE信号，忽略该信号 */
    setsockopt(socketfd, SOL_SOCKET, MSG_NOSIGNAL, &option, sizeof(option));
}

void sockets::setNoDelay(int sockfd)
{
#ifdef TCP_NODELAY
    int on = 1;
    int ret = setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *)&on, sizeof(on));
#endif
}

void sockets::setKeepAlive(int sockfd)
{
    /* 选项用于启用或禁用套接字的保持活动（keep-alive）功能。
       保持活动功能用于检测对端是否仍然活跃，如果对端长时间没有响应，
       系统会自动发送探测包以确认连接是否仍然有效。*/
    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof(on));
}

void sockets::setNoSigpipe(int sockfd)
{
#ifdef SO_NOSIGPIPE
    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_NOSIGPIPE, (char *)&on, sizeof(on));
#endif
}

void sockets::setSendBufSize(int sockfd, int size)
{
    setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (char *)&size, sizeof(size));
}

void sockets::setRecvBufSize(int sockfd, int size)
{
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char *)&size, sizeof(size));
}

std::string sockets::getPeerIp(int sockfd)
{
    struct sockaddr_in addr = { 0 };
    socklen_t addrlen = sizeof(struct sockaddr_in);
    if (getpeername(sockfd, (struct sockaddr *)&addr, &addrlen) == 0) {
        return inet_ntoa(addr.sin_addr);
    }

    return INVALID_ADDR;
}

int16_t sockets::getPeerPort(int sockfd)
{
    struct sockaddr_in addr = { 0 };
    socklen_t addrlen = sizeof(struct sockaddr_in);
    if (getpeername(sockfd, (struct sockaddr *)&addr, &addrlen) == 0) {
        return ntohs(addr.sin_port);
    }

    return 0;
}

int sockets::getPeerAddr(int sockfd, struct sockaddr_in *addr)
{
    socklen_t addrlen = sizeof(struct sockaddr_in);
    return getpeername(sockfd, (struct sockaddr *)addr, &addrlen);
}

void sockets::close(int sockfd)
{
    ::close(sockfd);
}

bool sockets::connect(int sockfd, std::string ip, uint16_t port, int timeout)
{
    bool isConnected = true;
    if (timeout > 0) {
        sockets::setNonBlock(sockfd);
    }

    struct sockaddr_in addr = { 0 };
    socklen_t addrlen = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    if (::connect(sockfd, (struct sockaddr*)&addr, addrlen) < 0) {
        if (timeout > 0) {
            isConnected = false;
            //使用select实现超时阻塞
            fd_set fdWrite;
            FD_ZERO(&fdWrite);
            FD_SET(sockfd, &fdWrite);
            struct timeval tv = { timeout / 1000, timeout % 1000 * 1000 };
            select(sockfd + 1, NULL, &fdWrite, NULL, &tv);
            if (FD_ISSET(sockfd, &fdWrite)) {
                isConnected = true;
            }
            sockets::setBlock(sockfd, 0);
        } else {
            isConnected = false;
        }
    }

    return isConnected;
}

std::string sockets::getLocalIp()
{
    int sockfd = 0;
    char buf[512] = { 0 };
    struct ifconf ifconf;
    struct ifreq *ifreq;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        close(sockfd);
        return INVALID_ADDR;
    }

    ifconf.ifc_len = 512;
    ifconf.ifc_buf = buf;
    if (ioctl(sockfd, SIOCGIFCONF, &ifconf) < 0) {
        close(sockfd);
        return INVALID_ADDR;
    }

    close(sockfd);
    ifreq = (struct ifreq*)ifconf.ifc_buf;
    for (int i = (ifconf.ifc_len / sizeof(struct ifreq)); i>0; i--) {
        if (ifreq->ifr_flags == AF_INET) {
            if (strcmp(ifreq->ifr_name, "lo") != 0) {
                return inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr);
            }
            ifreq++;
        }
    }

    return INVALID_ADDR;
}