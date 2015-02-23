
// hostDlg.h : ͷ�ļ�
//
#pragma once

#include "SerialPort.h"
#include "afxwin.h"
	
const char cmdGetTemperature = '\x00';
const char cmdSettingTemperature = '\x10';
const char cmdSettingLineNo = '\x20';
const char cmdRunningStatus = '\x30';

const CString dryRunningStatus[]={"����","����","����","��ͣ","����"};

const int m_dryLine[8][3]={50,5,0,50,0,16,85,5,0,85,0,16,95,5,0,95,0,16,120,3,0,120,0,16};

// ChostDlg �Ի���
class ChostDlg : public CDialogEx
{
// ����
public:
	ChostDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_HOST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	//{{AFX_MSG(ChostDlg)
	afx_msg LONG OnComm(WPARAM ch,LPARAM port);  //�����ַ�������Ϣ(WM_ONCOMM_RXCHAR)(���ڽ��ջ���������һ���ַ�)����Ӧ����
	//}}AFX_MSG

	void OnButtonSend();
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	BYTE v_portin[5];
	int v_index;

	int m_curLineNo; // ��ǰ�������߶κ�
	int m_curLineTime; // ��ǰ�κ�����ʱ��(��)
	int m_TotalTime; // ��������������ʱ��(��)
	BOOL GetSystemSerialComport(CArray<CString,CString> &comarray);

public:
	CString m_cbxPort;
	CString m_cbxBaudrate;
	CString m_cbxVerifybit;
	CString m_cbxDatabit;
	CString m_cbxStopbit;
	float m_lbTemperature;
	float m_lbSettingtemperature;
	afx_msg void OnCbnSelchangeCombo7();

	BOOL m_bPortOpen;
	CSerialPort m_SerialPort;
	CComboBox m_cbPort;
	CComboBox m_cbBaudrate;
	CComboBox m_cbVerifybit;
	CComboBox m_cbDatabit;
	CComboBox m_cbStopbit;
	CEdit m_lblTemperature;
	afx_msg void OnBnClickedButtonOpenport();
	afx_msg void OnBnClickedButtonCloseport();
	afx_msg void OnCbnSelchangeComboVerifybit();
	afx_msg void OnTimer(UINT nIDEvent);
};
