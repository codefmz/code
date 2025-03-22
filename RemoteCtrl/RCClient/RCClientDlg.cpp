#include "pch.h"
#include "afxdialogex.h"
#include "framework.h"
#include "RCClient.h"
#include "RCClientDlg.h"
#include "RCControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


RCClientDlg::RCClientDlg(CWnd* pParent)
    : CDialogEx(IDD_REMOTECTRLCLIENTMFC_DIALOG, pParent)
    , cstrPort(_T(""))
    , dwIP(0)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BOOL RCClientDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // IDM_ABOUTBOX 必须在系统命令范围内。
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr) {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty()) {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    SetIcon(m_hIcon, TRUE);			// 设置大图标
    SetIcon(m_hIcon, FALSE);		// 设置小图标

    cstrPort = _T("9527");
    dwIP = 0xC0A88901;
    UpdateData(FALSE);
    RCControl::instance()->UpdateAddress(dwIP, _ttoi(cstrPort));
    int ret = RCControl::instance()->StartRecv();
    if (ret != RC_OK) {
        LOG(ERR, " start recv fail.");
        return FALSE;
    }
    InitRespCtlFunc();

    return TRUE;
}

void RCClientDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_IPAddress(pDX, IDC_IPADDRESS1, dwIP);
    DDX_Text(pDX, IDC_EDIT1, cstrPort);
    DDX_Control(pDX, IDC_TREE_DIR, treeDir);
    DDX_Control(pDX, IDC_LIST_FILE, listFile);
}

BEGIN_MESSAGE_MAP(RCClientDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_LINK, &RCClientDlg::OnBnLinkClicked) /* 建联按钮 */
    ON_BN_CLICKED(IDC_GET_DISK, &RCClientDlg::OnBnGetDiskClicked) /* 获取磁盘内容 */
    ON_NOTIFY(NM_DBLCLK, IDC_TREE_DIR, &RCClientDlg::OnNMDblclkTreeDir)
    ON_NOTIFY(NM_RCLICK, IDC_LIST_FILE, &RCClientDlg::OnNMRClickListFile)
    ON_COMMAND(ID_32772, &RCClientDlg::OnDeleteFile)
    ON_COMMAND(ID_32773, &RCClientDlg::OnRunFile)
    ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS1, &RCClientDlg::OnIpnFieldchange)
    ON_MESSAGE(WM_RESPONSE_PACK, &RCClientDlg::OnRespCtl)
END_MESSAGE_MAP()

