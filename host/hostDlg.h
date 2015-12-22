
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

using   namespace   std;

typedef std::function<void(int)> FNOperating;

class ObserverArray
{
public:
	//virtual void Update(int) = 0;
};

class SubjectArray
{
	int *FValue;
	vector<FNOperating> *FCallbackFunction;//m_allowOperating 观察者容器
public:
	SubjectArray(int arrayLen){
		FValue = new int[arrayLen];
		FCallbackFunction = new vector<FNOperating>[arrayLen];
	}
	~SubjectArray(){
		delete FValue;
		delete FCallbackFunction;
	}
	void Attach(int i, FNOperating fun){
		FCallbackFunction[i].push_back(fun);
		//fun(FValue[i]);
	}
	void Detach(int i, FNOperating fun){
		FCallbackFunction[i].pop_back();
	}
	void notify(int i){                //更新所有的观察者
		for (const auto&p : FCallbackFunction[i])
			p(FValue[i]);
	}
	void set(int i, int value){
		if (FValue[i] != value){
			FValue[i] = value;
			notify(i);
		}
	}
	int get(int i){
		return FValue[i];
	}
};

#include "optionDlg.h"
//class forstcastModel;
//class velocityForecast;
class xml;

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

class forecastModel;

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

	void forecast(int nCurpos);
	void savePoint(WORD temperature);
	void initSettingTemperature(WORD temperature);
	void showTemperature(WORD temperature);

	UINT timeToSecond(CString time);
	void dryRuning(void);
public:
	afx_msg void OnNMThemeChangedScrollbarHfigure(NMHDR *pNMHDR, LRESULT *pResult);
private:
	void setCommCtrlEnable(bool enabled, int minIndex, int maxIndex);
public:
	int m_nScrollPos;
	short int* toLP(short int * record);
	CString m_edRealHeatingRate;
	CString m_edSetHeatingRate;

	float m_temperature430, m_temperatureRoom;
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
class forecastModel :public ObserverArray
{
private:
	SubjectArray *m_pSubject;
	int time;
	FNOperating FSetTime;
public:
	forecastModel(SubjectArray *pSubject) : m_pSubject(pSubject){
		//FSetTime = bind1st(mem_fun(&forecastModel::setTime), this);
		FSetTime = bind(&forecastModel::setTime, this, std::placeholders::_1);
		pSubject->Attach(5, FSetTime);
	}
	~forecastModel()
	{
		m_pSubject->Detach(5, FSetTime);
	}
	int getTime(void)
	{
		return time;
	}
	void setTime(int time)
	{
		this->time = time;
	}
	virtual double _forecast(short int record[12][4], int nstartpos, ChostDlg* pThis) = 0;
};

class velocityForecast :
	public forecastModel
{
public:
	velocityForecast(SubjectArray *pSubject) :forecastModel(pSubject){}

	~velocityForecast(){}
	virtual double _forecast(short int record[12][4], int nstartpos, ChostDlg* pThis)
	{
		double v0, v, a, temper;
		short int rd[4];
		v0 = static_cast<double>(record[10][1] - record[0][1]) / (record[10][2] - record[0][2]);
		v = static_cast<double>(record[11][1] - record[1][1]) / (record[11][2] - record[1][2]);
		a = (v - v0) / (record[11][2] - record[10][2]);
		temper = record[3][1];
		for (int t = 2; t < this->getTime() * 10 * 6; t++){
			temper += v;// +a*(t + 0.5);
			rd[1] = temper;
			rd[2] = t + record[3][2];
			pThis->toLP(rd);
			if (t == 2)
				pThis->m_dcMem.MoveTo(rd[2] - pThis->m_nScrollPos, rd[1]);
			else
				pThis->m_dcMem.LineTo(rd[2] - pThis->m_nScrollPos, rd[1]);
		}
		return v;

	}
};

