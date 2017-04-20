#pragma once


// DlgNotice 对话框

class DlgNotice : public CDialogEx
{
	DECLARE_DYNAMIC(DlgNotice)

public:
	DlgNotice(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgNotice();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SEND_MSG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
