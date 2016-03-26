#pragma once


// CTabHtmlGet 对话框

class CTabHtmlGet : public CDialogEx
{
	DECLARE_DYNAMIC(CTabHtmlGet)

public:
	CTabHtmlGet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTabHtmlGet();

// 对话框数据
	enum { IDD = IDD_TAB_HTML_GET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
