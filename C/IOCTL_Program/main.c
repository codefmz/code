#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    int fd;
    fd = open("/dev/test", O_RDWR); // 我们的设备挂载在/dev/test处
    if (fd < 0) {
        perror("Failed to open the device...");
        return errno;
    } else {
        printf("Open device successful!\n");
    }

    switch (argc) {
    case 0:
        ioctl(fd, 0);
        break;
    case 1:
        ioctl(fd, 1, "hello world!");
        break;
    }
}