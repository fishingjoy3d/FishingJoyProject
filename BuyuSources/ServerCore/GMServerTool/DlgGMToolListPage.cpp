// DlgGMToolListPage.cpp : 实现文件
//

#include "stdafx.h"
#include "GMServerTool.h"
#include "DlgGMToolListPage.h"
#include "afxdialogex.h"


// DlgGMToolListPage 对话框

IMPLEMENT_DYNAMIC(DlgGMToolListPage, CDialogEx)

DlgGMToolListPage::DlgGMToolListPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CMD_LIST, pParent)
{
	_CurCtrlPop = 0;
}

DlgGMToolListPage::~DlgGMToolListPage()
{
}

void DlgGMToolListPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST2, m_ListCtrlRoles);
	DDX_Control(pDX, IDC_COMBO_FIND_TYPE, _CBFindType);
}


BEGIN_MESSAGE_MAP(DlgGMToolListPage, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &DlgGMToolListPage::OnLvnItemchangedList2)
	ON_BN_CLICKED(IDC_BTN_FIND, &DlgGMToolListPage::OnBnClickedBtnFind)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &DlgGMToolListPage::OnNMRClickList2)
	ON_COMMAND(ID_FROZEN_USER, &DlgGMToolListPage::OnFrozenUser)
	
	ON_COMMAND(ID_LIST_SEND_SYSTEM_MAIL, &DlgGMToolListPage::OnListSendSystemMail)
	ON_COMMAND(ID_LIST_NOTICE, &DlgGMToolListPage::OnListNotice)
	ON_BN_CLICKED(IDC_BTN_SEND_MSG, &DlgGMToolListPage::OnBnClickedBtnSendMsg)
	ON_BN_CLICKED(IDC_BTN_SEND_OPERATOR_MAIL, &DlgGMToolListPage::OnBnClickedBtnSendOperatorMail)
END_MESSAGE_MAP()


// DlgGMToolListPage 消息处理程序



void DlgGMToolListPage::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


