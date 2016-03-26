
// HttpSourceViewerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "TabHtmlHead.h"
#include "TabHtmlBody.h"
#include "TabHtmlGet.h"

#include "ThirdParty/ATLRegExp/WinHttpClient.h"

// CHttpSourceViewerDlg �Ի���
class CHttpSourceViewerDlg : public CDialogEx
{
// ����
public:
	CHttpSourceViewerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_HTTPSOURCEVIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
