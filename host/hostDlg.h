
// hostDlg.h : ͷ�ļ�
//
#pragma once

#include "SerialPort.h"
#include "HttpClient.h"
#include "afxwin.h"
#include <functional>  
#include <memory>  
#include   <vector>   
#include "dryLine.h"
using   namespace   std;
	
// ��������λ�����͵�����
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

const CString dryRunningStatus[] = { _T("����"), _T("����"), _T("����"), _T("��ͣ"), _T("����") };
//const CString domain = _T("http://www.yrr8.com/woo/");
//const CString domain = _T("http://127.0.0.1/woo/");
const CString timeFormat = _T("%2d:%02d");
struct dryRecord{
	WORD record[4];
};

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
	COLORREF m_redcolor,m_bluecolor,m_greencolor,m_textcolor,m_auxiliary_line_color;  

	// ���ɵ���Ϣӳ�亯��
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
	int m_smoothvalue; // �����¶�ͻ�����ֵ

	int m_curLineNo; // ��ǰ�������߶κ�
	UINT m_curLineTime; // ��ǰ�κ�����ʱ��(��)
	long int m_TotalTimes; // ��������������ʱ��(10��)
	UINT m_curLinePauseTime; // ��ǰ�κ�����ʱ��(��)
	UINT m_TotalPauseTime; // ��������������ʱ��(��)
	int m_dataInvalid; // ��Ч�Ĵ�����������
	BOOL m_Pause; // ������ͣ
	int m_startDryMode;
	CTime m_startDryTime;

	CHttpClient *httpClinet; 

	typedef std::function<void(WORD)> FNTemperature;
	typedef std::function<void(float)> FNsettingTemperature;
	vector<FNTemperature> m_fnTemperature;
	vector<FNsettingTemperature> m_fnSettingTemperature;
	FNTemperature m_fnSavePoint, m_fnAdjuster,m_fnInitSettingTemperature;

	CDC m_dcMem,m_dcMemTime,m_dcMemHG; //����DC�ͱ���DC

	//CPtrArray m_ptrArray[2];
	WORD m_record[4];
	vector<WORD> m_restoreTemperaturePoint;
	CFile m_file;
	CString m_filename;
	bool m_redrawingTemperatureLine;
	int m_nLeft, m_nTop, m_nWidth, m_nHeight; // ���߻�ͼ����
	int m_tLeft, m_tTop, m_tWidth, m_tHeight; // ʱ��̶�����
	BOOL GetSystemSerialComport(CArray<CString,CString> &comarray);
	void findLineStart(int lineno, WORD *record);
	ULONGLONG findLinePoint(int time, WORD *record);
	void midDryData(WORD*rs, WORD*rm, WORD* re, WORD time);
	BOOL m_drawGrad;
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
	int m_upTemperatureTime;  // ��λ������λ������ʵʱ�¶�ʱ����
	int m_downSetTemperatureTime;  // ����λ���´��趨�¶�ʱ����
	CString m_url;
	BOOL m_allowHandPause; // �Ƿ������ֶ���ͣ
	BOOL m_allowOperating[5]; // �Ƿ�������Ӧ���� 0 - ������ͣ 1-�¶��˲� 2-���¾��� 3-���±��� 4-�ֶ���ͣ
	int m_allowOperatingValue[5]; // ������������µ��¶���ֵ 5-��������¶������������
	void DrawTemperatureLine(void);
	void drawGrad(int dx);
	afx_msg void OnBnClickedButtonOption();
	virtual BOOL DestroyWindow();
private:
	CComPtr<IXMLDOMDocument> spDoc; //DOM 
	CComPtr<IXMLDOMElement> spRootEle; 
	CString getText(BSTR path);
	void loadXLM(void);
	void putText(BSTR path, CString value);
	void saveXML(void);

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
	void saveDryDataToWeb(WORD* record);

	long strToJsonId(CString s);

	void setLineTime(int time);
	void setRunTime(int time);
	void setLinePauseTime(int time);
	void setRunPauseTime(int time);
	int setLineState(void);

	void adjuster(WORD temperature);
	void savePoint(WORD temperature);
	void initSettingTemperature(WORD temperature);
	void showTemperature(WORD temperature);

	UINT timeToSecond(CString time);
	void dryRuning(void);
public:
	afx_msg void OnNMThemeChangedScrollbarHfigure(NMHDR *pNMHDR, LRESULT *pResult);
private:
	void setCommCtrlEnable(bool enabled, int minIndex, int maxIndex);
	WORD* toLP(WORD * record);
public:
	CString m_edTemperature430;
	CString m_edTemperatureRoom;
	float m_430Room;
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
