// DlgNotice.cpp : 实现文件
//

#include "stdafx.h"
#include "GMServerTool.h"
#include "DlgNotice.h"
#include "afxdialogex.h"


// DlgNotice 对话框

IMPLEMENT_DYNAMIC(DlgNotice, CDialogEx)

DlgNotice::DlgNotice(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SEND_MSG, pParent)
{

}

DlgNotice::~DlgNotice()
{
}

void DlgNotice::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgNotice, CDialogEx)
	ON_BN_CLICKED(IDOK, &DlgNotice::OnBnClickedOk)
END_MESSAGE_MAP()


// DlgNotice 消息处理程序


void DlgNotice::OnBnClickedOk()
{
	CString context;
	GetDlgItemText(IDC_EDIT_CONTEXT, context);
	int stepNum;
	int stepSec;
	int param;
	stepNum = GetDlgItemInt(IDC_EDIT_SUM);
	stepSec = GetDlgItemInt(IDC_EDIT_SEC);
	param = GetDlgItemInt(IDC_EDIT_PARAM);
	WORD Color;
	if (((CButton*)GetDlgItem(IDC_RADIO_RED))->GetCheck() == true)
	{
		Color = RGB(255, 0, 0);
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO_GREEN))->GetCheck() == true)
	{
		Color = RGB(0, 255, 0);
	}
	else
	{
		Color = RGB(0, 0, 255);
	}
	
	g_GMToolManager.SendMsgToAllGame(context, stepSec, stepNum, param, Color);
	
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

BOOL DlgNotice::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetDlgItemInt(IDC_EDIT_SUM, 0);
	SetDlgItemInt(IDC_EDIT_SEC, 0);
	SetDlgItemInt(IDC_EDIT_PARAM, 0);
	
	((CButton*)GetDlgItem(IDC_RADIO_RED))->SetCheck(true);
	return TRUE;
}

