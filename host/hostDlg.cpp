

// hostDlg.cpp : 实现文件
//

// 在程序的文件夹下生成MSXML3.TLH和MSXML3.TLI两个文件;  

#include "stdafx.h"
#include "host.h"
#include "hostDlg.h"
//#include "optionDlg.h"
#include "interruptDlg.h"
#include "afxdialogex.h"
#include "windows.h"
#include <stdlib.h> 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()

};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    // 处理MFC默认的数据交换   
    CDialogEx::DoDataExchange(pDX);   
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// ChostDlg 对话框


bool ChostDlg::ignoreWeb = false;

ChostDlg::ChostDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ChostDlg::IDD, pParent)
	, m_cbxPort(_T(""))
	, m_cbxBaudrate(_T("9600"))
	, m_cbxVerifybit(_T("无"))
	, m_cbxDatabit(_T("8"))
	, m_cbxStopbit(_T("1"))
	, m_lbTemperature(0)
	, m_lbSettingtemperature(-100)
	, m_edtRunning(_T("未运行"))
	, m_edtArea(_T(""))
	, m_edtAreaTime(_T("0:0"))
	, m_edtRunTime(_T("0:0"))
	, m_edtAreaPauseTime(_T("0:0"))
	, m_edtRunPauseTime(_T("0:0"))
	, m_edRealHeatingRate(_T(""))
	, m_edSetHeatingRate(_T(""))
	, m_edtSendTimes(0)
	, m_edtReceviTimes(0)
	, m_edtSendFarTimes(0)
	, m_edtReciveValidTimes(0)
	, m_sameTemperature(0)
	, m_sameTemperatureTimes(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void ChostDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO_PORT, m_cbxPort);
	DDX_CBString(pDX, IDC_COMBO_BAUDRATE, m_cbxBaudrate);
	DDX_CBString(pDX, IDC_COMBO_VERIFYBIT, m_cbxVerifybit);
	DDX_CBString(pDX, IDC_COMBO_DATABIT, m_cbxDatabit);
	DDX_CBString(pDX, IDC_COMBO_STOPBIT, m_cbxStopbit);
	DDX_Text(pDX, IDC_EDIT_TEMPERATURE, m_lbTemperature);
	DDX_Text(pDX, IDC_EDIT_SETTINGTEMPERATURE, m_lbSettingtemperature);
	DDX_Text(pDX, IDC_EDIT_RUNNING, m_edtRunning);
	DDX_Text(pDX, IDC_EDIT_AREA, m_edtArea);
	DDX_Control(pDX, IDC_COMBO_PORT, m_cbPort);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_cbBaudrate);
	DDX_Control(pDX, IDC_COMBO_VERIFYBIT, m_cbVerifybit);
	DDX_Control(pDX, IDC_COMBO_DATABIT, m_cbDatabit);
	DDX_Control(pDX, IDC_COMBO_STOPBIT, m_cbStopbit);
	DDX_Control(pDX, IDC_EDIT_TEMPERATURE, m_lblTemperature);
	DDX_Text(pDX, IDC_EDIT_AREATIME, m_edtAreaTime);
	DDX_Text(pDX, IDC_EDIT_RUNTIME, m_edtRunTime);
	DDX_Text(pDX, IDC_EDIT_AREAPAUSETIME, m_edtAreaPauseTime);
	DDX_Text(pDX, IDC_EDIT_RUNPAUSETIME, m_edtRunPauseTime);
	DDX_Text(pDX, IDC_EDIT_TEMPERATURE430, m_edRealHeatingRate);
	DDX_Text(pDX, IDC_EDIT_TEMPERATUREROOM, m_edSetHeatingRate);
	DDX_Control(pDX, IDC_BUTTON_START, m_btnStartDry);
	DDX_Text(pDX, IDC_EDIT_SENDTIMES, m_edtSendTimes);
	DDX_Text(pDX, IDC_EDIT_RECIVETIMES, m_edtReceviTimes);
	DDX_Text(pDX, IDC_EDIT_SENDFARTIME, m_edtSendFarTimes);
	DDX_Text(pDX, IDC_EDIT_RECIVEVALIDTIME, m_edtReciveValidTimes);
}

BEGIN_MESSAGE_MAP(ChostDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	//{{AFX_MSG_MAP(ChostDlg)
	ON_MESSAGE(WM_COMM_RXCHAR,OnComm)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_OPENPORT, &ChostDlg::OnBnClickedButtonOpenport)
	ON_BN_CLICKED(IDC_BUTTON_CLOSEPORT, &ChostDlg::OnBnClickedButtonCloseport)
	ON_WM_CTLCOLOR()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_OPTION, &ChostDlg::OnBnClickedButtonOption)
	ON_BN_CLICKED(IDC_BUTTON_START, &ChostDlg::OnBnClickedButtonStart)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SCROLLBAR_HFIGURE, &ChostDlg::OnNMThemeChangedScrollbarHfigure)

	// 禁止以下编辑框获得焦点
	ON_EN_SETFOCUS(IDC_EDIT_TEMPERATURE, &ChostDlg::OnEnSetfocusEditTemperature)
	ON_EN_SETFOCUS(IDC_EDIT_SETTINGTEMPERATURE, &ChostDlg::OnEnSetfocusEditTemperature)
	ON_EN_SETFOCUS(IDC_EDIT_TEMPERATURE430, &ChostDlg::OnEnSetfocusEditTemperature)
	ON_EN_SETFOCUS(IDC_EDIT_TEMPERATUREROOM, &ChostDlg::OnEnSetfocusEditTemperature)
	ON_EN_SETFOCUS(IDC_EDIT_AREA, &ChostDlg::OnEnSetfocusEditTemperature)
	ON_EN_SETFOCUS(IDC_EDIT_RUNNING, &ChostDlg::OnEnSetfocusEditTemperature)
	ON_EN_SETFOCUS(IDC_EDIT_AREATIME, &ChostDlg::OnEnSetfocusEditTemperature)
	ON_EN_SETFOCUS(IDC_EDIT_RUNTIME, &ChostDlg::OnEnSetfocusEditTemperature)
	ON_EN_SETFOCUS(IDC_EDIT_AREAPAUSETIME, &ChostDlg::OnEnSetfocusEditTemperature)
	ON_EN_SETFOCUS(IDC_EDIT_RUNPAUSETIME, &ChostDlg::OnEnSetfocusEditTemperature)
	ON_EN_SETFOCUS(IDC_EDIT_SENDTIMES, &ChostDlg::OnEnSetfocusEditTemperature)
	ON_EN_SETFOCUS(IDC_EDIT_SENDFARTIME, &ChostDlg::OnEnSetfocusEditTemperature)
	ON_EN_SETFOCUS(IDC_EDIT_RECIVETIMES, &ChostDlg::OnEnSetfocusEditTemperature)
	ON_EN_SETFOCUS(IDC_EDIT_RECIVEVALIDTIME, &ChostDlg::OnEnSetfocusEditTemperature)
END_MESSAGE_MAP()

// ChostDlg 消息处理程序