BOOL DlgGMToolListPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	_CobConfig[TEXT("all_user")] = QueryUserType_All;
	_CobConfig[TEXT("online_user")] = QueryUserType_Online;
	_CobConfig[TEXT("offline_user")] = QueryUserType_Online;
	_CobConfig[TEXT("nick_name_user")] = QueryUserType_ByNickName;

	((CEdit*)GetDlgItem(IDC_COMBO_FIND_TYPE))->SetReadOnly(TRUE);

	std::map<CString, QueryUserType>::iterator it = _CobConfig.begin();
	for (; it != _CobConfig.end(); ++ it)
	{		
		_CBFindType.AddString(it->first);
	}
	_CBFindType.SetCurSel(0);
	LONG lStyle;
	lStyle = GetWindowLong(m_ListCtrlRoles.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_ListCtrlRoles.m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle = m_ListCtrlRoles.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_ListCtrlRoles.SetExtendedStyle(dwStyle); //设置扩展风格

	_ListCtrlType = ListCtrlType_CenterRole;
	m_ListCtrlRoles.InsertColumn(0, TEXT("user_id"), LVCFMT_CENTER, 40, 50);
	m_ListCtrlRoles.InsertColumn(1, TEXT("nick_name"), LVCFMT_CENTER, 140, 50);
	m_ListCtrlRoles.InsertColumn(2, TEXT("level"), LVCFMT_CENTER, 80, 50);
	m_ListCtrlRoles.InsertColumn(3, TEXT("gender"), LVCFMT_CENTER, 40, 50);
	m_ListCtrlRoles.InsertColumn(4, TEXT("vip_level"), LVCFMT_CENTER, 40, 50);
	m_ListCtrlRoles.InsertColumn(5, TEXT("particular_state"), LVCFMT_CENTER, 40, 50);

	m_ListCtrlRoles.InsertColumn(6, TEXT("game_id"), LVCFMT_CENTER, 40, 50);
	m_ListCtrlRoles.InsertColumn(7, TEXT("金币"), LVCFMT_CENTER, 140, 50); //dwGlobeNum
	m_ListCtrlRoles.InsertColumn(8, TEXT("dwMedalNum"), LVCFMT_CENTER, 40, 50);
	m_ListCtrlRoles.InsertColumn(9, TEXT("钻石"), LVCFMT_CENTER, 40, 50); //dwCurrencyNum
	m_ListCtrlRoles.InsertColumn(10, TEXT("当天获得金币"), LVCFMT_CENTER, 140, 50); //dwProduction
	m_ListCtrlRoles.InsertColumn(11, TEXT("当天游戏时间"), LVCFMT_CENTER, 140, 50); //dwGameTime
	m_ListCtrlRoles.InsertColumn(12, TEXT("SendGiffSum"), LVCFMT_CENTER, 40, 50);
	m_ListCtrlRoles.InsertColumn(13, TEXT("AcceptGiffSum"), LVCFMT_CENTER, 40, 50);

	m_ListCtrlRoles.InsertColumn(14, TEXT("OnlineMin"), LVCFMT_CENTER, 40, 50);
	m_ListCtrlRoles.InsertColumn(15, TEXT("OnlineRewardStates"), LVCFMT_CENTER, 40, 50);
	m_ListCtrlRoles.InsertColumn(16, TEXT("IsCanResetAccount"), LVCFMT_CENTER, 40, 50);
	m_ListCtrlRoles.InsertColumn(17, TEXT("AchievementPointIndex"), LVCFMT_CENTER, 40, 50);

	m_ListCtrlRoles.InsertColumn(18, TEXT("CheckData"), LVCFMT_CENTER, 40, 50);
	m_ListCtrlRoles.InsertColumn(19, TEXT("ExChangeStates"), LVCFMT_CENTER, 40, 50);
	m_ListCtrlRoles.InsertColumn(20, TEXT("TotalRechargeSum"), LVCFMT_CENTER, 40, 50);

	m_ListCtrlRoles.InsertColumn(21, TEXT("bIsFirstPayGlobel"), LVCFMT_CENTER, 40, 50);
	m_ListCtrlRoles.InsertColumn(22, TEXT("bIsFirstPayCurrcey"), LVCFMT_CENTER, 40, 50);

	m_ListCtrlRoles.InsertColumn(23, TEXT("LotteryScore"), LVCFMT_CENTER, 40, 50);
	m_ListCtrlRoles.InsertColumn(24, TEXT("LotteryFishSum"), LVCFMT_CENTER, 40, 50);

	m_ListCtrlRoles.InsertColumn(25, TEXT("MonthCardID"), LVCFMT_CENTER, 40, 50);
	m_ListCtrlRoles.InsertColumn(26, TEXT("MonthCardEndTime"), LVCFMT_CENTER, 40, 50);
	m_ListCtrlRoles.InsertColumn(27, TEXT("GetMonthCardRewardTime"), LVCFMT_CENTER, 40, 50);

	m_ListCtrlRoles.InsertColumn(28, TEXT("RateValue"), LVCFMT_CENTER, 40, 50);
	m_ListCtrlRoles.InsertColumn(29, TEXT("CashSum"), LVCFMT_CENTER, 40, 50);
	m_ListCtrlRoles.InsertColumn(30, TEXT("benefitCount"), LVCFMT_CENTER, 40, 50);
	m_ListCtrlRoles.InsertColumn(31, TEXT("benefitTime"), LVCFMT_CENTER, 40, 50);
	m_ListCtrlRoles.InsertColumn(32, TEXT("TotalUseMedal"), LVCFMT_CENTER, 40, 50);
	m_ListCtrlRoles.InsertColumn(33, TEXT("ParticularStates"), LVCFMT_CENTER, 140, 50);

	m_ListCtrlRoles.InsertColumn(34, TEXT("bShareStates"), LVCFMT_CENTER, 40, 50);
	m_ListCtrlRoles.InsertColumn(35, TEXT("TotalCashSum"), LVCFMT_CENTER, 40, 50);
	//_CBFindType.AddString();
	RefrashRoleList();
	return TRUE;
}

