#pragma once
#include "afxwin.h"


// optionDlg 对话框

class optionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(optionDlg)

public:
	optionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~optionDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_cbLowPause;
	int m_cbLowPauseValue;
	int m_cbTemperatureFilterValue;
	CString m_cbTemperatureFilter;
	CString m_cbOverHeratingWarning;
	int m_cbOverHeratingValue;
	CString m_cbUltraLimitAlarming;
	int m_cbUltraLimitAlarmingValue;
	afx_msg void OnCbnSelchangeComboHandpause();
	CString m_cbHandPause;
	CString m_edLineTemperature;
	CString m_edLineHeatingRate;
	CString m_edLineTimeLength;
	CString m_ltLine;
	afx_msg void OnBnClickedAdd();
	afx_msg void OnChangeEditLine();
	CEdit m_edtLineTemperature;
};