BOOL ChostDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	CArray<CString ,CString> comPort;
	GetSystemSerialComport(comPort);
	int size=comPort.GetSize();
	for(int i=0;i<size;i++){
		CString s = comPort.GetAt(i);
		m_cbPort.AddString(s);
	}

	m_cbPort.SetCurSel(0); //初始选择串口1
	m_bPortOpen = FALSE;

	setCommCtrlEnable(size!=0,3,14);
	GetDlgItem(IDC_BUTTON_CLOSEPORT)->EnableWindow(m_bPortOpen);

	m_curLineNo = -1;

	v_index = 0;
	v_lastTemperature = -100;

	m_redcolor=RGB(255,0,0);                      // 红色  
	m_bluecolor=RGB(0,0,255);                     // 蓝色  
	m_greencolor = RGB(0, 255, 0);                     // 绿色  
	m_auxiliary_line_color = RGB(220, 220, 220);                     // 绿色  
	m_textcolor = RGB(255, 255, 255);                 // 文本颜色设置为白色  
	m_redbrush.CreateSolidBrush(m_redcolor);      // 红色背景色  
	m_bluebrush.CreateSolidBrush(m_bluecolor);    // 蓝色背景色  
	m_greenbrush.CreateSolidBrush(m_greencolor);    // 蓝色背景色  
	m_yellowbrush.CreateSolidBrush(RGB(255,255,0));    // 黄色背景色  

	CRect rect;
	CPaintDC dc(this);
	GetDlgItem(IDC_STATIC_RUNNINGTIME)->GetWindowRect(&rect);//获取控件相对于屏幕的位置
	ScreenToClient(rect);//转化为对话框上的相对位置
	m_nLeft = rect.left+40;
	m_nTop = rect.top+20;
	m_nWidth = rect.Width()-50;
	m_nHeight = rect.Height()-140;

	m_redrawingTemperatureLine = FALSE;

    m_dcMem.CreateCompatibleDC(&dc);
	//m_dcBack.CreateCompatibleDC(GetDC());
	CBitmap tmp,tmpp;
	//tmp.CreateCompatibleBitmap(GetDC(), m_nWidth, m_nHeight);
	tmpp.CreateCompatibleBitmap(&dc, m_nWidth, m_nHeight);
	//m_dcBack.SelectObject(&tmp);
	m_dcMem.SelectObject(&tmpp);
	//tmp.DeleteObject();
	tmpp.DeleteObject();
	
	m_dcMem.SetMapMode(MM_ANISOTROPIC);
	m_dcMem.SetViewportExt(m_nWidth, m_nHeight);
	m_dcMem.SetWindowExt(m_nWidth, -m_nHeight);
	m_dcMem.SetViewportOrg(0,0);
	m_dcMem.SetWindowOrg(0,m_nHeight);

	m_tLeft = m_nLeft-20;
	m_tTop = m_nTop+m_nHeight+8;
	m_tWidth = m_nWidth+20;
	m_tHeight = 16;

    m_dcMemTime.CreateCompatibleDC(&dc);
	tmp.CreateCompatibleBitmap(&dc, m_tWidth, m_tHeight);
	m_dcMemTime.SelectObject(&tmp);
	tmp.DeleteObject();

    m_dcMemHG.CreateCompatibleDC(&dc);
	tmp.CreateCompatibleBitmap(&dc, m_nWidth, 6);
	m_dcMemHG.SelectObject(&tmp);
	tmp.DeleteObject();

	m_drawGrad = true;

	CScrollBar* pScrollBar = (CScrollBar*)GetDlgItem(IDC_SCROLLBAR_HFIGURE);
	pScrollBar->SetScrollRange(0,32767);//滑块移动的位置为0——34560；32767
	DrawTemperatureLine();
	SetHStaff();
	drawGrad(0);

	httpClinet = new CHttpClient(L"HttpClinet");

	m_xml = new xml();
	m_smoothvalue = (m_xml->getValues()->get(1) + 1) * 16; // 设置 10 秒内温度变化阀值在 1℃ 之内

	//m_fnSavePoint = bind1st(mem_fun(&ChostDlg::savePoint), this);
	m_fnSavePoint = bind(&ChostDlg::savePoint, this, std::placeholders::_1);
	m_fnTemperature.push_back(m_fnSavePoint);

	//m_fnAdjuster = bind1st(mem_fun(&ChostDlg::adjuster), this);
	m_fnAdjuster = bind(&ChostDlg::adjuster, this, std::placeholders::_1);
	m_fnTemperature.push_back(m_fnAdjuster);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void ChostDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void ChostDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // 用于绘制的设备上下文
		CSize size;
		CScrollBar* pScrollBar = (CScrollBar*)GetDlgItem(IDC_SCROLLBAR_HFIGURE);
		for(int n=-50,i=m_nTop+m_nHeight-1;i>30;i-=10,n+=10){
			TCHAR s[5];
			dc.MoveTo(m_nLeft - 5, i);
			dc.LineTo(m_nLeft,i);
			if (n % 50 == 0){
				int ns;
				swprintf(s,5,L"%d",n);
				ns = wcslen(s);
				size = dc.GetTextExtent(s, ns);
				dc.TextOut(m_nLeft - 7 - size.cx, i - size.cy / 2, s, ns);
			}
		}

		// 显示水平标尺
		dc.BitBlt(m_nLeft, m_nTop+m_nHeight, m_nWidth, m_nHeight+6, &m_dcMemHG, 0,0, SRCCOPY);// 显示刻度线
		dc.BitBlt(m_tLeft, m_tTop, m_tWidth, m_tHeight, &m_dcMemTime, 0,0, SRCCOPY); // 显示刻度时间

		// 绘制温度曲线
		dc.BitBlt(m_nLeft, m_nTop, m_nWidth, m_nHeight, &m_dcMem, 0,0, SRCCOPY);

		CDialogEx::OnPaint();
	}
}

/*
 * 绘制网格线
 * @param dx - int ,指定滚动棒位置
 * @return void
 */
