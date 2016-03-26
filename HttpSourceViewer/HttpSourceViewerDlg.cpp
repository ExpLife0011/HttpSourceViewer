
// HttpSourceViewerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HttpSourceViewer.h"
#include "HttpSourceViewerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHttpSourceViewerDlg 对话框

CHttpSourceViewerDlg* CHttpSourceViewerDlg::pThis = NULL;

CHttpSourceViewerDlg::CHttpSourceViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHttpSourceViewerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pThis = this;
}

void CHttpSourceViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editURL);
	DDX_Control(pDX, IDC_TAB1_MAIN, m_tabMain);
	DDX_Control(pDX, IDC_PROGRESS1, m_urlProgress);
}

BEGIN_MESSAGE_MAP(CHttpSourceViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1_MAIN, &CHttpSourceViewerDlg::OnTcnSelchangeTab1Main)
	ON_BN_CLICKED(IDC_BUTTON1, &CHttpSourceViewerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CHttpSourceViewerDlg 消息处理程序

BOOL CHttpSourceViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	InitTabControl();
	m_editURL.SetWindowText(_T("5566.net"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CHttpSourceViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHttpSourceViewerDlg::OnPaint()
{
	if (IsIconic())
	{
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
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHttpSourceViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHttpSourceViewerDlg::InitTabControl()
{
	m_tabMain.InsertItem(0, _T("HTML 头"));
	m_tabMain.InsertItem(1, _T("HTML 内容"));
	m_tabMain.InsertItem(2, _T("提取内容"));

	m_tabHtmlHead.Create(IDD_TAB_HTML_HEAD, GetDlgItem(IDC_TAB1_MAIN));
	m_tabHtmlBody.Create(IDD_TAB_HTML_BODY, GetDlgItem(IDC_TAB1_MAIN));
	m_tabHtmlGet.Create(IDD_TAB_HTML_GET, GetDlgItem(IDC_TAB1_MAIN));

	MoveSubTab();

	m_tabHtmlHead.ShowWindow(true);
	m_tabHtmlBody.ShowWindow(false);
	m_tabHtmlGet.ShowWindow(false);

	m_tabMain.SetCurSel(0);
}


void CHttpSourceViewerDlg::MoveSubTab()
{
	static bool isFirstTime = true;
	if (isFirstTime)
	{
		CRect rect;
		m_tabMain.GetClientRect(&rect);
		rect.left += 10;
		rect.top += 30;
		rect.right -= 10;
		rect.bottom -= 10;

		m_tabHtmlHead.MoveWindow(&rect);
		m_tabHtmlBody.MoveWindow(&rect);
		m_tabHtmlGet.MoveWindow(&rect);
	}
	isFirstTime = false;
}


void CHttpSourceViewerDlg::OnTcnSelchangeTab1Main(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	int CurSel = m_tabMain.GetCurSel();

	m_tabHtmlHead.ShowWindow(false);
	m_tabHtmlBody.ShowWindow(false);
	m_tabHtmlGet.ShowWindow(false);

	switch (CurSel)
	{
	case 0:
		m_tabHtmlHead.ShowWindow(true);
		break;
	case 1:
		m_tabHtmlBody.ShowWindow(true);
		break;
	case 2:
		m_tabHtmlGet.ShowWindow(true);
		break;
	default:
		break;
	}
	*pResult = 0;
}


void CHttpSourceViewerDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strURL;
	m_editURL.GetWindowText(strURL);

	if (strURL.Left(4) != _T("http"))
	{
		strURL.Insert(0, _T("http://"));
	}
	// Set URL.
	WinHttpClient client(strURL.GetBuffer(), &CHttpSourceViewerDlg::ProgressProc);

	// Send http request, a GET request by default.
	client.SendHttpRequest();

	// The response header.
	wstring httpResponseHeader = client.GetResponseHeader();

	// The response content.
	wstring httpResponseContent = client.GetResponseContent();

	m_tabHtmlHead.m_editHtmlHead.SetWindowText(httpResponseHeader.c_str());

	m_tabHtmlBody.m_editHtmlBody.SetWindowText(httpResponseContent.c_str());
}


bool CHttpSourceViewerDlg::ProgressProc(double progress)
{
	pThis->m_urlProgress.SetPos(static_cast<int>(progress));
	return true;
}
