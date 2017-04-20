// DlgFrozenUser.cpp : 实现文件
//

#include "stdafx.h"
#include "GMServerTool.h"
#include "DlgFrozenUser.h"
#include "afxdialogex.h"


// DlgFrozenUser 对话框

IMPLEMENT_DYNAMIC(DlgFrozenUser, CDialogEx)

DlgFrozenUser::DlgFrozenUser(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_FROZEN_USER, pParent)
{

}

DlgFrozenUser::~DlgFrozenUser()
{
}

void DlgFrozenUser::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BOOL DlgFrozenUser::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetDlgItemInt(IDC_EDIT_FROZEN_TIME, 0);
	SetDlgItemInt(IDC_EDIT_USER_ID, _UserID);
	return TRUE;
}



void DlgFrozenUser::SetNickName(CString NickName)
{
	_NickName = NickName;
}
void DlgFrozenUser::SetUserID(DWORD UserID)
{
	_UserID = UserID;	
}
BEGIN_MESSAGE_MAP(DlgFrozenUser, CDialogEx)
	ON_BN_CLICKED(IDOK, &DlgFrozenUser::OnBnClickedOk)
END_MESSAGE_MAP()


// DlgFrozenUser 消息处理程序


void DlgFrozenUser::OnBnClickedOk()
{
	int FrozenTime = GetDlgItemInt(IDC_EDIT_FROZEN_TIME);
	g_GMToolManager.KickUser(_UserID, FrozenTime);
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