void DlgGMToolListPage::RefrashRoleList()
{
	std::map<DWORD, tagRoleInfo>* roles = g_GMToolManager.GetRoleList();
	WCHAR temp[512];
	swprintf_s(temp, TEXT("%d"), roles->size());
	SetDlgItemText(IDC_EDIT_CUR_ROLE_NUMBER, temp);
	m_ListCtrlRoles.DeleteAllItems();
	int columns = m_ListCtrlRoles.GetHeaderCtrl()->GetItemCount();
	std::map<DWORD, tagRoleInfo>::iterator it = roles->begin();
	WCHAR sz[512];
	for (int i = 0; it != roles->end(); it++, i++)
	{

		tagRoleInfo& role = it->second;
		m_ListCtrlRoles.InsertItem(i, role.NickName);
		m_ListCtrlRoles.SetItemData(i, (DWORD_PTR)&role);
		swprintf_s(sz, TEXT("%u"), role.dwUserID);
		//sprintf_s(sz, "%u", role.dwUserID);
		m_ListCtrlRoles.SetItemText(i, 0, (LPCTSTR)sz);
		m_ListCtrlRoles.SetItemText(i, 1, role.NickName);

		swprintf_s(sz, TEXT("%u"), role.wLevel);
		m_ListCtrlRoles.SetItemText(i, 2, (LPCTSTR)sz);
		if (role.bGender)
		{
			swprintf_s(sz, TEXT("Male"));
		}
		else
		{
			swprintf_s(sz, TEXT("Female"));
		}
		m_ListCtrlRoles.SetItemText(i, 3, (LPCTSTR)sz);
		swprintf_s(sz, TEXT("%u"), role.VipLevel);
		m_ListCtrlRoles.SetItemText(i, 4, (LPCTSTR)sz);

		swprintf_s(sz, TEXT("%u"), role.ParticularStates);
		m_ListCtrlRoles.SetItemText(i, 5, (LPCTSTR)sz);

		swprintf_s(sz, TEXT("%u"), role.GameID);
		m_ListCtrlRoles.SetItemText(i, 6, (LPCTSTR)sz);

		swprintf_s(sz, TEXT("%u"), role.dwGlobeNum);
		m_ListCtrlRoles.SetItemText(i, 7, (LPCTSTR)sz);

		swprintf_s(sz, TEXT("%u"), role.dwMedalNum);
		m_ListCtrlRoles.SetItemText(i, 8, (LPCTSTR)sz);

		swprintf_s(sz, TEXT("%u"), role.dwCurrencyNum);
		m_ListCtrlRoles.SetItemText(i, 9, (LPCTSTR)sz);


		swprintf_s(sz, TEXT("%u"), role.dwProduction);
		m_ListCtrlRoles.SetItemText(i, 10, (LPCTSTR)sz);

		swprintf_s(sz, TEXT("%u"), role.dwGameTime);
		m_ListCtrlRoles.SetItemText(i, 11, (LPCTSTR)sz);
		swprintf_s(sz, TEXT("%u"), role.SendGiffSum);
		m_ListCtrlRoles.SetItemText(i, 12, (LPCTSTR)sz);
		swprintf_s(sz, TEXT("%u"), role.AcceptGiffSum);
		m_ListCtrlRoles.SetItemText(i, 13, (LPCTSTR)sz);


		swprintf_s(sz, TEXT("%u"), role.OnlineMin);
		m_ListCtrlRoles.SetItemText(i, 14, (LPCTSTR)sz);
		swprintf_s(sz, TEXT("%u"), role.OnlineRewardStates);
		m_ListCtrlRoles.SetItemText(i, 15, (LPCTSTR)sz);
		swprintf_s(sz, TEXT("%u"), role.IsCanResetAccount);
		m_ListCtrlRoles.SetItemText(i, 16, (LPCTSTR)sz);

		swprintf_s(sz, TEXT("%u"), role.AchievementPointIndex);
		m_ListCtrlRoles.SetItemText(i, 17, (LPCTSTR)sz);

		swprintf_s(sz, TEXT("%u"), role.CheckData);
		m_ListCtrlRoles.SetItemText(i, 18, (LPCTSTR)sz);
		swprintf_s(sz, TEXT("%u"), role.ExChangeStates);
		m_ListCtrlRoles.SetItemText(i, 19, (LPCTSTR)sz);
		swprintf_s(sz, TEXT("%u"), role.TotalRechargeSum);
		m_ListCtrlRoles.SetItemText(i, 20, (LPCTSTR)sz);
		swprintf_s(sz, TEXT("%u"), role.bIsFirstPayGlobel);
		m_ListCtrlRoles.SetItemText(i, 21, (LPCTSTR)sz);

		swprintf_s(sz, TEXT("%u"), role.bIsFirstPayCurrcey);
		m_ListCtrlRoles.SetItemText(i, 22, (LPCTSTR)sz);
		swprintf_s(sz, TEXT("%u"), role.LotteryScore);
		m_ListCtrlRoles.SetItemText(i, 23, (LPCTSTR)sz);
		swprintf_s(sz, TEXT("%u"), role.LotteryFishSum);
		m_ListCtrlRoles.SetItemText(i, 24, (LPCTSTR)sz);
		swprintf_s(sz, TEXT("%u"), role.MonthCardID);
		m_ListCtrlRoles.SetItemText(i, 25, (LPCTSTR)sz);

		swprintf_s(sz, TEXT("%llu"), role.MonthCardEndTime);
		m_ListCtrlRoles.SetItemText(i, 26, (LPCTSTR)sz);
		swprintf_s(sz, TEXT("%llu"), role.GetMonthCardRewardTime);
		m_ListCtrlRoles.SetItemText(i, 27, (LPCTSTR)sz);
		swprintf_s(sz, TEXT("unkonow"));
		m_ListCtrlRoles.SetItemText(i, 28, (LPCTSTR)sz);
		swprintf_s(sz, TEXT("%u"), role.CashSum);
		m_ListCtrlRoles.SetItemText(i, 29, (LPCTSTR)sz);
		swprintf_s(sz, TEXT("%u"), role.benefitCount);
		m_ListCtrlRoles.SetItemText(i, 30, (LPCTSTR)sz);
		
		swprintf_s(sz, TEXT("%lu"), role.benefitTime);
		m_ListCtrlRoles.SetItemText(i, 31, (LPCTSTR)sz);
		swprintf_s(sz, TEXT("%lu"), role.TotalUseMedal);
		m_ListCtrlRoles.SetItemText(i, 32, (LPCTSTR)sz);
		swprintf_s(sz, TEXT("%u"), role.ParticularStates);
		m_ListCtrlRoles.SetItemText(i, 33, (LPCTSTR)sz);
		swprintf_s(sz, TEXT("%u"), role.bShareStates);
		m_ListCtrlRoles.SetItemText(i, 34, (LPCTSTR)sz);
		swprintf_s(sz, TEXT("%u"), role.TotalCashSum);
		m_ListCtrlRoles.SetItemText(i, 35, (LPCTSTR)sz);


	}
}

