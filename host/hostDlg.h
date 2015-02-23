
// hostDlg.h : 头文件
//
#pragma once

#include "SerialPort.h"
#include "afxwin.h"
	
const char cmdGetTemperature = '\x00';
const char cmdSettingTemperature = '\x10';
const char cmdSettingLineNo = '\x20';
const char cmdRunningStatus = '\x30';

const CString dryRunningStatus[]={"升温","保温","降温","暂停","结束"};

const int m_dryLine[8][3]={50,5,0,50,0,16,85,5,0,85,0,16,95,5,0,95,0,16,120,3,0,120,0,16};

// ChostDlg 对话框
class ChostDlg : public CDialogEx
{
// 构造
public:
	ChostDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HOST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	//{{AFX_MSG(ChostDlg)
	afx_msg LONG OnComm(WPARAM ch,LPARAM port);  //串口字符接收消息(WM_ONCOMM_RXCHAR)(串口接收缓冲区内有一个字符)的响应函数
	//}}AFX_MSG

	void OnButtonSend();
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	BYTE v_portin[5];
	int v_index;

	int m_curLineNo; // 当前干燥曲线段号
	int m_curLineTime; // 当前段号运行时间(分)
	int m_TotalTime; // 干燥曲线运行总时间(分)
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
