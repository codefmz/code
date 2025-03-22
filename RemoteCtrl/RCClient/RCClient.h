#ifndef _RC_CLIENT_H_
#define _RC_CLIENT_H_

#ifndef __AFXWIN_H__
#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

class RCClientApp : public CWinApp
{
public:
    RCClientApp();

public:
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
    virtual int ExitInstance();
private:
    ULONG_PTR gdiplusToken;
};

#endif //_RC_CLIENT_H_