void ChostDlg::drawGrad(int dx)
{
	if (!m_drawGrad)
		return;
	HPEN hPen1 = CreatePen(PS_SOLID, 1, m_auxiliary_line_color);
	HPEN hPen2 = CreatePen(PS_SOLID, 1, RGB(235,235,235));
	HPEN oldPen = (HPEN)m_dcMem.SelectObject(hPen1);
	for (int i = m_nTop + m_nHeight - 1,j=0; i > 30; i -= 10,j++){
		if (j%5==0)
			m_dcMem.SelectObject(hPen1);
		else
			m_dcMem.SelectObject(hPen2);
		m_dcMem.MoveTo(0, i - 31);
		m_dcMem.LineTo(m_nWidth, i - 31);
	}
	m_dcMem.SelectObject(hPen1);
	for (int i = 60 - dx%60; i<m_tWidth; i += 60){
			m_dcMem.MoveTo(i, 0);
			m_dcMem.LineTo(i, m_nHeight);
	}
	m_dcMem.SelectObject(oldPen);
	DeleteObject(hPen1);
	DeleteObject(hPen2);

}
//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR ChostDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 运行中断对话框
void ChostDlg::runInterruptDlg(void)
{
	m_curLineNo = v_portin[2] - 1;
	if (m_curLineNo >= 0){// 干燥已经开始
		// 把干燥参数传递给断点对话框
		div_t h_m;
		interruptDlg dlg;
		vector< vector<int> >&m_dryLines = m_xml->getdryLines();
		for (int i = 0; i<m_dryLines.size(); i++){
			CString str;
			str.Format(_T("%d℃ %s"), m_dryLines[i][0], m_dryLines[i][1]>0 ? _T("升温") : (m_dryLines[i][1] == 0 ? _T("保温") : _T("降温")));
			dlg.m_lineNames.push_back(str);
		}
		dlg.m_dryLines.assign(m_dryLines.begin(), m_dryLines.end());
		dlg.m_curSelLine = m_curLineNo;
		dlg.m_edLineName.Format(_T("%d℃ %s"), m_dryLines[m_curLineNo][0], dryRunningStatus[0]);
		m_lbSettingtemperature = (*(WORD*)(v_portin + 4))*0.0625;
		dlg.m_edSetingTemperature = m_lbSettingtemperature;
		dlg.m_roomTemperature = m_temperatureRoom;
		h_m = div(*(WORD*)(v_portin + 8), 60);
		m_edtRunTime.Format(timeFormat, h_m.quot, h_m.rem);
		dlg.m_edAllTime = m_edtRunTime;
		h_m = div(*(WORD*)(v_portin + 6), 60);
		m_edtAreaTime.Format(timeFormat, h_m.quot, h_m.rem);
		dlg.m_edLineTime = m_edtAreaTime;
		dlg.m_edTemperature = m_lbTemperature;
		dlg.setFile(m_recordFile);
		INT_PTR nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
			// TODO: 在此放置处理何时用
			//  “确定”来关闭对话框的代码
			// 根据断点对话框参数，设置如何继续干燥
			short int record[4];
			m_curLinePauseTime = 0;
			m_TotalTimes = timeToSecond(dlg.m_edAllTime);
			m_TotalPauseTime = 0;
			m_curLineNo = dlg.m_curSelLine;
			dryMainId = getMainIdFromWeb();
			m_recordFile = dlg.m_file;
			switch (dlg.m_rdAutoRun){
			case 0:// 以文件
				m_Pause = false;
				if (m_recordFile->recordCount()>0){
					m_recordFile->seekRecord(0, CFile::begin);
					m_recordFile->readRecord(record,1);
					m_TotalTimes = record[0];
					m_lbSettingtemperature = record[1];
					m_lbTemperature = record[2];
					m_curLineNo = record[3];
					findLineStart(m_curLineNo, record);
					m_curLineTime = m_TotalTimes - record[0];
				}
				SetTimer(1, 60000, NULL);
				break;
			case 1: // in setting temperature
				m_TotalTimes = 0;
				m_curLineTime = 0;
				m_Pause = false;
				m_curLineNo = 0;
				m_lbSettingtemperature = tempRoom[0];
				while (m_lbSettingtemperature < dlg.m_edSetingTemperature){
					OnTimer(1);
				}
				if (processInterruptFile(m_curLineNo, m_curLineTime)){
					SetTimer(1, 60000, NULL);
				}
				break;
			case 2: // in time
				if (processInterruptFile(m_curLineNo, timeToSecond(dlg.m_edLineTime)/10)){
					dryRuning();
				}
				break;
			case 3: // from current line begin
				if (processInterruptFile(m_curLineNo, 0)){
					m_curLineNo--;
					goNextLine();
					SetTimer(1, 60000, NULL);
				}
				break;
			case 4: // from head
				OnBnClickedButtonStart();
				break;
			}
			DrawTemperatureLine();
			UpdateData();
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			setCommCtrlEnable(TRUE, 18, 38);
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
	else{
		// 使能“开始干燥”按钮
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	}
}

//字符接收消息响应函数
LONG ChostDlg::OnComm(WPARAM ch, LPARAM port)
{
		v_portin[v_index++] = ch;
	if(v_portin[v_index-1]==0xfe && (v_index==5 || v_index==13)){ //接收完毕
		CString str; 
		char cmd = v_portin[1];
		short int temperature;
		float temper;
		SubjectArray *m_xmlOption = m_xml->getOptions();
		SubjectArray *m_xmlValue = m_xml->getValues();
		v_index = 0;
		m_edtReceviTimes++;
		switch(cmd){
			case cmdGetTemperature: // 当前实际温度
				temperature = *(short int*)(v_portin+2);
				showTemperature(temperature);
				if (v_lastTemperature == -100)
					v_lastTemperature = temperature;
				if (m_xmlOption->get(1) && temperature > v_lastTemperature - m_smoothvalue && temperature < v_lastTemperature + m_smoothvalue || !m_xmlOption->get(1) && temperature > -800 && temperature < 3200){ // ，为正常，否则认为是由其它因素造成的误差
					if(m_curLineNo>-1){// 干燥已开始
						for (auto fnTemperature : m_fnTemperature)
							(fnTemperature)(temperature);
					}else{
						m_edtRunning = L"未开始";
						m_runbrush = NULL;
					}
					v_lastTemperature = temperature;
				}
				else{
					static int m_temperatureContinueError = 0;
					m_dataInvalid++;
					m_edtReciveValidTimes++;
					m_temperatureContinueError++;
					if (m_temperatureContinueError >= m_xmlValue->get(4) + 1){
						m_temperatureContinueError = 0;
						v_lastTemperature = temperature;
					}
				}
				UpdateData(FALSE);  //将接收到的字符显示在接受编辑框中
				break;
			case cmdGetRoomTemperature: // MSP430片内温度
				temperature = *(short int*)(v_portin + 2);
				temper = temperature / 1.45266 - 278.75;
				m_temperature430 = temper;
				m_temperatureRoom = temper - m_xml->get430Temperature();
				UpdateData(FALSE);
				break;
			case cmdGetSettingTemperature: // 下位机当前的设定温度
				temperature = *(short int*)(v_portin + 2);
				temper = temperature*0.0625;
				for (auto fnSettingTemperature : m_fnSettingTemperature)
					(fnSettingTemperature)(temper);
				break;
			case cmdGetLineNo: // 下位机当前段号
				break;
			case cmdGetAll: // 下位机全部信息

				// 显示实际温度
				//m_lbTemperature = (*(WORD*)(v_portin + 10))*0.0625;
				showTemperature((*(WORD*)(v_portin + 10)));

				str.Format(L"DS18B20 温度为 %2.1f, 要对室温进行较准吗?", m_lbTemperature);
				if (MessageBox(str, L"室温较准", MB_ICONQUESTION | MB_OKCANCEL) == IDOK){
					m_xml->set430Temperature(m_temperature430 - m_lbTemperature);
					m_temperatureRoom = m_lbTemperature;
				}
				UpdateData(FALSE);

				switch (v_portin[2]){
				case 0xff: // 干燥未进行
				case 0x00: // 干燥未进行
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
					break;
				default:
					runInterruptDlg();
				}
				break;
			case cmdGetLineTime: // 下位机当前段运行时间,单位：分
				for(UINT i=0;i<m_fnSettingTemperature.size();i++){
					FNsettingTemperature fun = m_fnSettingTemperature[i];
					(fun)(*(short int*)(v_portin+2));
				}
				break;
			case cmdGetTime: // 下位机当前段运行时间,单位：分
				for(UINT i=0;i<m_fnSettingTemperature.size();i++){
					FNsettingTemperature fun = m_fnSettingTemperature[i];
					(fun)(*(short int*)(v_portin+2));
				}
				break;
			case cmdRunningStatus: // 下位机当前段运行时间,单位：分
				break;
		}
	}
	return 0;
}

/*
* 查找段开始记录
* @param lineno - 整数，指定要查找的段号
*        record - 指向 WORD 的指针，保存查找到的段 lineno 开始记录
* @return 无
*/
void ChostDlg::findLineStart(int lineno, short int *record){
	ULONGLONG nSizes = m_recordFile->getRecordSize();
	ULONGLONG p = nSizes / 2, p0 = 0, p1 = nSizes;
	while (p1 > p0){
		m_recordFile->seekRecord(p, CFile::begin);
		m_recordFile->readRecord(record, 1);
		if (record[3] < lineno){
			p0 = p;
		}
		else{
			p1 = p;
		}
		p = (p1 + p0) / 2;
	}
}

/*
* 查找段开始记录
* @param lineno - 整数，指定要查找的段号
*        record - 指向 WORD 的指针，保存查找到的段 lineno 开始记录
* @return 无
*/
ULONGLONG ChostDlg::findLinePoint(int time, short int *record){
	ULONGLONG pos = sizeof(dryHead);
	ULONGLONG nSizes = time; //m_ptrArray[0].GetSize();
	ULONGLONG p = nSizes / 2, p0 = 0, p1 = nSizes;
	short int record1[4];
	do{
		pos = m_recordFile->seekRecord(p, CFile::begin);
		pos += m_recordFile->readRecord(record,1);
		if (record[2] == time)
			break;
		else if (record[2] < time){
			if (p == p0) break;
			p0 = p;
		}
		else{
			if (p == p1) break;
			p1 = p;
		}
		p = (p1 + p0) / 2;
	}while (p1 > p0);
	if (p>1 && record[2] > time){
		m_recordFile->seekRecord(-2, CFile::current);
		m_recordFile->readRecord(record, 1);
	}
	if (m_recordFile->readRecord(record1, 1)){
		m_recordFile->seekRecord(-1, CFile::current);
		if (record1[2] > record[2] + 1){
			midDryData(record, record1, record,time);
		}
	}
	return pos;
}

void ChostDlg::midDryData(short int*rs, short int*rm, short int* re, WORD time)
{
	rm[0] = (WORD)(rs[0] + double(time - rs[2]) / double(re[2] - rs[2])*(re[0] - rs[0]));
	rm[1] = (WORD)(rs[1] + double(time - rs[2]) / double(re[2] - rs[2])*(re[1] - rs[1]));
	rm[2] = time;
}

void  ChostDlg::OnTimer(UINT nIDEvent)
{
	float at{};
	CTime time;
	vector< vector<int> >&m_dryLines{ m_xml->getdryLines() };
	switch(nIDEvent){
	case 0: // 10 秒计时一次
		downSend(cmdGetTemperature,0);// 通知下位机发送当前实际温度
		m_TotalTimes ++;
		break;
	case 1: // 1 分钟计时一次
		setRunTime(m_TotalTimes + 1);
		if (m_Pause){ // 程序暂停
			setLinePauseTime(m_curLinePauseTime+1);
			setRunPauseTime(m_TotalPauseTime+1);
			DrawTemperatureLine();
		}else{ // 程序未暂停
			setLineTime(m_curLineTime + 1);
			at = m_dryLines[m_curLineNo][1] / 60.0;
		}
		if(m_dryLines[m_curLineNo][1]){ // 升温
			float t = m_lbSettingtemperature;
			t += at;
			if((short int)t<m_dryLines[m_curLineNo][0]){// 未升到段指定温度
				if((short int)t > (short int)m_lbSettingtemperature){
					downSend(cmdSetSettingTemperature,(WORD)(t*16));// 传送当前设定温度给下位机
				}
				m_lbSettingtemperature = t;
			}else{ // 升温结束
				goNextLine();
			}
		}else if(m_curLineTime>m_dryLines[m_curLineNo][2]*60){ // 保温时间到
			goNextLine();
		}
		UpdateData(FALSE);     //读入编辑框的数据
		downSend(cmdGetRoomTemperature, 0);// 通知下位机发送当前室温
		break;
	case 2: // 定时干燥开始倒计时处理
		time = CTime::GetCurrentTime();
		CTime m_startDryTime = m_xml->getStartDryTime();
		if (m_startDryTime > time){ // 定时干燥时间未到
			CString caption{ L"倒计时中..." };
			CTimeSpan t{ m_startDryTime - time };
			if (t.GetTotalSeconds() < 86400) // 定时时间少于 1 天，显示倒计时时间
				caption.Format(L"%02d:%02d:%02d", t.GetHours(), t.GetMinutes(), t.GetSeconds());
			SetDlgItemText(IDC_BUTTON_START, caption);
		}
		else{ // 定时时间到，开始干燥
			KillTimer(2);
			dryBegin();
		}
	}
	v_index = 0;
	CDialogEx::OnTimer(nIDEvent);
}

void ChostDlg::OnButtonSend() 
{
 // TODO: Add your control notification handler code here
 if(!m_SerialPort.IsOpen())
 return ;      //检测串口是否打开
 UpdateData(TRUE);     //读入编辑框的数据
 //m_SerialPort.WriteToPort((LPCTSTR)m_strTxMsg);  //发送数据
}

BOOL ChostDlg::GetSystemSerialComport(CArray<CString,CString> &comarray)
{
 HKEY hKey=NULL;
 comarray.RemoveAll();
 if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"),0,KEY_READ,&hKey)!=ERROR_SUCCESS)
 {
  return FALSE;
 }
 CString valuename,databuffer;
 DWORD valuenamebufferlength=200,valuetype,databuddersize=200;
 int i=0;
 while(RegEnumValue(hKey,i++,valuename.GetBuffer(200),&valuenamebufferlength,NULL,&valuetype,(BYTE*)databuffer.GetBuffer(200),&databuddersize) != ERROR_NO_MORE_ITEMS)
 {
  comarray.Add(databuffer);
  databuddersize=200;
  valuenamebufferlength=200;
 }
 RegCloseKey(hKey);
 return TRUE;
}


void ChostDlg::OnBnClickedButtonOpenport() //打开串口按钮消息响应函数
{
	// TODO: 在此添加控件通知处理程序代码
	CString str,str1;
	UINT nPort=m_cbPort.GetCurSel()+1;   //得到串口号
	m_cbBaudrate.GetLBText(m_cbBaudrate.GetCurSel(),str);   //得到波特率
	UINT nBuad;
	nBuad = _ttoi(str);// swscanf(str, _T("%d"), &nBuad);
	UINT iParity = m_cbVerifybit.GetCurSel();
	m_cbDatabit.GetLBText(m_cbDatabit.GetCurSel(),str1);   //得到数据位
	UINT nDatabit;
	nDatabit = _ttoi(str1);// swscanf(str1, _T("%d"), &nDatabit);
	UINT nStopbit = m_cbStopbit.GetCurSel();
	if(m_SerialPort.InitPort(m_hWnd,nPort,nBuad,"NOE"[iParity],nDatabit,nStopbit,EV_RXFLAG | EV_RXCHAR,512))
	{
		SetTimer(0,10000,NULL);
		m_SerialPort.StartMonitoring();  //启动串口通信检测线程函数
		downSend(cmdGetRoomTemperature, 0);// 通知下位机发送当前室温
		downSend(cmdGetAll, 0);  // 取下位机干燥参数
	}
	else
	{
		MessageBox(_T("没有发现此串口或者被占用"));
	}
	m_bPortOpen = m_SerialPort.IsOpen();
	setCommCtrlEnable(!m_SerialPort.IsOpen(),3,14);
}


void ChostDlg::OnBnClickedButtonCloseport()  //关闭串口按钮消息响应函数
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_curLineNo > -1){
		if (MessageBox(L"干燥正在进行！确实要强行退出吗？", L"警告", MB_ICONEXCLAMATION | MB_OKCANCEL) == IDCANCEL){
			return;
		}
	}
	m_SerialPort.ClosePort();
	m_bPortOpen = m_SerialPort.IsOpen();
	setCommCtrlEnable(!m_SerialPort.IsOpen(),3,14);
	setCommCtrlEnable(FALSE, 18, 38);
	KillTimer(1);
}



