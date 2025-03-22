/*
* 该文件定义一些基础的宏与枚举类型
*/
#ifndef _RC_BASE_H_
#define _RC_BASE_H_

#include <WinUser.h>

/* 错误码 */
#define RC_ERR -1
#define RC_OK 0

#define RC_UINT16_INVALID_VAL (uint16_t)(-1)
#define RC_UINT32_INVALID_VAL (uint32_t)(-1)
#define RC_UINT64_INVALID_VAL (uint64_t)(-1)

#define RC_RECV_BUF_SIZE 4096                 //4k
#define RC_SEND_BUF_SIZE 1024                 //1k
#define WM_SHOW_STATUS (WM_USER + 3)          //展示状态
#define WM_SHOW_WATCH (WM_USER + 4)           //远程监控
#define WM_SEND_MESSAGE (WM_USER + 0x1000)    //自定义消息处理
#define WM_RESPONSE_PACK (WM_USER + 0x1001)   //自定义消息处理

typedef enum {
    RC_SERVER_OK = 0,
    RC_SERVER_ERR
}RC_SERVER_RET;

typedef enum {
    RC_SOCKET_WAIT = 0,
    RC_SOCKET_CREATE,
    RC_SOCKET_CONNECT,
    RC_SOCKET_CLOSE,
    RC_SOCKET_ACCEPTED
} RC_SOCKET_STATE;

typedef enum {
    RC_CONNECT = 0,
    RC_GET_DRIVER_INFO,
    RC_GET_DIR_AND_FILE,
    RC_RUN_FILE,
    RC_DOWNLOAD_FILE,
    RC_DELETE_FILE,
    RC_MOUSE_EVENT,
    RC_SHOW_SCREEN,
    RC_LOCK_MACHINE,
    RC_UNLOCK_MACHINE,
    RC_CMD_BUTT
}RC_CMD;

#endif /* _RC_BASE_H_ */
