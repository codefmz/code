#include "pch.h"
#include "framework.h"
#include "Util.h"
#include "Command.h"
#include "resource.h"
#include "Server.h"
#include "Network.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 唯一的应用程序对象
//CWinApp theApp;

int main()
{

    bool ret;
    ret = Util::Init();
    if (!ret) {
        LOG(ERR, "init mfc fail.");
        return -1;
    }

    ret = Network::Initialize();
    if (!ret) {
        LOG(ERR, "Net work init fail.");
        return -1;
    }
    Server::instance()->Create(9527, CCommand::ProcPacket);

    while (true) {
        Sleep(5 * 1000);
    }
    return 1;
}
