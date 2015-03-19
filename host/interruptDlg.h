#pragma once
#include "afxwin.h"
#include   <vector>   
using   namespace   std;   


// interruptDlg 对话框

class interruptDlg : public CDialogEx
{
	DECLARE_DYNAMIC(interruptDlg)

public:
	interruptDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~interruptDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_INTERRUPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_edLineName;
	CString m_edLineTime;
	float m_edTemperature;
	float m_edSetingTemperature;
	int m_rdAutoRun;
	void setSettingTemperature(float temperature);
	vector<CString> m_lineNames;
	CComboBox m_cbLines;
	int m_curSelLine;
	virtual BOOL OnInitDialog();
	CString m_edAllTime;
	afx_msg void OnBnClickedOk();
};
