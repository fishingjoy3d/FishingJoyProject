// DlgLogin.cpp : 实现文件
//

#include "stdafx.h"
#include "GMServerTool.h"
#include "DlgLogin.h"
#include "afxdialogex.h"


// DlgLogin 对话框

IMPLEMENT_DYNAMIC(DlgLogin, CDialogEx)

DlgLogin::DlgLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_LOGIN, pParent)
{

}

DlgLogin::~DlgLogin()
{
}

void DlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgLogin, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &DlgLogin::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &DlgLogin::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// DlgLogin 消息处理程序


void DlgLogin::OnBnClickedBtnLogin()
{
	CString acc;
	CString password;
	GetDlgItemText(IDC_EDT_ACCOUNT, acc);
	GetDlgItemText(IDC_EDT_PASSWORD, password);
	char * acc_temp = NULL;
	g_GMToolManager.StringToChar(acc, acc_temp);
	char * pd = NULL;
	g_GMToolManager.StringToChar(password, pd);
	g_GMToolManager.SendLoginReq(acc_temp, pd);
	delete[] acc_temp;
	delete[] pd;
	// TODO: 在此添加控件通知处理程序代码
}


void DlgLogin::OnBnClickedBtnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
}
