#include "pch.h"
#include <direct.h>
#include <atlimage.h>
#include <afx.h>
#include <io.h>
#include "Util.h"
#include "Command.h"
#include "resource.h"
#include "Packet.h"

#define RC_UNLOCK_NUM 0x1b

CCommand* CCommand::instance()
{
    static CCommand* pInstance = new CCommand;
    return pInstance;
}
CCommand::CCommand() : threadId(0)
{
    struct {
        int nCmd;
        CMD_FUNC fun;

    }data[] = {
        {RC_CONNECT,&CCommand::ConnectTest},
        {RC_GET_DRIVER_INFO,&CCommand::GetDriveInfo},
        {RC_GET_DIR_AND_FILE,&CCommand::GetDirInfo},
        {RC_RUN_FILE,&CCommand::RunFile},
        {-1,NULL}
    };
    for (size_t i = 0; data[i].nCmd != -1; ++i) {
        umCmdFun.insert({ data[i].nCmd, data[i].fun });
    }
}

CCommand::~CCommand()
{
}

int CCommand::ExecuteCommand(int nCmd, std::list<Packet>& lstPacket, Packet& inPacket)
{
    return (this->*umCmdFun[nCmd])(lstPacket, inPacket);
}

void CCommand::RunCommand(void* arg, int sCmd, std::list<Packet>& lstPacket, Packet& inPacket)
{
    CCommand* thiz = (CCommand*)arg;
    int ret = thiz->ExecuteCommand(sCmd, lstPacket, inPacket);
    if (ret != 0) {
        LOG(ERR, "执行命令失败：%d ret=%d\r\n", sCmd, ret);
    }
}

void CCommand::ProcPacket(Packet* packet, std::list<Packet>& lstPacket)
{
    CCommand::instance()->ExecuteCommand(packet->cmd, lstPacket, *packet);
}

int CCommand::ConnectTest(std::list<Packet>& lstPacket, Packet& inPacket)
{
    uint32_t ret = RC_SERVER_OK;
    lstPacket.emplace_back(inPacket, (char*)&ret, sizeof(ret));
    return 0;
}

int CCommand::GetDriveInfo(std::list<Packet>& lstPacket, Packet& inPacket)
{
    DWORD drivesBitmask = GetLogicalDrives();
    std::string result;
    for (int i = 0; i < 26; ++i) {
        if (drivesBitmask & (1 << i)) {
            if (result.size() > 0) {
                result += ',';
            }
            result += 'A' + i;
        }
    }
    lstPacket.emplace_back(inPacket, (char*)result.c_str(), result.size() + 1);
    return 0;
}


int CCommand::GetDirInfo(std::list<Packet>& lstPacket, Packet& inPacket)
{
    FILE_INFO info;
    char* path = inPacket.data;
    if (_chdir(path) == -1) {
        lstPacket.emplace_back(inPacket, (char*)&info, sizeof(info));
        return RC_ERR;
    }

    _finddata_t fdata;
    intptr_t hfind = 0;
    hfind = _findfirst("*", &fdata);
    if (hfind <= -1) {
        return RC_ERR;
    }

    int ncount = 0;
    do {
        if (!DirNameFilter(fdata.name)) {
            continue;
        }
        info.isDir = (fdata.attrib & _A_SUBDIR) != 0;
        info.hasNext = true;
        memcpy(info.name, fdata.name, strlen(fdata.name) + 1);
        lstPacket.emplace_back(inPacket, (char*)&info, sizeof(info));
        ++ncount;
    } while (!_findnext(hfind, &fdata));

    info.isDir = false;
    info.hasNext = false;
    lstPacket.emplace_back(inPacket, (char*)&info, sizeof(info));

    return 0;
}

int CCommand::RunFile(std::list<Packet>& lstPacket, Packet& inPacket)
{
    ShellExecuteA(NULL, NULL, inPacket.data, NULL, NULL, SW_SHOWNORMAL);
    uint32_t ret = RC_RUN_FILE;
    lstPacket.emplace_back(RC_RUN_FILE, (char*)&ret, sizeof(ret));
    return 0;
}

