
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

	CBrush m_redbrush,m_bluebrush,m_greenbrush,m_yellowbrush,*m_runbrush;  
	COLORREF m_redcolor,m_bluecolor,m_greencolor,m_textcolor;  

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	BYTE v_portin[5];
	short int v_lastTemperature;
	int v_index;
	BOOL m_smooth; // �Ƿ������¶�ͻ��
	BOOL m_canPause; // �Ƿ����������ͣ
	int m_smoothvalue; // �����¶�ͻ�����ֵ

	int m_curLineNo; // ��ǰ�������߶κ�
	int m_curLineTime; // ��ǰ�κ�����ʱ��(��)
	int m_TotalTime; // ��������������ʱ��(��)
	long int m_TotalTimes; // ��������������ʱ��(��)
	int m_curLinePauseTime; // ��ǰ�κ�����ʱ��(��)
	int m_TotalPauseTime; // ��������������ʱ��(��)
	int m_dataInvalid; // ��Ч�Ĵ�����������
	BOOL m_Pause; // ������ͣ

	CDC m_dcMem,m_dcMemTime,m_dcMemHG; //����DC�ͱ���DC
	CPtrArray m_ptrArray[2];
	int m_nLeft, m_nTop, m_nWidth, m_nHeight; // ���߻�ͼ����
	int m_tLeft, m_tTop, m_tWidth, m_tHeight; // ʱ��̶�����
	BOOL GetSystemSerialComport(CArray<CString,CString> &comarray);

public:
	CString m_cbxPort;
	CString m_cbxBaudrate;
	CString m_cbxVerifybit;
	CString m_cbxDatabit;
	CString m_cbxStopbit;
	float m_lbTemperature;
	float m_lbSettingtemperature;

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
	CString m_edtRunning;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString m_edtArea;
	CString m_edtAreaTime;
	CString m_edtRunTime;
	CString m_edtAreaPauseTime;
	CString m_edtRunPauseTime;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
private:
	void SetHStaff(void);
public:
	void DrawTemperatureLine(void);
	afx_msg void OnBnClickedButtonOption();
};
