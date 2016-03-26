// TabHtmlBody.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HttpSourceViewer.h"
#include "TabHtmlBody.h"
#include "afxdialogex.h"

WNDPROC g_lpEditProc;
HWND g_editHwnd;

// CTabHtmlBody �Ի���

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


// CTabHtmlBody ��Ϣ�������


BOOL CTabHtmlBody::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitRichEdit();

	// ��ʼ�����������б�
	InitRegComboBoxStyle();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


bool CTabHtmlBody::InitRichEdit()
{
	// ��������
	CFont font;
	font.CreatePointFont(100, _T("����"));
	m_editHtmlBody.SetFont(&font);

	// �������������ʾ���
	m_editHtmlBody.SendMessage(EM_SETMARGINS, EC_RIGHTMARGIN | EC_LEFTMARGIN,
		0x00050005 + 30);
	m_editHtmlBody.SendMessage(EM_EXLIMITTEXT, 0, -1);

	// �кŵ�ʵ��
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
		// �����ҳ�����
		MarkSource(lPos, lPos + nLen);
		ft.chrg.cpMin = lPos + nLen;
	} while (true);

}


void CTabHtmlBody::OnBnClickedButtonSearch()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
		//���к��������ʾ�кŵĹ���
		ShowLineNum(g_editHwnd);

		return ::EndPaint(hWnd, &ps);
	}
	return ::CallWindowProc(g_lpEditProc, hWnd, uMsg, wParam, lParam);
}

void  CTabHtmlBody::ShowLineNum(HWND hEdit)
{
	/*
	���ܣ���ʾ�ı���������
	������
	hEdit��Ҫ��ʾ�кŵ��ı�����ͨ��Edit�ؼ�û�в��Թ�������ֻ��RichEdit

	����ֵ��û�С�
	--------------------------------------------------------------------------------
	*/
	RECT			ClientRect;			//RichEdit�Ŀͻ�����С
	HDC				hdcEdit;			//RichEdit��Dc���豸������
	HDC				hdcCpb;				//��RichEdit���ݵ�Dc
	HBITMAP			hdcBmp;				//RichEdit���ݵ�λͼdc
	int				CharHeight;			//�ַ��ĸ߶�
	int				chHeight;			//�ַ��ĸ߶ȣ�����	
	int				FirstLine;			//�ı����еĵ�һ���ɼ��е��кš�
	int				ClientHeight;		//RichEdit�Ŀͻ����߶�	
	int				LineCount;			//�ı���������	
	wchar_t			countBuf[10];		//��ʾ�кŵĻ�����
	CHARFORMAT		CharFmt;			//RichEdit�е�һ���ṹ�����ڻ�ȡ�ַ���һϵ����Ϣ������ֻ��������ȡ�ַ��߶�

	//��ȡRichEdit��Dc
	hdcEdit = ::GetDC(hEdit);
	::GetClientRect(hEdit, &ClientRect);
	//��ȡRichEdit�Ŀͻ����߶�
	ClientHeight = ClientRect.bottom - ClientRect.top;
	//������RichEdit���ݵ�Dc
	hdcCpb = ::CreateCompatibleDC(hdcEdit);
	//������RichEdit���ݵ�λͼDc������������ʾ�к��õġ�
	hdcBmp = ::CreateCompatibleBitmap(hdcEdit, 30, ClientHeight);
	//��λͼdcѡ��RichEdit������
	::SelectObject(hdcCpb, hdcBmp);
	//�����ʾ�к�dc�ı�����ɫ����ҿ�������������ɫ
	::FillRect(hdcCpb, &ClientRect, ::CreateSolidBrush(0x8080ff));
	::SetBkMode(hdcCpb, TRANSPARENT);
	//��ȡ��ǰRichEdit�ı��е�������
	LineCount = ::SendMessage(hEdit, EM_GETLINECOUNT, 0, 0);

	RtlZeroMemory(&CharFmt, sizeof(CharFmt));
	CharFmt.cbSize = sizeof(CharFmt);
	::SendMessage(hEdit, EM_GETCHARFORMAT, TRUE, (long)&CharFmt);
	//��ȡ�ַ��߶ȣ���Ӣ��Ϊ��λ����ת��Ϊ����
	CharHeight = CharFmt.yHeight / 15;
	chHeight = CharHeight;
	CharHeight = 2;
	//������ʾ�кŵ�ǰ��ɫ
	::SetTextColor(hdcCpb, 0x000000);
	//��ȡ�ı����е�һ���ɼ����е��кţ�û������кţ���ʾ��������ı��Ĺ�����������
	FirstLine = ::SendMessage(hEdit, EM_GETFIRSTVISIBLELINE, 0, 0);
	FirstLine++;
	//��λͼdc��ѭ������к�
	while (FirstLine <= LineCount)
	{
		::TextOut(hdcCpb, 1, CharHeight, countBuf, wsprintf(countBuf, L"%4u", FirstLine++));
		CharHeight += chHeight + 3;
		if (CharHeight > ClientHeight) break;
	}
	//����"����"��λͼ����"��"��RichEdit��
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
