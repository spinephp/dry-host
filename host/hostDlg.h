
// hostDlg.h : 头文件
//
#pragma once

#include "SerialPort.h"
#include "afxwin.h"
#include <functional>  
#include <memory>  
#include   <vector>   
using   namespace   std;   
	
// 定义向下位机发送的命令
const char cmdGetTemperature = '\x00';
const char cmdGetSettingTemperature = '\x10';
const char cmdSetSettingTemperature = '\x11';
const char cmdGetLineNo = '\x20';
const char cmdSetLineNo = '\x21';
const char cmdRunningStatus = '\x30';
const char cmdGetLineTime = '\x40';
const char cmdSetLineTime = '\x41';
const char cmdGetTime = '\x50';
const char cmdSetTime = '\x51';
const char cmdGetAll = '\xf0';

const CString dryRunningStatus[] = { _T("升温"), _T("保温"), _T("降温"), _T("暂停"), _T("结束") };

struct dryHead{
	char mark[3];
	char date[8];
	char time[6];
};

struct dryRecord{
	WORD record[4];
};
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

	CBrush m_redbrush,m_bluebrush,m_greenbrush,m_yellowbrush,*m_runbrush;  
	COLORREF m_redcolor,m_bluecolor,m_greencolor,m_textcolor;  

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	BYTE v_portin[13];
	short int v_lastTemperature;
	int v_index;
	int m_smoothvalue; // 允许温度突变最大值

	int m_curLineNo; // 当前干燥曲线段号
	UINT m_curLineTime; // 当前段号运行时间(分)
	long int m_TotalTimes; // 干燥曲线运行总时间(10秒)
	UINT m_curLinePauseTime; // 当前段号运行时间(分)
	UINT m_TotalPauseTime; // 干燥曲线运行总时间(分)
	int m_dataInvalid; // 无效的传输数据数量
	BOOL m_Pause; // 程序暂停

	typedef std::tr1::function<void (float)> FNsettingTemperature;  
	vector<FNsettingTemperature> m_fnSettingTemperature;

	CDC m_dcMem,m_dcMemTime,m_dcMemHG; //缓冲DC和背景DC

	//CPtrArray m_ptrArray[2];
	WORD m_record[4];
	vector<WORD> m_restoreTemperaturePoint;
	CFile m_file;
	CString m_filename;
	bool m_redrawingTemperatureLine;
	int m_nLeft, m_nTop, m_nWidth, m_nHeight; // 拆线绘图区域
	int m_tLeft, m_tTop, m_tWidth, m_tHeight; // 时间刻度区域
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
	vector< vector<int> > m_dryLines;
	int m_upTemperatureTime;  // 下位机向上位机传送实时温度时间间隔
	int m_downSetTemperatureTime;  // 向下位机下达设定温度时间间隔
	BOOL m_allowHandPause; // 是否允许手动暂停
	BOOL m_allowOperating[4]; // 是否允许相应操作 0 - 低温暂停 1-温度滤波 2-超温警报 3-超温报警
	int m_allowOperatingValue[4]; // 允许操作条件下的温度阈值
	void DrawTemperatureLine(void);
	afx_msg void OnBnClickedButtonOption();
	virtual BOOL DestroyWindow();
private:
	CComPtr<IXMLDOMDocument> spDoc; //DOM 
	CComPtr<IXMLDOMElement> spRootEle; 
	void loadXLM(void);
	void endDry(void);
public:
	void downSend(char cmd, WORD degress);
private:
	void saveXML(void);
public:
	afx_msg void OnBnClickedButtonStart();
private:
	void goNextLine(void);
public:
	void adjuster(double temperature);
private:
	void savePoint(double temperature);
	UINT timeToSecond(CString time);
public:
	afx_msg void OnNMThemeChangedScrollbarHfigure(NMHDR *pNMHDR, LRESULT *pResult);
private:
	void setCommCtrlEnable(bool enabled, int minIndex, int maxIndex);
};
