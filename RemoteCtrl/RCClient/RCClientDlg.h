#ifndef _RC_CLIENT_DLG_H_
#define _RC_CLIENT_DLG_H_

#include <string>
#include "Packet.h"
#include "Util.h"

class RCClientDlg : public CDialogEx
{
    typedef void (RCClientDlg::* RC_DLG_RESPONSE_FUNC)(Packet* packet);
private:
    CTreeCtrl treeDir;
    CListCtrl listFile;
    DWORD dwIP;
    CString cstrPort;
    RC_DLG_RESPONSE_FUNC respCtlFunc[RC_CMD_BUTT];
protected:
    HICON m_hIcon;

public:
    RCClientDlg(CWnd* pParent = nullptr);
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_REMOTECTRLCLIENTMFC_DIALOG };
#endif

protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);

private:
    void InitRespCtlFunc(void);
    void RespConnect(Packet* packet);
    void RespGetDirverInfo(Packet* packet);
    void RespGetDirInfo(Packet* packet);

    void UpdateFileInfo(const FILE_INFO* info, HTREEITEM hParent);
    void DeleteTreeDirChildItems(HTREEITEM hTree);
    CString GetPath(HTREEITEM hTree); //获取完整的路径
    void LoadFileInfo(void);
    void DealCommand(WORD nCmd, char* data);
    void Str2Tree(const char* data, CTreeCtrl& treeDir);

    void UpdateDownloadFile(const std::string& strData, FILE* pFile);
    void LoadCurrentFile();

protected:
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg void OnCancel();
    afx_msg void OnOK();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

private:
    /* 处理CTRL层发送过来的消息 */
    afx_msg LRESULT OnRespCtl(WPARAM wParam, LPARAM lParam);
    afx_msg void OnBnLinkClicked();
    afx_msg void OnBnGetDiskClicked();
    afx_msg void OnNMDblclkTreeDir(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnNMRClickListFile(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDeleteFile();
    afx_msg void OnRunFile();
    afx_msg void OnIpnFieldchange(NMHDR* pNMHDR, LRESULT* pResult);
};

#endif /*_RC_CLIENT_DLG_H_ */
