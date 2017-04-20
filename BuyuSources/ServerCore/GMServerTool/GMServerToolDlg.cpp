
// GMServerToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GMServerTool.h"
#include "GMServerToolDlg.h"
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
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CGMServerToolDlg 对话框



CGMServerToolDlg::CGMServerToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GMSERVERTOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGMServerToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGMServerToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CGMServerToolDlg 消息处理程序

BOOL CGMServerToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	g_dlg = this;
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
	DlgLogin *DlgLoginTemp = new DlgLogin(this);
	DlgLoginTemp->Create(IDD_DIALOG_LOGIN, this);	
	DlgLoginTemp->ShowWindow(1);
	SetTimer(0, 100, 0);
	_dlgs[LoginDlg_Type] = DlgLoginTemp;	
	DlgGMToolListPage * DlgGMToolList = new DlgGMToolListPage(this);
	DlgGMToolList->Create(IDD_DIALOG_CMD_LIST, this);
	_dlgs[CmdPage_Type] = DlgGMToolList;
	std::map<DlgType, CDialogEx*>::iterator it = _dlgs.begin();
	for (; it != _dlgs.end(); ++ it)
	{
		CDialogEx* dlg = it->second;
		dlg->ActivateTopParent();
	}
	g_GMToolManager.Init();
	// TODO: 在此添加额外的初始化代码
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CGMServerToolDlg::LoginSucessful()
{
	
	_dlgs[LoginDlg_Type]->ShowWindow(FALSE);
	_dlgs[CmdPage_Type]->ShowWindow(TRUE);
}

void CGMServerToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGMServerToolDlg::OnPaint()
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
HCURSOR CGMServerToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


DlgLogin* CGMServerToolDlg::GetLogin()
{
	return (DlgLogin*)_dlgs[LoginDlg_Type];
}
DlgGMToolListPage* CGMServerToolDlg::GetGMTool()
{
	return (DlgGMToolListPage*)_dlgs[CmdPage_Type];
}


void CGMServerToolDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);
	CRect rcClient;
	GetClientRect(rcClient);
	ClientToScreen(rcClient);
	rcClient.left = rcClient.left + 210;
	std::map<DlgType, CDialogEx*>::iterator it = _dlgs.begin();
	for (; it != _dlgs.end(); ++ it)
	{
		CDialogEx* dlg = it->second;
		dlg->MoveWindow(rcClient);	
	}

	// TODO: 在此处添加消息处理程序代码
}


void CGMServerToolDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnTimer(nIDEvent);
	g_GMToolManager.Update();
}


void CGMServerToolDlg::Notice(CString notice)
{
	_Notices.push_front(notice);
	if (_Notices.size() >= 15)
	{
		_Notices.pop_back();
	}

	CString curText = TEXT("");
	std::list<CString>::iterator it = _Notices.begin();
	for (; it != _Notices.end(); ++ it)
	{
		if (curText.IsEmpty() == false)
		{
			curText += TEXT("/r/n");
		}	
		curText += (*it);
	}
	SetDlgItemText(IDC_EDIT_NOTICE, curText);
}