HBRUSH ChostDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here  
	switch (nCtlColor) //对所有同一类型的控件进行判断  
	{  
	   // process my edit controls by ID.  
	case CTLCOLOR_EDIT:  
	case CTLCOLOR_MSGBOX://假设控件是文本框或者消息框，则进入下一个switch  
	   switch (pWnd->GetDlgCtrlID())//对某一个特定控件进行判断  
	   {      
	   case IDC_EDIT_TEMPERATURE:         // 实际温度文本框  
		pDC->SetBkColor(m_redcolor);    // change the background  
		pDC->SetTextColor(m_textcolor); // change the text color  
		hbr = (HBRUSH) m_redbrush;    // apply the blue brush [this fills the control rectangle]  
		break;    
	   case IDC_EDIT_SETTINGTEMPERATURE:         // 设定温度文本框  
		// but control is still filled with the brush color!  
		pDC->SetBkMode(TRANSPARENT);   // make background transparent [only affects the TEXT itself]  
		pDC->SetTextColor(m_textcolor); // change the text color  
		hbr = (HBRUSH) m_bluebrush;     // apply the red brush [this fills the control rectangle]  
		break;  
	   case IDC_EDIT_AREAPAUSETIME:         // 段暂停时间文本框  
	   case IDC_EDIT_RUNPAUSETIME:         // 全部暂停时间文本框  
		   // but control is still filled with the brush color!  
		   pDC->SetBkMode(TRANSPARENT);   // make background transparent [only affects the TEXT itself]  
		   //pDC->SetTextColor(m_textcolor); // change the text color  
		   hbr = (HBRUSH)m_yellowbrush;     // apply the red brush [this fills the control rectangle]  
		   break;
	   case IDC_EDIT_SENDFARTIME:         // 发送远程数据次数文本框  
	   case IDC_EDIT_RECIVEVALIDTIME:         // 接收无效数据次数文本框  
		   // but control is still filled with the brush color!  
		   pDC->SetBkMode(TRANSPARENT);   // make background transparent [only affects the TEXT itself]  
		   //pDC->SetTextColor(m_textcolor); // change the text color  
		   hbr = (HBRUSH)m_yellowbrush;     // apply the red brush [this fills the control rectangle]  
		   break;
	   case IDC_EDIT_RUNNING:         // 设定温度文本框  
		   if(m_runbrush){
				// but control is still filled with the brush color!  
				pDC->SetBkMode(TRANSPARENT);   // make background transparent [only affects the TEXT itself]  
				//pDC->SetTextColor(m_textcolor); // change the text color  
				hbr = (HBRUSH) *m_runbrush;     // apply the red brush [this fills the control rectangle]  
		   }
		   break;  
	   }  
	   break;  
	}  
	// TODO: Return a different brush if the default is not desired  
	return hbr;  
}


void ChostDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int TempPos{ pScrollBar->GetScrollPos() };
	int tPos = TempPos;
	switch(nSBCode)
	{
	case SB_LEFT://拖动滑块
		TempPos = 0;
		break;
	case SB_RIGHT://拖动滑块
		TempPos = 34560;
		break;
	case SB_THUMBPOSITION://拖动滑块
		TempPos = nPos;
		break;
	case SB_LINELEFT://点击左边的箭头
		if (TempPos > 0)
			{
				TempPos--;
			}
		break;
		case SB_LINERIGHT://点击右边的箭头
			if (TempPos<34560) // 32767
			{
				TempPos++;
			}
		break;
	} 
	if (tPos != TempPos){
		pScrollBar->SetScrollPos(TempPos);
		DrawTemperatureLine();
		SetHStaff();
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

// 绘制水平标尺
void ChostDlg::SetHStaff(void)
{
	CScrollBar* pScrollBar = (CScrollBar*)GetDlgItem(IDC_SCROLLBAR_HFIGURE);
	int nCurpos=pScrollBar->GetScrollPos();
	m_dcMemHG.FillSolidRect(0,0, m_nWidth, 6,RGB(240,240,240));
	m_dcMemTime.FillSolidRect(0,0, m_tWidth, m_tHeight,RGB(240,240,240));
	for(int n=0,i=20-nCurpos;i<m_tWidth-20;i+=60,n++){
		if(i>=20){
			TCHAR s[6];
			m_dcMemHG.MoveTo(i - 20, 0);
			m_dcMemHG.LineTo(i-20,5);
			swprintf(s,6,_T("%d:%02d"),n/6,(n%6)*10);
			CSize size = m_dcMemTime.GetTextExtent(s, wcslen(s));
			m_dcMemTime.TextOut(i - size.cx / 2, 0, s, wcslen(s));
		}
	}
	CDC *hdc = GetDC();
	if (hdc){
		hdc->BitBlt(m_nLeft, m_nTop + m_nHeight, m_nWidth, m_nHeight + 6, &m_dcMemHG, 0, 0, SRCCOPY);
		hdc->BitBlt(m_tLeft, m_tTop, m_tWidth, m_tHeight, &m_dcMemTime, 0, 0, SRCCOPY);
		ReleaseDC(hdc);
	}
}

// 绘制温度曲线
void ChostDlg::DrawTemperatureLine(void)
{
	m_dcMem.FillSolidRect(0, 0, m_nWidth, m_nHeight, RGB(255, 255, 255));
	CDC *hdc = GetDC();
	if (hdc){
		CScrollBar* pScrollBar = (CScrollBar*)GetDlgItem(IDC_SCROLLBAR_HFIGURE);
		ULONGLONG nCurpos = pScrollBar->GetScrollPos();
		ULONGLONG nSizes = (m_recordFile == nullptr)? 0:m_recordFile->recordCount();
		drawGrad(nCurpos);

		if (nSizes){
			short int record[4];
			ULONGLONG lPos;
			BOOL drawing = true;
			showSettingTemperatureAsForecast(nCurpos);
			TRY{
				HPEN hPen1 = CreatePen(PS_SOLID, 1, m_bluecolor);
				HPEN hPen2 = CreatePen(PS_SOLID, 1, m_redcolor);
				lPos = findLinePoint(nCurpos, m_record);
				toLP(m_record);
				while (drawing){
					if (m_recordFile->readRecord(record, 1)){
						toLP(record);
						if (m_record[2] > m_nWidth + nCurpos){
							midDryData(m_record, record, record, m_nWidth + nCurpos);
							drawing = false;
						}
						m_dcMem.SelectObject(hPen1);
						m_dcMem.MoveTo(m_record[2] - nCurpos, m_record[0]);
						m_dcMem.LineTo(record[2] - nCurpos, record[0]);
						m_dcMem.SelectObject(hPen2);
						m_dcMem.MoveTo(m_record[2] - nCurpos, m_record[1]);
						m_dcMem.LineTo(record[2] - nCurpos, record[1]);
						memcpy(m_record, record, m_recordFile->getRecordSize());
					}
					else
						drawing = false;
				}
				DeleteObject(hPen1);
				DeleteObject(hPen2);
			}
				CATCH(CFileException, e){
#ifdef _DEBUG
				afxDump << "文件不能打开！" << e->m_cause << "\n";
#endif
			}END_CATCH
		}
		hdc->BitBlt(m_nLeft, m_nTop, m_nWidth, m_nHeight, &m_dcMem, 0, 0, SRCCOPY);
		ReleaseDC(hdc);
		forecast(-1);
	}
}


void ChostDlg::OnBnClickedButtonOption()
{
	// TODO: 在此添加控件通知处理程序代码
	m_xml->dialog(m_curLineNo == -1);
}
BOOL ChostDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete httpClinet;
	delete m_xml;

	// 关闭记录文件
	if (m_recordFile != nullptr){
		delete m_recordFile;
		m_recordFile = nullptr;
	}

	m_dcMem.DeleteDC();
	m_dcMemTime.DeleteDC();
	m_dcMemHG.DeleteDC();

	return CDialogEx::DestroyWindow();
}

// 结束干燥
void ChostDlg::endDry(void)
{
	CString data, url,s;
	url.Format(L"%sindex.php? cmd=%s/%d",m_xml->getUrl(),L"DryMain",dryMainId);
	data.Format(L"{\"item\":{\"drymain\":{\"state\":%d}}}",-1);
	int result = httpClinet->HttpPut(url, data, s);
	m_edtSendFarTimes++;

	KillTimer(1);
	downSend(cmdSetLineNo, 0);// 设置下位机当前段号为 0
	m_curLineNo = -1;
	downSend(cmdSetSettingTemperature, -99);// 设置下位机当前段号为 0
	downSend(cmdSetLineNo, 0xff00);// 设置下位机当前运行状态为 4(结束)
	setCommCtrlEnable(FALSE, 20, 45);
	SetDlgItemText(IDC_BUTTON_START, L"开始干燥");
}

/**************************************************************************************
* 功  能：向下位机发送指令或传送数据
* 参  数：cmd 字符类型，指定命令
*         degress 无符号整数，指定指令或数据
* 返回值：void
**************************************************************************************/
void ChostDlg::downSend(char cmd, WORD degress)
{
	char send[]{0xfd,0x0,0x0,0x0,0xfe};
	send[1] = cmd;
	*(WORD*)(send+2) = degress;
	m_SerialPort.WriteToPort(send,5);
	m_edtSendTimes++;
	UpdateData(false);
	Sleep(100);
}


void ChostDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_xml->getStartDryMode()){
		dryBegin();
	}
	else{
		SetTimer(2, 1000, NULL);
	}
}

int ChostDlg::setLineState(void)
{
	int state;
	vector< vector<int> >&m_dryLines = m_xml->getdryLines();
	if (m_dryLines[m_curLineNo][1]>0) state = 0;
	else if (m_dryLines[m_curLineNo][1] == 0) state = 1;
	else if (m_dryLines[m_curLineNo][1]<0) state = 2;

	m_edtArea.Format(_T("%d℃ %s"), m_dryLines[m_curLineNo][0], dryRunningStatus[state]);
	return state;
}
/**************************************************************************************
* 功  能：从给定的线段号和时间处开始干燥
* 参  数：lineNo 整数，指定线段号
*         lineTime 整数，指定时间(距lineNo起点的时间),单位：分钟
* 返回值：void
**************************************************************************************/
void ChostDlg::goLine(int lineNo, int lineTime)
{
	m_curLineNo = lineNo;
	vector< vector<int> >&m_dryLines = m_xml->getdryLines();
	if (m_curLineNo<m_dryLines.size()){ // 干燥未结束
		setLineTime(lineTime);
		setLinePauseTime(0);
		m_lbSettingtemperature = getSettingTemperature(lineNo, lineTime);

		m_edSetHeatingRate.Format(L"%3.1f", static_cast<float>(m_dryLines[lineNo][1]));

		int state = setLineState();

		downSend(cmdSetLineNo, ((state + 1) << 8) | (m_curLineNo + 1));// 设置下位机当前段号
		downSend(cmdSetSettingTemperature, (WORD)(m_lbSettingtemperature * 16));// 传送当前设定温度给下位机
		downSend(cmdSetLineTime, (WORD)(lineTime));// 传送当前设定温度给下位机
	}
	else{
		endDry();
	}
}

/**************************************************************************************
* 功  能：进入下一段干燥线
* 参  数：void
* 返回值：void
**************************************************************************************/
void ChostDlg::goNextLine(void)
{
	goLine(++m_curLineNo, 0);
}

void ChostDlg::initSettingTemperature(WORD temperature)
{
	if (m_lbSettingtemperature == -100){
		m_lbSettingtemperature = temperature*0.0625;
		m_record[0] = temperature;
		m_record[1] = temperature;
		m_record[2] = 0;
		m_record[0] = 0;
	}
	//for (int i = 0; i < m_fnTemperature.size();i++)
		//if (memcmp((const void*)m_fnTemperature[i], (const void*)m_fnInitSettingTemperature, sizeof(FNTemperature))
			//m_fnTemperature.erase(i);
}