class Matrix{
	int m;
	int n;
	float * pval;
public:
	Matrix(void)
	{
		pval = nullptr;
	}
	~Matrix()
	{
		if (pval){
			delete []pval;
			pval = nullptr;
		}
	}
	Matrix(float * p, int n)
	{
		this->m = n - 1;
		this->n = 1;
		this->pval = new float[this->m*this->n];

		for (int i = 0; i<this->m*this->n; i++)
		{
			this->pval[i] = p[i + 1];
		}
	}
	void Initail(float * p,int n)
	{
		this->m = n - 1;
		this->n = 2;
		this->pval = new float[this->m*this->n];
		for (int i = 0; i<this->m; i++)
		{
			for (int j = 0; j<this->n; j++)
			{
				if (j == 0) *(this->pval + i*this->n + j) = (*(p + i) + *(p + i + 1))*(-0.5);
				else *(this->pval + i*this->n + j) = 1;
			}
		}
	}
	void RevTrans(Matrix &after)
	{
		after.m = n;
		after.n = m;
		after.pval = new float[m*n];
		for (int i = 0; i<after.m; i++)
		{
			for (int j = 0; j<after.n; j++)
			{
				*(after.pval + i*after.n + j) = *(pval + j*n + i);
			}
		}
	}
	void MultiAdd(Matrix &y,Matrix &r)
	{
		if (this->n == y.m)
		{
			r.m = this->m;
			r.n = y.n;
			r.pval = new float[r.m*r.n];

			for (int i = 0; i<r.m; i++)
			{
				for (int j = 0; j<r.n; j++)
				{
					float sum = 0;
					for (int k = 0; k<this->n; k++)
						sum += this->pval[i*this->n + k] * y.pval[k*y.n + j];
					*(r.pval + i*r.n + j) = sum;
				}
			}
		}
	}
	void Rever(Matrix& res)
	{
		if (this->m == this->n)
		{
			int m = this->m;
			int i, j;

			float *b = new float[m*m * 2];

			for (i = 0; i<m*m * 2; i++)
			{
				if (i % (2 * m)<m) b[i] = this->pval[(i / (2 * m))*m + (i % (2 * m))];
				else if ((i % (2 * m)) - m == (i / (2 * m))) b[i] = 1;
				else b[i] = 0;
			}

			float t;
			int k;
			for (i = 0; i<m; i++)
			{

				t = b[i * 2 * m + i];
				for (j = 0; j<2 * m; j++)//把第i行按第i列单位化
					b[i * 2 * m + j] /= t;


				for (j = 0; j<m; j++)
					if (j != i)
					{
						t = b[j * 2 * m + i] / b[i * 2 * m + i];
						for (k = 0; k<2 * m; k++)
							b[j * 2 * m + k] -= b[i * 2 * m + k] * t;
					}

			}

			res.m = this->m;
			res.n = this->n;
			res.pval = new float[m*m];

			for (i = 0; i<m; i++)
				for (j = 0; j<m; j++)
					res.pval[i*m + j] = b[i * 2 * m + j + m];
			delete[]b;
		}
	}
	float *getData(void)
	{
		return pval;
	}
};

const int FORECAST_DATA_LENGTH = 12;
class grayForecast :
	public forecastModel
{
public:
	grayForecast(SubjectArray *pSubject) :forecastModel(pSubject){}

	~grayForecast(){}
	virtual double _forecast(short int record[FORECAST_DATA_LENGTH][4], int nstartpos, ChostDlg* pThis)
	{
		double v0, v, a, temper;
		short int rd[4];
		float p[FORECAST_DATA_LENGTH], sumData[FORECAST_DATA_LENGTH];
		float aPar = 0, uPar = 0;

		toPData(record, p);

		Acculate(p,sumData);

		Matrix B, BT, BR,R,RES,T;
		B.Initail(sumData, FORECAST_DATA_LENGTH);

		B.RevTrans(BT);

		BT.MultiAdd(B,R);

		R.Rever(BR);

		Matrix YM(p, FORECAST_DATA_LENGTH);

		BR.MultiAdd(BT, T);
		T.MultiAdd(YM,RES);
		//OutputJUZHEN(RES);

		float *data = RES.getData();
		aPar = data[0];
		uPar = data[1];
		int m_Count = this->getTime() * 10 * 6;
		int totalcount = FORECAST_DATA_LENGTH + m_Count;
		float * rpdata0, *rpdata1;
		rpdata1 = new float[totalcount];
		rpdata0 = new float[totalcount];

		for (int i = 0; i<totalcount; i++)
		{
			if (i == 0) rpdata1[i] = sumData[i];
			else
			{
				rpdata1[i] = (p[0] - uPar / aPar)*exp(aPar*i*(-1)) + uPar / aPar;
			}
		}

		for (int i = 0; i<totalcount; i++)
		{
			if (i == 0) rpdata0[i] = p[i];
			else
			{
				rpdata0[i] = rpdata1[i] - rpdata1[i - 1];
			}
		}

		float ep[FORECAST_DATA_LENGTH];
		for (int i = 0; i<FORECAST_DATA_LENGTH; i++)
		{
			ep[i] = (p[i] - rpdata0[i]) / p[i] * 100;
		}
		for (int t = 2; t < m_Count; t++){
			rd[1] = rpdata0[FORECAST_DATA_LENGTH + t];
			rd[2] = t + record[3][2];
			pThis->toLP(rd);
			if (t == 2)
				pThis->m_dcMem.MoveTo(rd[2] - pThis->m_nScrollPos, rd[1]);
			else
				pThis->m_dcMem.LineTo(rd[2] - pThis->m_nScrollPos, rd[1]);
		}
		delete[]rpdata1;
		delete[]rpdata0;
		return ep[0];

	}

	void toPData(short int record[12][4], float *p)
	{
		int j = 11;
		for (int i = 11; i >-1; i--){
			*(p + j--) = record[i][0];
			if (j < 0) break;
			if (i > 0){
				int dt = record[i][2] - record[i - 1][2];
				for (int k = 1; k < dt; k++){
					*(p + j--) = record[i][0] - (record[i][0] - record[i - 1][0])*k / dt;
					if (j < 0) break;
				}
			}
			if (j < 0) break;
		}
	}

	void Acculate(float *pdata,float *sumData)
	{
		float sum{ *pdata };
		*sumData = *pdata;
		for (int i = 1; i<12; i++)
		{
			sum += *(pdata + i);
			*(sumData + i) = sum;
		}
	}

	void OutData(float *pdata, int count, int flag)
	{
		const char *title[]{
			"原始数列X(0)为："
				, "经过一次累加后，数列X(1)为："
				, "经计算得X^(1)数列为："
				, "经一次累减回归后，X^(0)数列为："
				, "其中与实际数据的相对误差为：(单位：％)"
				, "最终预测数列为："
		};
		//FILE * fp;

		//CString filename = "out.txt";

		//fp = fopen(filename, "a");
		//fprintf(fp, title[flag]);

		//for (int i = 0; i<count; i++)
		//{
		//	fprintf(fp, "%.5f ", *(pdata + i));
		//}
		//fprintf(fp, "\n");
		//fclose(fp);
	}
};

