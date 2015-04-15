
// hostDlg.h : ͷ�ļ�
//
#pragma once

#include "SerialPort.h"
#include "afxwin.h"
#include <functional>  
#include <memory>  
#include   <vector>   
using   namespace   std;   
	
// ��������λ�����͵�����
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

const CString dryRunningStatus[] = { _T("����"), _T("����"), _T("����"), _T("��ͣ"), _T("����") };

struct dryHead{
	char mark[3];
	char date[8];
	char time[6];
};

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
	COLORREF m_redcolor,m_bluecolor,m_greencolor,m_textcolor;  

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
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

	typedef std::tr1::function<void (float)> FNsettingTemperature;  
	vector<FNsettingTemperature> m_fnSettingTemperature;

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
	BOOL m_allowHandPause; // �Ƿ������ֶ���ͣ
	BOOL m_allowOperating[4]; // �Ƿ�������Ӧ���� 0 - ������ͣ 1-�¶��˲� 2-���¾��� 3-���±���
	int m_allowOperatingValue[4]; // ������������µ��¶���ֵ
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