void DlgGMToolListPage::OnBnClickedBtnFind()
{
	CString cur_sel;
	_CBFindType.GetLBText(_CBFindType.GetCurSel(), cur_sel);
	QueryUserType cur_type = QueryUserType_All;
	std::map<CString, QueryUserType>::iterator it = _CobConfig.find(cur_sel);
	if (it != _CobConfig.end())
	{
		cur_type = it->second;
	}
	CString argu;
	GetDlgItemText(IDC_EDIT_FIND, argu);
	g_GMToolManager.SendQueryUser(cur_type, argu);
	// TODO: 在此添加控件通知处理程序代码
}


void DlgGMToolListPage::OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码  
	*pResult = 0;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu menu;
		//添加线程操作  
		VERIFY(menu.LoadMenu(IDR_MENU1));           //这里是我们在1中定义的MENU的文件名称  
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		_CurCtrlPop = pNMListView->iItem;
		//下面的两行代码主要是为了后面的操作为准备的  
		//获取列表视图控件中第一个被选择项的位置    
		//POSITION m_pstion = GetFirstSelectedItemPosition();
		//该函数获取由pos指定的列表项的索引，然后将pos设置为下一个位置的POSITION值  
		//m_nIndex = GetNextSelectedItem(m_pstion);

		//CString str;
		//str.Format(L" m_nIndex = %d", m_nIndex);
		//MessageBox(str, str, 0);
	}
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void DlgGMToolListPage::OnFrozenUser()
{
	tagRoleInfo* info = (tagRoleInfo*)m_ListCtrlRoles.GetItemData(_CurCtrlPop);

	DlgFrozenUser dlg;	
	dlg.SetUserID(info->dwUserID);
	CString nickname;
	nickname.Format(info->NickName);
	dlg.SetNickName(nickname);
	if (dlg.DoModal() == IDOK)
	{
		//处理完毕后的操作
	}		
}



void DlgGMToolListPage::OnListSendSystemMail()
{

	DlgSystemMail dlg;
	tagRoleInfo* info = (tagRoleInfo*)m_ListCtrlRoles.GetItemData(_CurCtrlPop);
	dlg.SetUserID(info->dwUserID);
	dlg.SetDlgType(DlgSystemMailType_SinglePlayer);
	CString nickname;
	nickname.Format(info->NickName);
	dlg.SetNickName(nickname);

	if (dlg.DoModal() == IDOK)
	{
		//处理完毕后的操作
	}
	// TODO: 在此添加命令处理程序代码
}


void DlgGMToolListPage::OnListNotice()
{
	DlgNotice dlg;
	if (dlg.DoModal() == IDOK)
	{
		//处理完毕后的操作
	}

	// TODO: 在此添加命令处理程序代码
}


void DlgGMToolListPage::OnBnClickedBtnSendMsg()
{
	OnListNotice();
	// TODO: 在此添加控件通知处理程序代码
}


void DlgGMToolListPage::OnBnClickedBtnSendOperatorMail()
{
	DlgSystemMail dlg;
	//tagRoleInfo* info = (tagRoleInfo*)m_ListCtrlRoles.GetItemData(_CurCtrlPop);	
	dlg.SetDlgType(DlgSystemMailType_SomePlayers);
	if (dlg.DoModal() == IDOK)
	{
		//处理完毕后的操作
	}
	// TODO: 在此添加控件通知处理程序代码
}