/* 响应系统命令，关闭，最大化，最小化，移动等操作 */
void RCClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    } else {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

void RCClientDlg::OnCancel()
{
    RCControl::instance()->StopRecv();
    CDialog::OnCancel();
}

void RCClientDlg::OnOK()
{
    RCControl::instance()->StopRecv();
    CDialog::OnOK();
}

void RCClientDlg::OnPaint()
{
    if (IsIconic()) {
        CPaintDC dc(this); // 用于绘制的设备上下文

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
    } else {
        CDialogEx::OnPaint();
    }
}

HCURSOR RCClientDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void RCClientDlg::InitRespCtlFunc(void)
{
    respCtlFunc[RC_CONNECT] = &RCClientDlg::RespConnect;
    respCtlFunc[RC_GET_DRIVER_INFO] = &RCClientDlg::RespGetDirverInfo;
    respCtlFunc[RC_GET_DIR_AND_FILE] = &RCClientDlg::RespGetDirInfo;
}

void RCClientDlg::RespConnect(Packet* packet)
{
    if (*(RC_SERVER_RET*)packet->data == RC_SERVER_OK) {
        MessageBox(_T("连接测试"), _T("连接成功"), MB_ICONINFORMATION);
    } else {
        MessageBox(_T("连接测试"), _T("连接失败"), MB_ICONERROR);
    }
}

void RCClientDlg::RespGetDirverInfo(Packet* packet)
{
    Str2Tree(packet->data, treeDir);
}

void RCClientDlg::RespGetDirInfo(Packet* packet)
{
    UpdateFileInfo((const FILE_INFO*)packet->data, (HTREEITEM)packet->param);
}

LRESULT RCClientDlg::OnRespCtl(WPARAM wParam, LPARAM lParam)
{
    (void)lParam;
    if (wParam != NULL) {
        Packet* packet = (Packet*)wParam;
        if (packet->cmd >= RC_CMD_BUTT) {
            return -1;
        }
        (this->*respCtlFunc[packet->cmd])(packet);
    }
    return LRESULT();
}

void RCClientDlg::DealCommand(WORD nCmd, char* data)
{
    switch (nCmd)
    {
        case RC_GET_DIR_AND_FILE://获取文件信息

            break;
        case RC_RUN_FILE:
            MessageBox(_T("打开文件完成！"), _T("操作完成"), MB_ICONINFORMATION);
            break;
        case RC_DOWNLOAD_FILE:
            //UpdateDownloadFile(strData, (FILE*)lParam);
            break;
        case RC_DELETE_FILE:
            //LoadCurrentFile();
            MessageBox(_T("删除文件完成！"), _T("操作完成"), MB_ICONINFORMATION);
            break;
        default:
            TRACE("unknow data received! %d\r\n", nCmd);
            break;
    }
}

void RCClientDlg::OnBnLinkClicked()
{
    bool ret = RCControl::instance()->StartConnect();
    if (!ret) {
        MessageBox(_T("连接测试"), _T("连接失败"), MB_ICONERROR);
        return;
    }
    Packet packet(RC_CONNECT, 0, NULL, (uint64_t)(uintptr_t)GetSafeHwnd());
    RCControl::instance()->SendPacket(&packet);
}

void RCClientDlg::OnBnGetDiskClicked()
{
    Packet packet(RC_GET_DRIVER_INFO, 0, NULL, (uint64_t)(uintptr_t)GetSafeHwnd());
    RCControl::instance()->SendPacket(&packet);
}

void RCClientDlg::DeleteTreeDirChildItems(HTREEITEM hTree)
{
    HTREEITEM hSub = NULL;
    do {
        hSub = treeDir.GetChildItem(hTree);
        if (hSub != NULL) {
            treeDir.DeleteItem(hSub);
        }
    } while (hSub != NULL);
}

CString RCClientDlg::GetPath(HTREEITEM hTree)
{
    CString strRet, strTmp;
    do {
        strTmp = treeDir.GetItemText(hTree);
        strRet = strTmp + '\\' + strRet;
        hTree = treeDir.GetParentItem(hTree);
    } while (hTree != NULL);
    return strRet;
}

void RCClientDlg::LoadFileInfo()
{
    CPoint ptMouse;
    GetCursorPos(&ptMouse);
    treeDir.ScreenToClient(&ptMouse);
    HTREEITEM hTreeSelected = treeDir.HitTest(ptMouse, 0);
    if (hTreeSelected == NULL) {
        return;
    }
    DeleteTreeDirChildItems(hTreeSelected);
    listFile.DeleteAllItems();
    CString strPath = GetPath(hTreeSelected);
    Packet packet(RC_GET_DIR_AND_FILE, (char*)strPath.GetString(),
        strPath.GetLength() + 1, (uint64_t)(uintptr_t)GetSafeHwnd(), (uint64_t)(uintptr_t)hTreeSelected);
    RCControl::instance()->SendPacket(&packet);
}

void RCClientDlg::Str2Tree(const char* data, CTreeCtrl& treeDir)
{
    treeDir.DeleteAllItems();
    std::string dr;
    std::string drivers(data);
    int len = drivers.size();
    for (size_t i = 0; i <= len; ++i) {
        if (i == len || drivers[i] == ',') {
            dr += ":";
            treeDir.InsertItem(dr.c_str(), TVI_ROOT, TVI_LAST);
            dr.clear();
            continue;
        }
        dr += drivers[i];
    }
}

void RCClientDlg::UpdateFileInfo(const FILE_INFO* info, HTREEITEM hParent)
{
    if (info->hasNext == FALSE) {
        return;
    }
    if (info->isDir) {
        HTREEITEM htemp = treeDir.InsertItem(info->name, hParent, TVI_LAST);
        treeDir.InsertItem("", htemp, TVI_LAST);
        treeDir.Expand(hParent, TVE_EXPAND);
    } else {
        listFile.InsertItem(0, info->name);
    }
}

void RCClientDlg::UpdateDownloadFile(const std::string& strData, FILE* pFile)
{
    static int64_t alreadyReadLen = 0, totalLen = 0;
    if (totalLen == 0) {
        totalLen = *(int64_t*)strData.c_str();
        if (totalLen == 0) {
            AfxMessageBox(_T("文件长度为零或者无法读取文件！！！"));
        }
    } else {
        fwrite(strData.c_str(), 1, strData.size(), pFile);
        alreadyReadLen += strData.size();
        if (alreadyReadLen >= totalLen) {
            fclose(pFile);
            alreadyReadLen = 0;
            totalLen = 0;
            LoadCurrentFile();
        }
    }
}

void RCClientDlg::LoadCurrentFile()
{
    HTREEITEM hTree = treeDir.GetSelectedItem();
    CString strPath = GetPath(hTree);
    listFile.DeleteAllItems();

    //CController::instance()->SendPacket(GetSafeHwnd(), 2, (const char*)strPath.GetString(), strPath.GetLength(), (WPARAM)hTree);
}

void RCClientDlg::OnNMDblclkTreeDir(NMHDR* pNMHDR, LRESULT* pResult)
{
    (void)pNMHDR;
    *pResult = 0;
    LoadFileInfo();
}

void RCClientDlg::OnNMRClickListFile(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    *pResult = 0;
    CPoint ptMouse, ptList;
    GetCursorPos(&ptMouse);
    ptList = ptMouse;
    listFile.ScreenToClient(&ptList);
    int ListSelected = listFile.HitTest(ptList);
    if (ListSelected < 0) {
        return;
    }
    CMenu menu;
    menu.LoadMenu(IDR_MENU_FILE);
    CMenu* pPupup = menu.GetSubMenu(0);
    if (pPupup != NULL) {
        pPupup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptMouse.x, ptMouse.y, this);
    }
}

void RCClientDlg::OnDeleteFile()
{
    int nListSelected = listFile.GetSelectionMark();
    CString fileName = listFile.GetItemText(nListSelected, 0);
    HTREEITEM hSelected = treeDir.GetSelectedItem();
    CString filePath = GetPath(hSelected) + fileName;

    //CController::instance()->SendPacket(GetSafeHwnd(), RC_DELETE_FILE, (const char*)filePath.GetString(), filePath.GetLength(), (WPARAM)hSelected);
}

void RCClientDlg::OnRunFile()
{
    int nListSelected = listFile.GetSelectionMark();
    CString fileName = listFile.GetItemText(nListSelected, 0);
    HTREEITEM hSelected = treeDir.GetSelectedItem();
    CString filePath = GetPath(hSelected) + fileName;
    //CController::instance()->SendPacket(GetSafeHwnd(), 3, (const char*)filePath.GetString(), filePath.GetLength(), (WPARAM)hSelected);
}

void RCClientDlg::OnIpnFieldchange(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
    *pResult = 0;
    UpdateData(TRUE);
    RCControl::instance()->UpdateAddress(dwIP, _ttoi(cstrPort));
}