int DownloadFile(std::list<Packet>& lstPacket, Packet& inPacket)
{
    int64_t data = 0;
    FILE* pFile = fopen(inPacket.data, "rb");
    if (pFile == NULL) {
        lstPacket.emplace_back(RC_DOWNLOAD_FILE, (char*)&data, (uint32_t)sizeof(int64_t));
        return RC_ERR;
    }

    fseek(pFile, 0, SEEK_END);
    data = _ftelli64(pFile);
    lstPacket.emplace_back(RC_DOWNLOAD_FILE, (char*)&data, sizeof(int64_t));

    fseek(pFile, 0, SEEK_SET);

    std::vector<char> buf;
    buf.resize(RC_SEND_BUF_SIZE);
    char* pBuf = buf.data();
    size_t rLen = 0;
    do {
        rLen = fread(pBuf, 1, RC_SEND_BUF_SIZE, pFile);
        lstPacket.emplace_back(RC_DOWNLOAD_FILE, (char*)pBuf, rLen);
    } while (rLen >= RC_SEND_BUF_SIZE);
    fclose(pFile);

    return RC_OK;
}

int CCommand::MouseEvent(std::list<Packet>& lstPacket, Packet& inPacket)
{
    MOUSEEV mouse;
    memcpy(&mouse, inPacket.data, sizeof(MOUSEEV));
    DWORD nFlags = 0;
    switch (mouse.nButton) {
        case 0:  //左键
            nFlags = 1;
            break;
        case 1:  //右键
            nFlags = 2;
            break;
        case 2:  //中键
            nFlags = 4;
            break;
        case 3:  //没有按键
            nFlags = 8;
            break;
        default:
            break;
    }
    if (nFlags != 8) {
        SetCursorPos(mouse.ptxy.x, mouse.ptxy.y);
    }

    switch (mouse.nAction) {
        case 0:  //单击
            nFlags |= 0x10;
            break;
        case 1:  //双击
            nFlags |= 0x20;
            break;
        case 2:  //按下
            nFlags |= 0x40;
            break;
        case 3:  //释放
            nFlags |= 0x80;
            break;
        default:
            break;
    }
    LOG(INFO, "nFlags = %d, x = %d, y = %d \r\n", nFlags, mouse.ptxy.x, mouse.ptxy.y);
    switch (nFlags)
    {
        case 0x21: //左键双击
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, GetMessageExtraInfo());
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, GetMessageExtraInfo());
        case 0x11: //左键单击
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, GetMessageExtraInfo());
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, GetMessageExtraInfo());
            break;
        case 0x41: //左按下
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, GetMessageExtraInfo());
            break;
        case 0x81: //左键释放
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, GetMessageExtraInfo());
            break;
        case 0x22: //右键双击
            mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, GetMessageExtraInfo());
            mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, GetMessageExtraInfo());
        case 0x12://右键单击
            mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, GetMessageExtraInfo());
            mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, GetMessageExtraInfo());
            break;
        case 0x42:
            mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, GetMessageExtraInfo());
            break;
        case 0x82:
            mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, GetMessageExtraInfo());
            break;
        case 0x24:
            mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, GetMessageExtraInfo());
            mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, GetMessageExtraInfo());
        case 0x14:
            mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, GetMessageExtraInfo());
            mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, GetMessageExtraInfo());
            break;
        case 0x44:
            mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, GetMessageExtraInfo());
            break;
        case 0x84:
            mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, GetMessageExtraInfo());
            break;
        case 0x08:
            mouse_event(MOUSEEVENTF_MOVE, mouse.ptxy.x, mouse.ptxy.y, 0, GetMessageExtraInfo());
            break;
        default:
            break;
    }
    return 0;
}

int CCommand::DeleteFile(std::list<Packet>& lstPacket, Packet& inPacket)
{
    std::string strPath = inPacket.data;
    int ret = -1;
    if (remove(strPath.c_str()) == 0) {
        ret = 0;
    }
    lstPacket.emplace_back(RC_DELETE_FILE, (char*)&ret, sizeof(int));

    return ret;
}

bool CCommand::DirNameFilter(char* name)
{
    string strName(name);

    if (strName == "." || (strName) == "..") {
        return false;
    }
    return true;
}