void ChostDlg::showTemperature(WORD temperature)
{
	CString str;
	str.Format(_T("%5.1f"), temperature*0.0625);
	m_lbTemperature = _ttof(str);
}
/**************************************************************************************
 * 功  能：判断当前温度与设定温度的差值，根据不同的阀值要求做相应处理
 *         (用不同的颜色显示不同的温度差范围及是否触发声音报警)
 * 参  数：temperature 双精度浮点数，指定实际温度
 * 返回值：void 
 **************************************************************************************/
void ChostDlg::adjuster(WORD temperature)
{
	double temper = temperature*0.0625;
	SubjectArray *m_xmlOption = m_xml->getOptions();
	SubjectArray *m_xmlValue = m_xml->getValues();
	m_Pause = FALSE;

	if (m_xmlOption->get(6)){ // 处理恒温故障
		if (m_sameTemperature == temperature){
			if (++m_sameTemperatureTimes > m_xmlValue->get(6)*10){ // 有可能下位机系统出现故障
				Beep(2500, 300);
				return;
			}
		}
		else{
			m_sameTemperature = temperature;
			m_sameTemperatureTimes = 0;
		}
	}

	if (temper < m_lbSettingtemperature - m_xmlValue->get(2)-1){
		// 温度低
		if(m_xmlOption->get(2))
			Beep (1000,1000); 
		m_edtRunning = "温度低";
		m_runbrush = &m_yellowbrush;
		if (temper < m_lbSettingtemperature - m_xmlValue->get(3)-1){
			m_edtRunning = "温度低暂停";
			m_runbrush = &m_redbrush;
			if(m_xmlOption->get(3))
				Beep (2000,500);
			if(m_xmlOption->get(0)) // 是否允许低温暂停
				m_Pause = TRUE;
		}
	}
	else if (temper > m_lbSettingtemperature + m_xmlValue->get(2)+1){
		// 温度高
		m_edtRunning = "温度高";
		if(m_xmlOption->get(2))
			Beep (1000,500);
		m_runbrush = &m_yellowbrush;
		if (temper > m_lbSettingtemperature + m_xmlValue->get(3)+1){
			if(m_xmlOption->get(3))
				Beep (2000,1000);
			m_edtRunning = "温度高!!!";
			m_runbrush = &m_redbrush;
		}
	}else{ // 温度正常
		m_edtRunning = "正常";
		m_runbrush = &m_greenbrush;
	}
}

void ChostDlg::showSettingTemperatureAsForecast(int nCurpos)
{
	if (m_TotalTimes < nCurpos + m_nWidth){
		vector< vector<int> >&dryLines = m_xml->getdryLines();
		float speed;
		int time, time1, time2;
		short int rd[4];
		int lineNo = m_curLineNo;
		float settingTemperature = m_lbSettingtemperature;
		float t0;
		if (m_TotalTimes < nCurpos){
			time1 = m_TotalTimes - m_curLineTime;
			do{
				t0 = getSettingTemperature(lineNo, 0, -273);
				speed = static_cast<float>(dryLines[lineNo][1]);
				time = (speed ? (dryLines[lineNo][0] - t0) / speed : dryLines[lineNo][2]) * 360;
				time1 += time;
				lineNo++;
			} while (time1 < nCurpos && lineNo < dryLines.size());
			if (time1 < nCurpos && lineNo >= dryLines.size())
				return;
			time2 = nCurpos - time1 + time;
			rd[0] = getSettingTemperature(--lineNo, (nCurpos-time1+time)/6, -273)*16;
			rd[2] = nCurpos;
		}
		else{
			time2 = m_curLineTime;
			rd[0] = m_lbSettingtemperature*16;
			rd[2] = m_TotalTimes;
		}
		toLP(rd);
		HPEN hPen = CreatePen(PS_DASH, 1, RGB(200, 200, 255));
		HPEN oldPen = (HPEN)m_dcMem.SelectObject(hPen);
		m_dcMem.MoveTo(rd[2] - nCurpos, rd[0]);
		time1 = rd[2]-time2;
		do{
			t0 = getSettingTemperature(lineNo, 0, -273);
			speed = static_cast<float>(dryLines[lineNo][1]);
			time = (speed ? (dryLines[lineNo][0] - t0) / speed : dryLines[lineNo][2]) * 360;
			time1 += time;
			if (time1 < nCurpos + m_nWidth){
				rd[0] = dryLines[lineNo][0] * 16;
				rd[2] = time1;
				toLP(rd);
				m_dcMem.LineTo(rd[2] - nCurpos, rd[0]);
				lineNo++;
			}
			else
				break;
		} while (lineNo < dryLines.size());
		if (lineNo < dryLines.size()){
			rd[0] = getSettingTemperature(lineNo, (time - time1 + nCurpos + m_nWidth)/6, -273)*16;
			rd[2] = nCurpos + m_nWidth;
			toLP(rd);
			m_dcMem.LineTo(rd[2] - nCurpos, rd[0]);
		}
		m_dcMem.SelectObject(oldPen);
		DeleteObject(hPen);
	}
}

void ChostDlg::forecast(int nCurpos)
{
	static int scrollpos = -1;
	double v = 0;
	if (nCurpos == -1){
		scrollpos = -1;
		return;
	}
	int count = m_recordFile->recordCount();
	forecastModel *m_pForecastMode = m_xml->getForecastModel();
	if (count > FORECAST_DATA_LENGTH-1 && m_pForecastMode){
		short int record[FORECAST_DATA_LENGTH][4];
		HPEN hPen = CreatePen(PS_DASH, 1, RGB(255,200,200));
		HPEN oldPen = (HPEN)m_dcMem.SelectObject(hPen);
		int oldRop = m_dcMem.SetROP2(R2_NOTXORPEN);
		if (scrollpos != -1){
			m_pForecastMode->setCanvas(&m_dcMem, nCurpos);
			m_pForecastMode->showLine();
			//drawSettingTemperatureLine(nCurpos);
		}
		m_recordFile->seekRecord(-FORECAST_DATA_LENGTH, CFile::end);
		m_recordFile->readRecord(record[0], FORECAST_DATA_LENGTH);
		v = m_pForecastMode->_forecast(record);
		//showSettingTemperatureAsForecast(nCurpos);
		m_dcMem.SetROP2(oldRop);
		m_dcMem.SelectObject(oldPen);
		DeleteObject(hPen);

		scrollpos = nCurpos;
	}
	else if(count > 5){
		short int record[6][4];
		m_recordFile->seekRecord(-6, CFile::end);
		m_recordFile->readRecord(record[0], 6);
		v = (record[5][1] - record[0][1]) / (record[5][2] - record[0][2]);
	}
	m_edRealHeatingRate.Format(L"%3.1f", v*0.0625*360);
}

/**************************************************************
 * 处理当前温度及相关参数(保存到本地、传送到远端并显示到屏幕)
 * 参数：temperature 无符号整数，指定实际温度
 * 返回值：void
 **************************************************************/
void ChostDlg::savePoint(WORD temperature)
{
	CString s;
	short int record[]{ m_lbSettingtemperature * 16, temperature, m_TotalTimes, m_curLineNo };

	// 保存到本地
	m_recordFile->appendRecord(record);

	saveDryDataToWeb(record);

	// 显示到屏幕
	toLP(record);
	CScrollBar* pScrollBar = (CScrollBar*)GetDlgItem(IDC_SCROLLBAR_HFIGURE);
	if (pScrollBar){
		int nCurpos = pScrollBar->GetScrollPos();
		if (m_TotalTimes > nCurpos + 1){
			HPEN hPen1 = CreatePen(PS_SOLID, 1, m_bluecolor);
			HPEN hPen2 = CreatePen(PS_SOLID, 1, m_redcolor);
			HPEN hOldPen = (HPEN)m_dcMem.SelectObject(hPen1);
			m_dcMem.MoveTo(m_record[2] - nCurpos, m_record[0]);
			m_dcMem.LineTo(record[2] - nCurpos, record[0]);
			m_dcMem.SelectObject(hPen2);
			m_dcMem.MoveTo(m_record[2] - nCurpos, m_record[1]);
			m_dcMem.LineTo(record[2] - nCurpos, record[1]);
			forecast(nCurpos);
			m_dcMem.SelectObject(hOldPen);
			DeleteObject(hPen1);
			DeleteObject(hPen2);
		}
		CDC *hdc = GetDC();
		if (hdc)
			hdc->BitBlt(m_nLeft, m_nTop, m_nWidth, m_nHeight, &m_dcMem, 0, 0, SRCCOPY);
		ReleaseDC(hdc);
	}
	memcpy(m_record, record, m_recordFile->getRecordSize());
	m_restoreTemperaturePoint.clear();
}

