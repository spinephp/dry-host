#pragma once
#include "afxwin.h"
#include   <vector>   
#include "afxdtctl.h"
using   namespace   std;


// optionDlg 对话框

class optionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(optionDlg)

public:
	optionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~optionDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_OPTION };

private:
	int m_LineEditMode;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_strAllowOperating[6];// 是否允许相应操作 0 - 低温暂停 1-温度滤波 2-超温警报 3-超温报警 4-手动暂停 5-允许预测
	int m_allowOperatingValue[6]; // 允许操作条件下的温度阈值 4-允许接收温度连续出错次数 5-预测时间(分钟)
	afx_msg void OnCbnSelchangeComboHandpause();
	CString m_edLineTemperature;
	CString m_edLineHeatingRate;
	CString m_edLineTimeLength;
	CString m_ltLine;
	afx_msg void OnBnClickedAdd();
	afx_msg void OnChangeEditLine();
	CEdit m_edtLineTemperature;
	afx_msg void OnBnClickedSave();
	CListBox m_lstLine;
	afx_msg void OnSelchangeListLine();
	afx_msg void OnBnClickedSave2();
	afx_msg void OnBnClickedEdit();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedUp();
	afx_msg void OnBnClickedDown();
	int m_edTemperatureUpTime;
	int m_edSetTemperatureDownTime;
	vector< vector<int> > m_dryLines;
private:
	int loadXML(void);
public:
	virtual BOOL OnInitDialog();
private:
	int digits(int n);
public:
	afx_msg void OnBnClickedOk();
private:
	void strToArray(CString str, vector<int>& intArray);
public:
	int m_rdStartMode;
	bool m_noDrring;
	CTime m_startDryTime;
	CDateTimeCtrl m_dtcDryStart;
	afx_msg void OnBnClickedRadioStartanytime();
	CString m_edtUrl;
};
