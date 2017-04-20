#pragma once


// DlgFrozenUser 对话框

class DlgFrozenUser : public CDialogEx
{
	DECLARE_DYNAMIC(DlgFrozenUser)

public:
	DlgFrozenUser(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgFrozenUser();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_FROZEN_USER };
#endif
public:
	void SetNickName(CString NickName);
	void SetUserID(DWORD UserID);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	
protected:
	DWORD _UserID;
	int _FrozenTime;
	CString _NickName;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