// 把 time 从 HH:MM 格式的字符串转换成以秒为单位的无符号整数
UINT ChostDlg::timeToSecond(CString time)
{
	int pos = time.Find(':');
	int hour;
	int minute;
	hour = _ttoi(time.Left(pos));// swscanf(time.Left(pos), _T("%d"), &hour);
	minute = _ttoi(time.Mid(pos + 1));// swscanf(time.Mid(pos + 1), _T("%d"), &minute);
	return (hour * 60 + minute) * 60;
}


void ChostDlg::OnNMThemeChangedScrollbarHfigure(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 该功能要求使用 Windows XP 或更高版本。
	// 符号 _WIN32_WINNT 必须 >= 0x0501。
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}


void ChostDlg::setCommCtrlEnable(bool enabled,int minIndex,int maxIndex)
{
	CWnd   *pWnd;
	int index = 1;
	pWnd = GetWindow(GW_CHILD);
	while (pWnd != NULL)
	{
		if (index >= minIndex && index <= maxIndex){
			pWnd->EnableWindow(enabled);
		}
		index++;
		pWnd = pWnd->GetNextWindow();
	}
	GetDlgItem(IDC_BUTTON_CLOSEPORT)->EnableWindow(enabled);
}


short int* ChostDlg::toLP(short int * record)
{
	CPoint pt(record[2], record[0]*0.0625 + 50);
	CPoint pt1(record[2], record[1] * 0.0625 + 50);
	m_dcMem.DPtoLP(&pt);
	m_dcMem.DPtoLP(&pt1);

	record[0] = pt.y;
	record[1] = pt1.y;
	record[2] = pt.x;
	return record;
}


void ChostDlg::dryRuning(void)
{
	//m_fnInitSettingTemperature = bind1st(mem_fun(&ChostDlg::initSettingTemperature), this);
	m_fnInitSettingTemperature = bind(&ChostDlg::initSettingTemperature, this, std::placeholders::_1);
	m_fnTemperature.push_back(m_fnInitSettingTemperature);
	SetTimer(1, 60000, NULL);
	setCommCtrlEnable(TRUE, 18, 45);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	SetDlgItemText(IDC_BUTTON_START, L"正在干燥...");
	showSettingTemperatureAsForecast(0);
}

void ChostDlg::dryBegin(void)
{
	m_TotalTimes = 0;
	m_TotalPauseTime = 0;
	m_dataInvalid = 0;
	downSend(cmdSetLineNo, 0x0);// 设置下位机当前段号为 0，置下位机为初始状态
	m_curLineNo = 0;

	if (processInterruptFile(m_curLineNo, m_curLineTime)){
		dryRuning();
	}
	else{
		throw L"不能打开文件 " + m_filename;
	}
}


void ChostDlg::OnEnSetfocusEditTemperature()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_OPTION)->SetFocus();
}


// 并把开始日期、时间和干燥曲线编号发送到远程数据库
int ChostDlg::saveToWeb(CString url,CString data)
{
	CString s;
	int result = httpClinet->HttpPost(url, data, s);
	int id = strToJsonId(s);
	if (id >= 0)
		m_edtSendFarTimes++;
	//else if (!ignoreWeb)
	//	switch (MessageBox(L"远程数据处理错误！请选择程序运行方式", L"网络错误", MB_ICONQUESTION | MB_ABORTRETRYIGNORE)){
	//	case IDABORT:
	//		exit(0);
	//	case IDRETRY:
	//		id = saveToWeb(url,data);
	//		break;
	//	case IDIGNORE:
	//		ignoreWeb = true;
	//		break;
	//}
	return id;
}

/**************************************************************
* 从远程数据库drymain表中取正在干燥记录的 id
* 参数：void
* 返回值：整数，指定正在干燥记录的 id
**************************************************************/
int ChostDlg::getMainIdFromWeb(void)
{
	CString cmd = L"DryMain";
	CString s;
	CString data, url;
	data = L"cond%5B0%5D%5Bfield%5D=state&cond%5B0%5D%5Bvalue%5D=0&cond%5B0%5D%5Boperator%5D=eq&filter%5B0%5D=id";
	url.Format(L"%sindex.php? cmd=%s&%s", m_xml->getUrl(), cmd, data);
	//data = L"{\"cond\":[{\"field\":\"state\",\"value\":0,\"operator\":\"eq\"}],\"filter\":[\"id\"]}";
	int result = httpClinet->HttpGet(url, data, s);
	return strToJsonId(s);
}

/**************************************************************
* 发送干燥数据（开始日期、时间和干燥曲线编号）到远程数据库
* 参数：record 指向 WORD 数组的指针，指定干燥数据
* 返回值：void
**************************************************************/
void ChostDlg::saveDryMainToWeb(CTime tm)
{
	CString cmd = L"DryMain";
	CString starttime = tm.Format(L"%Y-%m-%d %H:%M:%S");
	CString _line_no;
	CString data, url;

	_line_no.Format(L"%d", 0); // 干燥曲线号
	url.Format(L"%sindex.php? cmd=%s", m_xml->getUrl(), cmd);
	//data.Format(L"item%%5Bdrymain%%5D%%5Bstarttime%%5D=%s&item%%5Bdrymain%%5D%%5Blineno%%5D=%s&item%%5Bdrymain%%5D%%5Bstate%%5D=%d", starttime, _line_no, 0);
	data.Format(L"{\"item\":{\"drymain\":{\"starttime\":\"%s\",\"lineno\":%s,\"state\":%d}}}", starttime, _line_no, 0);
	dryMainId = saveToWeb(url, data);
}

/**************************************************************
* 发送干燥数据到远程数据库
* 参数：record 指向 WORD 数组的指针，指定干燥数据
* 返回值：void
**************************************************************/
void ChostDlg::saveDryDataToWeb(short int* record)
{
	if (dryMainId > -1){
		CString cmd = L"DryData";
		CString data, url;

		// 传送到远程
		url.Format(L"%sindex.php? cmd=%s", m_xml->getUrl(), cmd);
		//data.Format(L"item%%5Bdrydata%%5D%%5Bmainid%%5D=%d&item%%5Bdrydata%%5D%%5Btime%%5D=%d&item%%5Bdrydata%%5D%%5Bsettingtemperature%%5D=%d&item%%5Bdrydata%%5D%%5Btemperature%%5D=%d&item%%5Bdrydata%%5D%%5Bmode%%5D=%d",dryMainId, record[2], record[0], record[1], record[3]);
		//data.Format(R"({"item":{"drydata":{"mainid":%d,"time":%d,"settingtemperature":%d,"temperature":%d,"mode":%d}}})", dryMainId, record[2], record[0], record[1], record[3]);
		data.Format(L"{\"item\":{\"drydata\":{\"mainid\":%d,\"time\":%d,\"settingtemperature\":%d,\"temperature\":%d,\"mode\":%d}}}", dryMainId, record[2], record[0], record[1], record[3]);
		saveToWeb(url, data);
	}
}

/**************************************************************
* 取远程服务器返回数据中的 id 值
* 参数：s CString，指定远程服务器返回数据
* 返回值：long, 提取的 id 值
**************************************************************/
long ChostDlg::strToJsonId(CString s)
{
	CString strId = L"\"id\":";
	int extLen = 0;
	int iId = s.Find(strId);
	if (iId != -1){
		if (s.Find(L"\"", iId + strId.GetLength()) == iId + strId.GetLength())
			extLen = 1;
		int iSpace = s.Find(L",", iId);
		if (iSpace == -1)
			iSpace = s.Find(L"}", iId);
		CString cId = s.Mid(iId + strId.GetLength() + extLen, iSpace - iId - strId.GetLength() - 2 * extLen);
		return  _ttoi(cId);
	}
	return iId;
}

/**************************************************************
* 设置并显示当前线段时间
* 参数：time 整数，指定当前线段运行时间，单位：分钟
* 返回值：void
**************************************************************/
void ChostDlg::setLineTime(int time)
{
	div_t h_m = div(time, 60);
	m_curLineTime = time;
	m_edtAreaTime.Format(timeFormat, h_m.quot, h_m.rem);
}

/**************************************************************
* 设置并显示当前线段暂停时间
* 参数：time 整数，指定当前线段暂停时间，单位：分钟
* 返回值：void
**************************************************************/
void ChostDlg::setLinePauseTime(int time)
{
	div_t h_m = div(time, 60);
	m_curLinePauseTime = time;
	m_edtAreaPauseTime.Format(timeFormat, h_m.quot, h_m.rem);
}

