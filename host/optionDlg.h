#pragma once
#include "afxwin.h"
#include   <vector>   
#include "afxdtctl.h"
using   namespace   std;


// optionDlg �Ի���

class optionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(optionDlg)

public:
	optionDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~optionDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_OPTION };

private:
	int m_LineEditMode;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_strAllowOperating[6];// �Ƿ�������Ӧ���� 0 - ������ͣ 1-�¶��˲� 2-���¾��� 3-���±��� 4-�ֶ���ͣ 5-����Ԥ��
	int m_allowOperatingValue[6]; // ������������µ��¶���ֵ 4-��������¶������������ 5-Ԥ��ʱ��(����)
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
