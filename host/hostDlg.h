
// hostDlg.h : 头文件
//
#pragma once

#include "SerialPort.h"
#include "HttpClient.h"
#include "afxwin.h"
#include <functional>  
#include <memory>  
#include   <vector>   
#include "dryLine.h"
#include "recordFile.h"
#include "xml.hpp"
//#include "grayForecast.hpp"
//#include "velocityForecast.hpp"

using   namespace   std;

typedef std::function<void(int)> FNOperating;

#include "optionDlg.h"

// 定义向下位机发送的命令
const char cmdGetTemperature = '\x00';
const char cmdGetRoomTemperature = '\x01';
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

const float temp430[] = { 28.3 };
const float tempRoom[] = { 20.6 };

const CString dryRunningStatus[] = { _T("升温"), _T("保温"), _T("降温"), _T("暂停"), _T("结束") };
//const CString domain = _T("http://www.yrr8.com/woo/");
//const CString domain = _T("http://127.0.0.1/woo/");
const CString timeFormat = _T("%2d:%02d");
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
	COLORREF m_redcolor,m_bluecolor,m_greencolor,m_textcolor,m_auxiliary_line_color;  

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	static bool ignoreWeb;
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
	//int m_startDryMode;
	//CTime m_startDryTime;

	CHttpClient *httpClinet; 

	typedef std::function<void(WORD)> FNTemperature;
	typedef std::function<void(float)> FNsettingTemperature;
	//typedef std::function<void(int)> FNOperating;
	vector<FNTemperature> m_fnTemperature;
	vector<FNsettingTemperature> m_fnSettingTemperature;
	FNTemperature m_fnSavePoint, m_fnAdjuster,m_fnInitSettingTemperature;

	//vector<FNOperating> m_fnOperating[6];
	//vector<FNOperating> m_fnOperatingValue[6];

	//CPtrArray m_ptrArray[2];
	short int m_record[4];
	vector<WORD> m_restoreTemperaturePoint;
	recordFile *m_recordFile;
	//CFile m_file;
	CString m_filename;
	bool m_redrawingTemperatureLine;
	int m_nLeft, m_nTop, m_nWidth, m_nHeight; // 拆线绘图区域
	int m_tLeft, m_tTop, m_tWidth, m_tHeight; // 时间刻度区域
	BOOL GetSystemSerialComport(CArray<CString,CString> &comarray);
	void findLineStart(int lineno, short int *record);
	ULONGLONG findLinePoint(int time, short int *record);
	void midDryData(short int*rs, short int*rm, short int* re, WORD time);
	BOOL m_drawGrad;
public:

	CDC m_dcMem, m_dcMemTime, m_dcMemHG; //缓冲DC和背景DC
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
	xml *m_xml;
	//BOOL m_allowOperating[6]; // 是否允许相应操作 0 - 低温暂停 1-温度滤波 2-超温警报 3-超温报警 4-手动暂停 5-温度预测
	//int m_allowOperatingValue[6]; // 允许操作条件下的温度阈值 4-允许接收温度连续出错次数 5-预测时间(分钟)
	void DrawTemperatureLine(void);
	void drawGrad(int dx);
	afx_msg void OnBnClickedButtonOption();
	virtual BOOL DestroyWindow();
private:

	void endDry(void);
public:
	void downSend(char cmd, WORD degress);
public:
	afx_msg void OnBnClickedButtonStart();
private:
	void goLine(int lineNo, int lineTime);
	void goNextLine(void);
private:
	void runInterruptDlg(void);
	int saveToWeb(CString url, CString data);
	int getMainIdFromWeb(void);
	void saveDryMainToWeb(CTime tm);
	void saveDryDataToWeb(short int* record);

	long strToJsonId(CString s);

	void setLineTime(int time);
	void setRunTime(int time);
	void setLinePauseTime(int time);
	void setRunPauseTime(int time);
	int setLineState(void);

	void adjuster(WORD temperature);

	int forecastSettingTempersturePoint[3][2];
	void forecast(int nCurpos);
	void showSettingTemperatureAsForecast(int nCurpos);
	void savePoint(WORD temperature);
	void initSettingTemperature(WORD temperature);
	void showTemperature(WORD temperature);
	void printReport(void);
	UINT timeToSecond(CString time);
	void dryRuning(void);
public:
	afx_msg void OnNMThemeChangedScrollbarHfigure(NMHDR *pNMHDR, LRESULT *pResult);
private:
	void setCommCtrlEnable(bool enabled, int minIndex, int maxIndex);
public:
	//int m_nScrollPos;
	short int* toLP(short int * record);
	CString m_edRealHeatingRate;
	CString m_edSetHeatingRate;

	float m_temperature430, m_temperatureRoom;

	WORD m_sameTemperature;
	short int m_sameTemperatureTimes;

	void dryBegin(void);
	CButton m_btnStartDry;
	afx_msg void OnEnSetfocusEditTemperature();
	double getSettingTemperature(int lineNo, int lineTime, float roomTemperature = -273);
	int processInterruptFile(int lineNo, int lineTime);
	int m_edtSendTimes;
	int m_edtReceviTimes;
	int m_edtSendFarTimes;
	int m_edtReciveValidTimes;
	int dryMainId = -1;

};

class dryLines
{
	short FCurrent;
	long FTime;
	vector< vector<int> > m_dryLines;
public:
	dryLines(int lineCount)
	{
	}
	~dryLines()
	{
	}
	short getCurrent(void)
	{
		return FCurrent;
	}
	void setCurrent(short lineNo)
	{
		FCurrent = lineNo;
	}
	UINT operator ++ (int)
	{
		FTime++;
	}
	vector< vector<int> >& getdryLines(void)
	{
		return m_dryLines;
	}
	UINT getTotal(void)
	{

	}
};