/**************************************************************
* 设置并显示干燥运行时间
* 参数：time 整数，指定干燥运行时间，单位：10秒
* 返回值：void
**************************************************************/
void ChostDlg::setRunTime(int time)
{
	div_t h_m = div(time/6, 60);
	m_TotalTimes = time;
	m_edtRunTime.Format(timeFormat, h_m.quot, h_m.rem);
}

/**************************************************************
* 设置并显示干燥总暂停时间
* 参数：time 整数，指定干燥总暂停时间，单位：分钟
* 返回值：void
**************************************************************/
void ChostDlg::setRunPauseTime(int time)
{
	div_t h_m = div(time, 60);
	m_TotalPauseTime = time;
	m_edtRunPauseTime.Format(timeFormat, h_m.quot, h_m.rem);
}


/**************************************************************
* 取给定段号、段时间和开始温度下的理论设定温度
* 参数：lineNo - 整数，指定干燥段号
*       lineTime - 整数，指定段时间，单位：分钟
*       roomTemprature - 浮点数，指定干燥开始时的室温，单位：度
* 返回值：double - 双精度浮点数，指定设定温度
**************************************************************/
double ChostDlg::getSettingTemperature(int lineNo, int lineTime, float roomTemperature)
{
	double tem;
	vector< vector<int> >&m_dryLines = m_xml->getdryLines();
	if (lineNo % 2){
		tem = m_dryLines[lineNo][0];
	}
	else{
		float room;
		if (lineNo == 0)
			if (roomTemperature == -273){
				room = (m_temperatureRoom < m_lbTemperature) ? m_temperatureRoom : m_lbTemperature;
			}else
				room = roomTemperature;
		else
			room = m_dryLines[lineNo - 1][0];
		tem = room+m_dryLines[lineNo][1]*lineTime/60;
	}
	return tem;
}

// 中断程序处理记录文件
int ChostDlg::processInterruptFile(int lineNo,int lineTime)
{
	CTime tm = CTime::GetCurrentTime();
	CString filehead = tm.Format(L"dry%Y%m%d%H%M%S");
	CString t_filename = NULL;
	// 如记录文件已经存在，则保存已记录的内容到备份文件(每次中断保存在不同的文件中)
	if (m_recordFile && m_recordFile->getOpenStatus()){
		t_filename = m_recordFile->backup();
		m_filename = m_recordFile->GetFileName();
		delete m_recordFile;
		m_recordFile = nullptr;
		if (t_filename!=m_filename) CFile::Rename(m_filename, t_filename);

	}

	// 记录文件不存在，则根据当前日期创建文件名，并把开始日期、时间和干燥曲线编号发送到远程数据库
	else{
		saveDryMainToWeb(tm);
		m_filename = tm.Format(L"dry%Y%m%d.dat");
	}

	// 打开新记录文件
	const UINT openflags{ CFile::typeBinary | CFile::modeCreate | CFile::modeReadWrite };
	m_recordFile = new recordFile(m_filename, openflags);
	if (m_recordFile->getOpenStatus()){ // 如新文件打开成功
		m_recordFile->writeHeader(filehead); // 写文件头
		if (!t_filename.IsEmpty()){ // 如旧记录文件有内容，则把其导入新记录文件
			recordFile t_recordFile(t_filename, openflags | CFile::modeNoTruncate);
			if (t_recordFile.getOpenStatus()){
				double roomTemperature = m_xml->get430Temperature();
				short int record[4];// = { m_lbTemperature, m_lbTemperature, 0, 0 };
				unsigned int preTime = 0;
				bool lineChange = FALSE;

				//m_TotalTimes = 0;
				//m_lbSettingtemperature = m_lbTemperature;

				t_recordFile.seekRecord(0, CFile::begin);
				for (int i = 0; i < t_recordFile.recordCount(); i++){
					t_recordFile.readRecord(record, 1);
					if (i == 0)
						roomTemperature = record[0]*0.0625;
					if (record[3] <= lineNo){
						if (record[3] == lineNo && !lineChange){
							preTime = record[2]; // 段开始时间
							lineChange = TRUE;
						}
						if (lineChange && (record[2] - preTime) >= lineTime){
							break;
						}
						m_recordFile->appendRecord(record);
						//t_file.Read(record, size);
					}
				}

				if (record[3] == lineNo){ // 记录段号等于重新开始段号
					//m_lbSettingtemperature = record[0] * 0.0625; // 置设定温度为记录的设定温度
					setRunTime(preTime + lineTime - 1);
					m_lbTemperature = roomTemperature;// 置设定温度为理论计算值
					goLine(lineNo, lineTime/6);
				}
				else if (record[3] < lineNo){ // 记录段号小于重新开始段号
					int preSetTemperature = record[0] * 0.0625;
					int preLineTime = record[2]-preTime;
					vector< vector<int> >m_dryLines = m_xml->getdryLines();

					// 计算线段交点记录并写入文件
					for (int i = record[3]; i < lineNo; i++){
						if (i % 2){
							record[2] += m_dryLines[i][2] * 60 * 6-preLineTime;
						}
						else{
							record[2] += (m_dryLines[i][0] - preSetTemperature) / m_dryLines[i][1] * 60 * 6;
						}
						preLineTime = 0; // 第一次计算记录中断后置 0
						preSetTemperature = m_dryLines[i][0];
						record[0] = preSetTemperature * 16;
						record[1] = record[0];
						record[3] = i+1;
						m_recordFile->appendRecord(record);
						saveDryDataToWeb(record);
					}

					// 计算断点运行开始点记录并写入文件
					record[2] += lineTime;
					record[0] = getSettingTemperature(lineNo, lineTime/6)*16;
					record[1] = record[0];
					record[3] = lineNo;
					m_recordFile->appendRecord(record);
					saveDryDataToWeb(record);

					//m_lbSettingtemperature = record[0] * 0.0625;
					//m_TotalTimes = record[2];
					setRunTime(record[2]);
					goLine(lineNo, lineTime / 6);
				}
			}
		}
		else{
			setRunTime(0);
			goLine(lineNo, lineTime / 6);
		}
		UpdateData(FALSE);
		return 1;
	}
	return 0;
}

void ChostDlg::printReport(void)
{
	CPrintDialog print(false);
	if (print.DoModal() == IDOK){
		int page = 1;
		CDC printed;
		printed.Attach(print.GetPrinterDC());
		DOCINFO pdoc;
		pdoc.cbSize = sizeof(pdoc);
		pdoc.lpszDocName = L"pdoc";
		pdoc.lpszDatatype = NULL;
		pdoc.fwType = NULL;
		pdoc.lpszOutput = NULL;
		if (printed.StartDoc(&pdoc) >= 0){
			LOGFONT logfont;
			memset(&logfont, 0, sizeof(LOGFONT));
			logfont.lfHeight = 75;
			CFont font;
			CFont *oldfont = NULL;
			int nLeft, nTop;
			if (font.CreateFontIndirect(&logfont))
				oldfont = (CFont*)printed.SelectObject(&font);
			for (int j = 1; j <= page; j++)     {
				CRect rect;
				rect.left = 200;
				rect.top = 1800;
				rect.right = 4000 - 200;
				rect.bottom = 2000 + 200;
				dryLine *dryline = new dryLine((CPaintDC*)&printed, rect);
				printed.StartPage();
				int x = 500, y = 400;//A4纸，页面中的位置,横向为x轴，纵向是y轴,A4 maxX=4000 maxY=7000 建议按字符大小为75，每页安排40条纪录，初试纪录开始位置为x=500 y=200 
				CString pageHead{ L"干燥报告" }, pageBottom;
				printed.TextOut(1500, 100, pageHead); //打印页眉
				CString title{ L"开始时间                结束时间                      干燥曲线" };
				printed.TextOut(500, 200, title); //打印页眉
				CString stt{ L"______________________________________________________________________________________" };
				printed.TextOut(500, 200 + 80, stt); //打印页眉     
				dryline->draw(&printed, *m_recordFile, 0);
				pageBottom.Format(_T("共%d页   第%d页"), page, j);
				printed.TextOut(1500, y, pageBottom);
				printed.EndPage();//此页结束
			}
			font.DeleteObject();
			if (oldfont != NULL)
				printed.SelectObject(oldfont);
			printed.EndDoc();
		}
		printed.DeleteDC();
	}
}
