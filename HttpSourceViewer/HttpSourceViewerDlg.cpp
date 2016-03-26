
// HttpSourceViewerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HttpSourceViewer.h"
#include "HttpSourceViewerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CHttpSourceViewerDlg �Ի���

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


// CHttpSourceViewerDlg ��Ϣ�������

BOOL CHttpSourceViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	InitTabControl();
	m_editURL.SetWindowText(_T("5566.net"));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CHttpSourceViewerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CHttpSourceViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHttpSourceViewerDlg::InitTabControl()
{
	m_tabMain.InsertItem(0, _T("HTML ͷ"));
	m_tabMain.InsertItem(1, _T("HTML ����"));
	m_tabMain.InsertItem(2, _T("��ȡ����"));

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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
