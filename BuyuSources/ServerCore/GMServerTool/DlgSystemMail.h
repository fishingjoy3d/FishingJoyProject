#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


// DlgSystemMail 对话框

enum DlgSystemMailType
{
	DlgSystemMailType_SinglePlayer,
	DlgSystemMailType_SomePlayers
};

class DlgSystemMail : public CDialogEx
{
	DECLARE_DYNAMIC(DlgSystemMail)

public:
	DlgSystemMail(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgSystemMail();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SYSTEM_MAIL };
#endif
protected:
	CString _NickName;
	DWORD _UserID;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DlgSystemMailType _DlgType;
	DECLARE_MESSAGE_MAP()
public:
	void SetNickName(CString& NickName);
	void SetUserID(DWORD UserID);
	void SetDlgType(DlgSystemMailType type);
public:
	CListBox _ListRewards;
	CListCtrl _ListCtrlRewardItems;
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnLbnSelchangeListRewardId();
	afx_msg void OnBnClickedOk();
	
	CDateTimeCtrl _CtrlBeginTime;
	CDateTimeCtrl _CtrlEndTime;

};
