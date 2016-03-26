
// HttpSourceViewerDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "TabHtmlHead.h"
#include "TabHtmlBody.h"
#include "TabHtmlGet.h"

#include "ThirdParty/ATLRegExp/WinHttpClient.h"

// CHttpSourceViewerDlg 对话框
class CHttpSourceViewerDlg : public CDialogEx
{
// 构造
public:
	CHttpSourceViewerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HTTPSOURCEVIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editURL;
	CTabCtrl m_tabMain;
	void InitTabControl();

	CTabHtmlHead m_tabHtmlHead;
	CTabHtmlBody m_tabHtmlBody;
	CTabHtmlGet m_tabHtmlGet;
	void MoveSubTab();
	afx_msg void OnTcnSelchangeTab1Main(NMHDR *pNMHDR, LRESULT *pResult);
	CProgressCtrl m_urlProgress;
	afx_msg void OnBnClickedButton1();
	static bool ProgressProc(double progress);
	static CHttpSourceViewerDlg* pThis;
};