class xml
{
	vector< vector<int> > m_dryLines;
	int m_upTemperatureTime;  // 下位机向上位机传送实时温度时间间隔
	int m_downSetTemperatureTime;  // 向下位机下达设定温度时间间隔
	CString m_url;
	float m_430Room;
	int m_startDryMode;
	CTime m_startDryTime;
	forecastModel *m_pForecastMode;
	SubjectArray *m_xmlOption, *m_xmlValue;
	CComPtr<IXMLDOMDocument> spDoc; //DOM 
	CComPtr<IXMLDOMElement> spRootEle;

	// 取 xml 配制文件中 path 指定键对应的值
	CString getText(BSTR path)
	{
		CComPtr<IXMLDOMNode> spNode;
		BSTR tem;
		spDoc->selectSingleNode(path, &spNode);
		spNode->get_text(&tem);
		return tem;
	}

	// 设置 xml 配制文件中 path 指定键对应的值为 value
	void putText(BSTR path, CString value)
	{
		CComPtr<IXMLDOMNode> spNode;
		BSTR tem;
		spDoc->selectSingleNode(path, &spNode);
		tem = value.AllocSysString();
		spNode->put_text(tem);
		SysFreeString(tem); // 用完释放
	}

	void selectForecastModel(int mode)
	{
		if (m_pForecastMode != NULL){
			delete m_pForecastMode;
			m_pForecastMode = NULL;
		}
		switch (mode)
		{
		case 0:
			break;
		case 1:
			m_pForecastMode = new velocityForecast(m_xmlValue);
			break;
		case 2:
			m_pForecastMode = new grayForecast(m_xmlValue);
			break;
			
		}
	}
public:
	xml(){
		CoInitialize(NULL);
		m_xmlOption = new SubjectArray(6);
		m_xmlValue = new SubjectArray(6);
		m_xmlOption->Attach(5, bind(&xml::selectForecastModel, this, std::placeholders::_1));// 绑定[温度预测]观察者
		m_pForecastMode = NULL;
		m_startDryMode = 0;// 设置开始模式为 0-随时
		load();
	}
	~xml(){
		delete m_xmlOption;
		delete m_xmlValue;
		if (spDoc != NULL){
			spDoc.Release();
			spDoc = NULL;
		}
		CoUninitialize();
	}

