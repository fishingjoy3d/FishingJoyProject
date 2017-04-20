#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// DlgGMToolListPage 对话框
enum ListCtrlType
{
	ListCtrlType_CenterRole,
	ListCtrlType_GameRole,
};
class DlgGMToolListPage : public CDialogEx
{
	DECLARE_DYNAMIC(DlgGMToolListPage)

public:
	DlgGMToolListPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgGMToolListPage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CMD_LIST };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	std::map<CString, CDialogEx*> _CMD_Dlgs;
	std::map<CString, QueryUserType> _CobConfig;
	int _CurCtrlPop;
	ListCtrlType _ListCtrlType;
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrlRoles;
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
	void RefrashRoleList();
	CComboBox _CBFindType;
	afx_msg void OnBnClickedBtnFind();
	afx_msg void OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFrozenUser();
	afx_msg void OnListSendSystemMail();
	afx_msg void OnListNotice();
	afx_msg void OnBnClickedBtnSendMsg();
	afx_msg void OnBnClickedBtnSendOperatorMail();
};
