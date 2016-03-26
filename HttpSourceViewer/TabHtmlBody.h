#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CTabHtmlBody �Ի���

class CTabHtmlBody : public CDialogEx
{
	DECLARE_DYNAMIC(CTabHtmlBody)

public:
	CTabHtmlBody(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTabHtmlBody();

// �Ի�������
	enum { IDD = IDD_TAB_HTML_BODY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSearch();

	virtual BOOL OnInitDialog();

	bool InitRichEdit();

	void FindTextInHtml();

	int MarkSource(int start, int end);

	static long SubEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static void ShowLineNum(HWND hEdit);

	void InitRegComboBoxStyle();

public:
	CEdit m_searchInput;
	CRichEditCtrl m_editHtmlBody;
	CComboBox m_cbRegStyle;
};