	// 从配置文件读取干燥参数
	void load(void){
		//读取XML 
		spDoc.CoCreateInstance(CLSID_DOMDocument);
		VARIANT_BOOL vb;
		HRESULT hr = spDoc->load(CComVariant(OLESTR("dryHost.xml")), &vb); //加载XML文件 
		if (vb == VARIANT_TRUE){
			spDoc->get_documentElement(&spRootEle); //根节点 
			CComPtr<IXMLDOMNodeList> spNodeList, spNodeList1;

			// 取干燥曲线参数
			spDoc->selectNodes(OLESTR("/root/lines/*"), &spNodeList); //得到node2下的所有子节点 
			long nLen;
			spNodeList->get_length(&nLen); //子节点数 
			for (long i = 0; i < nLen; ++i) //遍历子节点 
			{
				CComPtr<IXMLDOMNode> spNode, chNode, chNode1, chNode2;
				spNodeList->get_item(i, &spNode);
				BSTR temperature, rate, time;
				spNode->get_firstChild(&chNode);
				chNode->get_text(&temperature); //节点值 
				chNode->get_nextSibling(&chNode1);
				chNode1->get_text(&rate); //节点值 
				chNode1->get_nextSibling(&chNode2);
				chNode2->get_text(&time); //节点值 

				CString s[3] = { temperature, rate, time };
				vector <int> line;
				for (int i = 0; i < 3; i++)
					line.push_back(_ttoi(s[i]));
				m_dryLines.push_back(line);

			}

			CString str;
			m_upTemperatureTime = _ttoi(getText(OLESTR("/root/uptemperaturetime")));
			m_downSetTemperatureTime = _ttoi(getText(OLESTR("/root/downsettemperaturetime")));
			m_url = getText(OLESTR("/root/url"));
			m_430Room = _ttof(getText(OLESTR("/root/room430temperature")));

			spDoc->selectNodes(OLESTR("/root/*"), &spNodeList1); //得到node2下的所有子节点 

			for (int i = 0; i < 6; i++){
				CComPtr<IXMLDOMNode> cpNode, spNodeAttrib0, spNodeAttrib1;
				CComPtr<IXMLDOMNamedNodeMap> spNameNodeMap;
				BSTR a1, a2;
				spNodeList1->get_item(i + 5, &cpNode);
				cpNode->get_attributes(&spNameNodeMap);
				spNameNodeMap->get_item(0, &spNodeAttrib0);
				spNodeAttrib0->get_text(&a1);
				m_xmlOption->set(i, _ttoi(a1));
				spNameNodeMap->get_item(1, &spNodeAttrib1);
				spNodeAttrib1->get_text(&a2);
				str = a2;
				m_xmlValue->set(i, _ttoi(str));
			}
		}

	}
	void save(void){
		CComPtr<IXMLDOMNodeList> spNodeList, spNodeList1;
		CComPtr<IXMLDOMNode> linesNode;

		// 取干燥曲线参数
		spDoc->selectNodes(OLESTR("/root/lines/*"), &spNodeList); //得到node2下的所有子节点 
		spDoc->selectSingleNode(OLESTR("/root/lines"), &linesNode); //得到node2下的所有子节点 
		long nLen;
		spNodeList->get_length(&nLen); //子节点数 
		for (long i = 0; i < nLen; ++i) //遍历子节点 
		{
			CComPtr<IXMLDOMNode> spNode;
			spNodeList->get_item(i, &spNode);
			linesNode->removeChild(spNode, NULL);
		}
		for (UINT i = 0; i < m_dryLines.size(); ++i) //遍历子节点 
		{
			CComPtr<IXMLDOMElement> lineEle, temperatureEle, rateEle, timeEle;
			CString s[3];
			BSTR bs[3];
			for (int j = 0; j<3; j++){
				s[j].Format(_T("%d"), m_dryLines[i][j]);
				bs[j] = s[j].AllocSysString();
			}
			spDoc->createElement(OLESTR("line"), &lineEle);
			spDoc->createElement(OLESTR("temperatureEle"), &temperatureEle);
			spDoc->createElement(OLESTR("rate"), &rateEle);
			spDoc->createElement(OLESTR("time"), &timeEle);
			temperatureEle->put_text(bs[0]);
			rateEle->put_text(bs[1]);
			timeEle->put_text(bs[2]);
			lineEle->appendChild(temperatureEle, NULL);
			lineEle->appendChild(rateEle, NULL);
			lineEle->appendChild(timeEle, NULL);

			linesNode->appendChild(lineEle, NULL);
		}
		CComPtr<IXMLDOMNode> spNode, spNode1, spNode2, spNode3;
		BSTR tem;
		CString str;

		// 上传实际温度时间间隔
		str.Format(_T("%d"), m_upTemperatureTime);
		putText(OLESTR("/root/uptemperaturetime"), str);

		// 下传设定温度时间间隔
		str.Format(_T("%d"), m_downSetTemperatureTime);
		putText(OLESTR("/root/downsettemperaturetime"), str);

		// 网址
		putText(OLESTR("/root/url"), m_url);

		// msp430 芯片内温度与环境温度之差
		str.Format(_T("%f"), m_430Room);
		putText(OLESTR("/root/room430temperature"), str);

		spDoc->selectNodes(OLESTR("/root/*"), &spNodeList1); //得到node2下的所有子节点 

		for (int i = 0; i<6; i++){
			CComPtr<IXMLDOMNode> cpNode, spNodeAttrib0, spNodeAttrib1;
			CComPtr<IXMLDOMNamedNodeMap> spNameNodeMap;
			BSTR a1, a2;
			spNodeList1->get_item(i + 5, &cpNode);
			cpNode->get_attributes(&spNameNodeMap);
			spNameNodeMap->get_item(0, &spNodeAttrib0);
			CString astr;
			astr.Format(_T("%d"), m_xmlOption->get(i));
			a1 = astr.AllocSysString();
			spNodeAttrib0->put_text(a1);
			SysFreeString(a1); // 用完释放

			spNameNodeMap->get_item(1, &spNodeAttrib1);
			astr.Format(_T("%d"), m_xmlValue->get(i));
			a2 = astr.AllocSysString();
			spNodeAttrib1->put_text(a2);
			SysFreeString(a2); // 用完释放
		}
		spDoc->save(CComVariant(OLESTR("dryHost.xml")));

	}
	SubjectArray *getOptions(void){
		return m_xmlOption;
	}
	SubjectArray *getValues(void){
		return m_xmlValue;
	}
	forecastModel *getForecastModel(void)
	{
		return m_pForecastMode;
	}

