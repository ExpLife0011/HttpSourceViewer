// TabHtmlBody.cpp : 实现文件
//

#include "stdafx.h"
#include "HttpSourceViewer.h"
#include "TabHtmlBody.h"
#include "afxdialogex.h"

WNDPROC g_lpEditProc;
HWND g_editHwnd;

// CTabHtmlBody 对话框

IMPLEMENT_DYNAMIC(CTabHtmlBody, CDialogEx)

CTabHtmlBody::CTabHtmlBody(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabHtmlBody::IDD, pParent)
{

}

CTabHtmlBody::~CTabHtmlBody()
{
}

void CTabHtmlBody::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_HTML_BODY, m_editHtmlBody);
	DDX_Control(pDX, IDC_EDIT_SEARCH_INPUT, m_searchInput);
	DDX_Control(pDX, IDC_COMBO_REG, m_cbRegStyle);
}


BEGIN_MESSAGE_MAP(CTabHtmlBody, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CTabHtmlBody::OnBnClickedButtonSearch)
END_MESSAGE_MAP()


// CTabHtmlBody 消息处理程序


BOOL CTabHtmlBody::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitRichEdit();

	// 初始化正则下拉列表
	InitRegComboBoxStyle();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


bool CTabHtmlBody::InitRichEdit()
{
	// 设置字体
	CFont font;
	font.CreatePointFont(100, _T("宋体"));
	m_editHtmlBody.SetFont(&font);

	// 留出左侧用来显示编号
	m_editHtmlBody.SendMessage(EM_SETMARGINS, EC_RIGHTMARGIN | EC_LEFTMARGIN,
		0x00050005 + 30);
	m_editHtmlBody.SendMessage(EM_EXLIMITTEXT, 0, -1);

	// 行号的实现
	g_editHwnd = m_editHtmlBody.GetSafeHwnd();
	g_lpEditProc = (WNDPROC)::SetWindowLong(m_editHtmlBody.GetSafeHwnd(), GWL_WNDPROC, (LONG)&CTabHtmlBody::SubEditProc);

	return true;
}


void CTabHtmlBody::FindTextInHtml()
{
	CString strSearchText;
	m_searchInput.GetWindowText(strSearchText);

	FINDTEXTEX ft;
	ft.chrg.cpMin = 0;
	ft.chrg.cpMax = -1;
	ft.lpstrText = strSearchText.GetBuffer();

	long lPos = -1;
	int nLen = strSearchText.GetLength();

	do 
	{
		lPos = m_editHtmlBody.FindText(FR_DOWN, &ft);
		if (-1 == lPos)
		{
			break;
		}
		// 给查找出来的
		MarkSource(lPos, lPos + nLen);
		ft.chrg.cpMin = lPos + nLen;
	} while (true);

}


void CTabHtmlBody::OnBnClickedButtonSearch()
{
	// TODO:  在此添加控件通知处理程序代码
	FindTextInHtml();
}


int CTabHtmlBody::MarkSource(int start, int end)
{
	m_editHtmlBody.SetSel(start, end);

	CHARFORMAT2 cf;
	memset(&cf, 0, sizeof(cf));
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_BACKCOLOR;
	cf.dwEffects = 1;
	cf.crTextColor = RGB(0, 0, 0);
	cf.crBackColor = RGB(116, 208, 241);;

	m_editHtmlBody.SendMessage(EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
	return 0;
}

long CTabHtmlBody::SubEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT		ps;

	if (uMsg == WM_PAINT)
	{
		::CallWindowProc(g_lpEditProc, hWnd, uMsg, wParam, lParam);
		::BeginPaint(hWnd, &ps);
		//下列函数完成显示行号的功能
		ShowLineNum(g_editHwnd);

		return ::EndPaint(hWnd, &ps);
	}
	return ::CallWindowProc(g_lpEditProc, hWnd, uMsg, wParam, lParam);
}

void  CTabHtmlBody::ShowLineNum(HWND hEdit)
{
	/*
	功能：显示文本的总行数
	参数：
	hEdit：要显示行号的文本框，普通的Edit控件没有测试过，这里只用RichEdit

	返回值：没有。
	--------------------------------------------------------------------------------
	*/
	RECT			ClientRect;			//RichEdit的客户区大小
	HDC				hdcEdit;			//RichEdit的Dc（设备环境）
	HDC				hdcCpb;				//与RichEdit兼容的Dc
	HBITMAP			hdcBmp;				//RichEdit兼容的位图dc
	int				CharHeight;			//字符的高度
	int				chHeight;			//字符的高度，常量	
	int				FirstLine;			//文本框中的第一个可见行的行号。
	int				ClientHeight;		//RichEdit的客户区高度	
	int				LineCount;			//文本的总行数	
	wchar_t			countBuf[10];		//显示行号的缓冲区
	CHARFORMAT		CharFmt;			//RichEdit中的一个结构，用于获取字符的一系列信息，这里只用它来获取字符高度

	//获取RichEdit的Dc
	hdcEdit = ::GetDC(hEdit);
	::GetClientRect(hEdit, &ClientRect);
	//获取RichEdit的客户区高度
	ClientHeight = ClientRect.bottom - ClientRect.top;
	//创建与RichEdit兼容的Dc
	hdcCpb = ::CreateCompatibleDC(hdcEdit);
	//创建与RichEdit兼容的位图Dc，我们用来显示行号用的。
	hdcBmp = ::CreateCompatibleBitmap(hdcEdit, 30, ClientHeight);
	//将位图dc选入RichEdit环境中
	::SelectObject(hdcCpb, hdcBmp);
	//填充显示行号dc的背景颜色。大家可以试试其它颜色
	::FillRect(hdcCpb, &ClientRect, ::CreateSolidBrush(0x8080ff));
	::SetBkMode(hdcCpb, TRANSPARENT);
	//获取当前RichEdit文本中的总行数
	LineCount = ::SendMessage(hEdit, EM_GETLINECOUNT, 0, 0);

	RtlZeroMemory(&CharFmt, sizeof(CharFmt));
	CharFmt.cbSize = sizeof(CharFmt);
	::SendMessage(hEdit, EM_GETCHARFORMAT, TRUE, (long)&CharFmt);
	//获取字符高度，以英寸为单位，需转化为磅。
	CharHeight = CharFmt.yHeight / 15;
	chHeight = CharHeight;
	CharHeight = 2;
	//设置显示行号的前景色
	::SetTextColor(hdcCpb, 0x000000);
	//获取文本框中第一个可见的行的行号，没有这个行号，显示不会跟着文本的滚动而滚动。
	FirstLine = ::SendMessage(hEdit, EM_GETFIRSTVISIBLELINE, 0, 0);
	FirstLine++;
	//在位图dc中循环输出行号
	while (FirstLine <= LineCount)
	{
		::TextOut(hdcCpb, 1, CharHeight, countBuf, wsprintf(countBuf, L"%4u", FirstLine++));
		CharHeight += chHeight + 3;
		if (CharHeight > ClientHeight) break;
	}
	//将已"画好"的位图真正"贴"到RichEdit中
	::BitBlt(hdcEdit, 0, 0, 40, ClientHeight, hdcCpb, 0, 0, SRCCOPY);
	::DeleteDC(hdcCpb);
	::ReleaseDC(hEdit, hdcEdit);
	::DeleteObject(hdcBmp);
}

void CTabHtmlBody::InitRegComboBoxStyle()
{
	m_cbRegStyle.InsertString(0, _T("Boost"));
	m_cbRegStyle.InsertString(1, _T("MFC ATL"));
	m_cbRegStyle.SetCurSel(0);
}
