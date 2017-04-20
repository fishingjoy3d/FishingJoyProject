// DlgSystemMail.cpp : 实现文件
//

#include "stdafx.h"
#include "GMServerTool.h"
#include "DlgSystemMail.h"
#include "afxdialogex.h"


// DlgSystemMail 对话框

IMPLEMENT_DYNAMIC(DlgSystemMail, CDialogEx)

DlgSystemMail::DlgSystemMail(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_SYSTEM_MAIL, pParent)
{
	_NickName = "";
	_UserID = 0;
}

DlgSystemMail::~DlgSystemMail()
{
}

void DlgSystemMail::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_REWARD_ID, _ListRewards);
	DDX_Control(pDX, IDC_LIST_CTRL_ITEMS, _ListCtrlRewardItems);
	DDX_Control(pDX, IDC_DATETIMEPICKER_BEGIN, _CtrlBeginTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER_END, _CtrlEndTime);
}
BOOL DlgSystemMail::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetDlgItemInt(IDC_EDIT_REWARD_ID, 0);
	SetDlgItemInt(IDC_EDIT_REWARD_COUNT, 0);
	SetDlgItemInt(IDC_EDIT_USER_ID, _UserID);
	SetDlgItemText(IDC_EDIT_USER_NICK_NAME, _NickName);
	LONG lStyle;
	lStyle = GetWindowLong(_ListCtrlRewardItems.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(_ListCtrlRewardItems.m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle = _ListCtrlRewardItems.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	_ListCtrlRewardItems.SetExtendedStyle(dwStyle); //设置扩展风格
	_ListCtrlRewardItems.InsertColumn(0, TEXT("ItemID"), LVCFMT_CENTER, 40, 50);
	_ListCtrlRewardItems.InsertColumn(1, TEXT("ItemSum"), LVCFMT_CENTER, 40, 50);
	_ListCtrlRewardItems.InsertColumn(2, TEXT("LastMin"), LVCFMT_CENTER, 40, 50);
	std::map<DWORD, tagRewardOnce>* rewards = g_GMToolManager.GetRewards();
	std::map<DWORD, tagRewardOnce>::iterator it = rewards->begin();
	CString reward_id;
	for (; it != rewards->end(); ++ it)
	{
		reward_id.Format(TEXT("%u"), it->first);
		_ListRewards.AddString(reward_id);
		reward_id = "";
	}
	if (rewards->size() > 0)
	{
		_ListRewards.SetCurSel(0);
	}
	SetDlgItemText(IDC_EDIT_TIME_BEGIN, TEXT("开始时间"));
	SetDlgItemText(IDC_EDIT_TIME_END, TEXT("结束时间"));
	CTime timeTime(2017, 4, 3, 0, 0, 0);
	_CtrlBeginTime.SetTime(&timeTime);
	_CtrlEndTime.SetTime(&timeTime);
	switch (_DlgType)
	{
	case DlgSystemMailType_SinglePlayer:		
		_CtrlBeginTime.ShowWindow(FALSE);
		_CtrlEndTime.ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_TIME_BEGIN)->ShowWindow(FALSE);
		break;
	case DlgSystemMailType_SomePlayers:
		_CtrlBeginTime.ShowWindow(TRUE);
		_CtrlEndTime.ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_TIME_BEGIN)->ShowWindow(TRUE);
		break;
	default:
		break;
	}
	


	return TRUE;
}


BEGIN_MESSAGE_MAP(DlgSystemMail, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ADD, &DlgSystemMail::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DELETE, &DlgSystemMail::OnBnClickedBtnDelete)
	ON_LBN_SELCHANGE(IDC_LIST_REWARD_ID, &DlgSystemMail::OnLbnSelchangeListRewardId)
	ON_BN_CLICKED(IDOK, &DlgSystemMail::OnBnClickedOk)
END_MESSAGE_MAP()


// DlgSystemMail 消息处理程序


void DlgSystemMail::OnBnClickedBtnAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CString reward_id;
	_ListRewards.GetText(_ListRewards.GetCurSel(), reward_id);
	int id = _wtoi(reward_id);
	SetDlgItemInt(IDC_EDIT_REWARD_ID, id);
}


void DlgSystemMail::OnBnClickedBtnDelete()
{
	SetDlgItemInt(IDC_EDIT_REWARD_ID, 0);
	SetDlgItemInt(IDC_EDIT_REWARD_COUNT, 0);
	// TODO: 在此添加控件通知处理程序代码
}


void DlgSystemMail::OnLbnSelchangeListRewardId()
{
	_ListCtrlRewardItems.DeleteAllItems();
	CString reward_id;
	_ListRewards.GetText(_ListRewards.GetCurSel(), reward_id);
	int id = _wtoi(reward_id);
	std::map<DWORD, tagRewardOnce>* rewards = g_GMToolManager.GetRewards();
	std::map<DWORD, tagRewardOnce>::iterator it = rewards->find(id);
	if (it != rewards->end())
	{
		const tagRewardOnce& tag = it->second;
		vector<tagItemOnce>::const_iterator it_item = tag.RewardItemVec.begin();
		CString item;
		for (int i = 0; it_item != tag.RewardItemVec.end(); ++ it_item, i ++)
		{
			const tagItemOnce& entry = (*it_item);
			item.Format(TEXT("%u"), entry.ItemID);
			_ListCtrlRewardItems.InsertItem(i, item);
			_ListCtrlRewardItems.SetItemText(i, 0, item);

			item.Format(TEXT("%u"), entry.ItemSum);
			_ListCtrlRewardItems.SetItemText(i, 1, item);

			item.Format(TEXT("%u"), entry.LastMin);
			_ListCtrlRewardItems.SetItemText(i, 2, item);
		}
	}
	// TODO: 在此添加控件通知处理程序代码
}


void DlgSystemMail::OnBnClickedOk()
{
	int Reward_id = GetDlgItemInt(IDC_EDIT_REWARD_ID);
	int Reward_count = GetDlgItemInt(IDC_EDIT_REWARD_COUNT);	
	int UserID = GetDlgItemInt(IDC_EDIT_USER_ID);
	if (Reward_id == 0)
	{
		Reward_count = 0;
	}
	CString Context;
	GetDlgItemText(IDC_EDIT_CONTEXT, Context);
	if (_DlgType == DlgSystemMailType_SinglePlayer)
	{
		g_GMToolManager.SendSystemMail(UserID, Context, Reward_id, Reward_count);
	}
	else
	{
		CTime timeTime_Begin;
		CTime timeTime_End;
		_CtrlBeginTime.GetTime(timeTime_Begin);
		_CtrlEndTime.GetTime(timeTime_End);
		unsigned long begin_time = timeTime_Begin.GetTime();
		unsigned long end_time = timeTime_End.GetTime();
		g_GMToolManager.SendSystemOperatorMail(Context, Reward_id, Reward_count, begin_time, end_time);
	}
	

	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
void DlgSystemMail::SetNickName(CString& NickName)
{
	_NickName = NickName;
}
void DlgSystemMail::SetUserID(DWORD UserID)
{
	_UserID = UserID;
}

void DlgSystemMail::SetDlgType(DlgSystemMailType type)
{
	_DlgType = type;
}