	vector< vector<int> > getdryLines(void)
	{
		return m_dryLines;
	}
	int getUpSendTemperatureTime(void)
	{
		return m_upTemperatureTime;  // 下位机向上位机传送实时温度时间间隔
	}
	void setUpSendTemperatureTime(int time)
	{
		m_upTemperatureTime = time;  // 下位机向上位机传送实时温度时间间隔
	}
	int getDownSendSettingTemperatureTime(void)
	{
		return m_downSetTemperatureTime;  // 向下位机下达设定温度时间间隔
	}
	void setDownSendSettingTemperatureTime(int time)
	{
		m_downSetTemperatureTime = time;  // 向下位机下达设定温度时间间隔
	}
	int getStartDryMode(void)
	{
		return m_startDryMode;  // 向下位机下达设定温度时间间隔
	}
	CTime getStartDryTime(void)
	{
		return m_startDryTime;  // 向下位机下达设定温度时间间隔
	}
	CString getUrl(void)
	{
		return m_url;
	}
	CString setUrl(CString url)
	{
		m_url = url;
	}
	float get430Temperature(void)
	{
		return m_430Room;
	}
	void set430Temperature(float temperature)
	{
		m_430Room = temperature;
	}
	void dialog(bool noDrring)
	{
		// TODO: 在此添加控件通知处理程序代码
		optionDlg dlg;
		//m_pMainWnd = &dlg;
		dlg.m_dryLines.assign(m_dryLines.begin(), m_dryLines.end());
		for (int i = 0; i < 6; i++){
			dlg.m_strAllowOperating[i] = m_xmlOption->get(i);
			dlg.m_allowOperatingValue[i] = m_xmlValue->get(i);
		}
		dlg.m_edTemperatureUpTime = m_upTemperatureTime;
		dlg.m_edSetTemperatureDownTime = m_downSetTemperatureTime;
		dlg.m_edtUrl = m_url;
		dlg.m_noDrring = noDrring;
		INT_PTR nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
			// TODO: 在此放置处理何时用
			//  “确定”来关闭对话框的代码
			if (noDrring){
				m_dryLines.clear();
				m_dryLines.assign(dlg.m_dryLines.begin(), dlg.m_dryLines.end());
				m_upTemperatureTime = dlg.m_edTemperatureUpTime;
				m_downSetTemperatureTime = dlg.m_edSetTemperatureDownTime;
				m_url = dlg.m_edtUrl;
				m_startDryMode = dlg.m_rdStartMode;
				m_startDryTime = dlg.m_startDryTime;
			}
			for (int i = 0; i < 6; i++){
				m_xmlOption->set(i, dlg.m_strAllowOperating[i]);
				m_xmlValue->set(i, dlg.m_allowOperatingValue[i]);
			}
			save();
		}
		else if (nResponse == IDCANCEL)
		{
			// TODO: 在此放置处理何时用
			//  “取消”来关闭对话框的代码
		}
		else if (nResponse == -1)
		{
			TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
			TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
		}
	}
};

