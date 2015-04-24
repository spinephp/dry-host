

// hostDlg.cpp : 实现文件
//

// 在程序的文件夹下生成MSXML3.TLH和MSXML3.TLI两个文件;  

#include "stdafx.h"
#include "host.h"
#include "hostDlg.h"
#include "optionDlg.h"
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
	, m_edTemperature430(_T(""))
	, m_edTemperatureRoom(_T(""))
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
	DDX_Text(pDX, IDC_EDIT_TEMPERATURE430, m_edTemperature430);
	DDX_Text(pDX, IDC_EDIT_TEMPERATUREROOM, m_edTemperatureRoom);
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

	m_startDryMode = 0;// 设置开始模式为 0-随时

	v_index = 0;
	v_lastTemperature = -100;
	m_smoothvalue = m_allowOperatingValue[1]*16; // 设置 10 秒内温度变化阀值在 1℃ 之内

	m_redcolor=RGB(255,0,0);                      // 红色  
	m_bluecolor=RGB(0,0,255);                     // 蓝色  
	m_greencolor=RGB(0,255,0);                     // 绿色  
	m_textcolor=RGB(255,255,255);                 // 文本颜色设置为白色  
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

	CScrollBar* pScrollBar = (CScrollBar*)GetDlgItem(IDC_SCROLLBAR_HFIGURE);
	pScrollBar->SetScrollRange(0,34560);//滑块移动的位置为0——34560；
	DrawTemperatureLine();
	SetHStaff();

	CoInitialize(NULL);
	loadXLM();
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
		for(int n=-50,i=m_nTop+m_nHeight;i>30;i-=10,n+=10){
			TCHAR s[5];
			dc.MoveTo(m_nLeft-5,i);
			dc.LineTo(m_nLeft,i);
			if(n%50==0){
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR ChostDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//字符接收消息响应函数
LONG ChostDlg::OnComm(WPARAM ch,LPARAM port)
{
	v_portin[v_index++] = ch;
	if(v_portin[v_index-1]==0xfe && (v_index==5 || v_index==13)){ //接收完毕
		CString str; 
		char cmd = v_portin[1];
		short int temperature;
		float temper;
		v_index = 0;
		switch(cmd){
			case cmdGetTemperature: // 当前实际温度
				temperature = *(short int*)(v_portin+2);
				str.Format(_T("%5.1f"), temperature*0.0625);
				m_lbTemperature = _ttof(str);// swscanf(str, _T("%5.1f"), &m_lbTemperature);
				if (v_lastTemperature == -100)
					v_lastTemperature = temperature;
				if(m_allowOperating[1] && temperature > v_lastTemperature-m_smoothvalue && temperature < v_lastTemperature+m_smoothvalue || !m_allowOperating[1] && temperature > -800 && temperature < 3200){ // ，为正常，否则认为是由其它因素造成的误差
					v_lastTemperature = temperature;
					//m_lbTemperature = strtod(str, NULL);// = v_portin.one*0.0625; //将接收到的字符存入编辑框对应的变量中
					if(m_curLineNo>-1){// 干燥已开始
						if(((int)m_lbSettingtemperature)==-100){
							m_lbSettingtemperature = m_lbTemperature;
							m_edtArea.Format(_T("%d℃ %s"),m_dryLines[m_curLineNo][0],dryRunningStatus[0]);
						}
						adjuster(temperature*0.0625);
						savePoint(temperature);
					}else{
						m_edtRunning = L"未开始";
						m_runbrush = NULL;
					}
				}else{
					m_dataInvalid++;
				}
				UpdateData(FALSE);  //将接收到的字符显示在接受编辑框中
				break;
			case cmdGetRoomTemperature: // MSP430片内温度
				temperature = *(short int*)(v_portin + 2);
				temper = temperature / 1.45266 - 278.75;
				str.Format(_T("%5.1f"), temper);
				m_edTemperature430 = str;
				m_edTemperatureRoom.Format(L"%5.1f",temper - 9.7);
				UpdateData(FALSE);
				break;
			case cmdGetSettingTemperature: // 下位机当前的设定温度
				temperature = *(short int*)(v_portin + 2);
				temper = temperature*0.0625;
				for(UINT i=0;i<m_fnSettingTemperature.size();i++){
					FNsettingTemperature fun = m_fnSettingTemperature[i];
					(fun)(temper);
				}
				break;
			case cmdGetLineNo: // 下位机当前段号
				break;
			case cmdGetAll: // 下位机全部信息

				// 显示实际温度
				m_lbTemperature = (*(WORD*)(v_portin + 10))*0.0625;
				UpdateData(FALSE);

				switch (v_portin[2]){
				case 0xff: // 干燥未进行
					// 使能“开始干燥”按钮
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
					break;
				default:
					div_t h_m;
					interruptDlg dlg;
					for(int i=0;i<m_dryLines.size();i++){
						CString str;
						str.Format(_T("%d℃ %s"),m_dryLines[i][0],m_dryLines[i][1]>0? _T("升温"):(m_dryLines[i][1]==0? _T("保温"):_T("降温")));
						dlg.m_lineNames.push_back(str);
					}
					dlg.m_dryLines.assign(m_dryLines.begin(), m_dryLines.end());

					m_curLineNo = *(WORD*)(v_portin+2)-1;
					if (m_curLineNo >= 0){// 干燥已经开始
						// 把干燥参数传递给断点对话框
						dlg.m_curSelLine = m_curLineNo;
						dlg.m_edLineName.Format(_T("%d℃ %s"), m_dryLines[m_curLineNo][0], dryRunningStatus[0]);
						m_lbSettingtemperature = (*(WORD*)(v_portin + 4))*0.0625;
						dlg.m_edSetingTemperature = m_lbSettingtemperature;
						h_m = div(*(WORD*)(v_portin + 6) / 6, 60);
						m_edtRunTime.Format(_T("%2d:%02d"), h_m.quot, h_m.rem);
						dlg.m_edAllTime = m_edtRunTime;
						h_m = div(*(WORD*)(v_portin + 8), 60);
						m_edtAreaTime.Format(_T("%2d:%02d"), h_m.quot, h_m.rem);
						dlg.m_edLineTime = m_edtAreaTime;
						dlg.m_edSetingTemperature = m_lbSettingtemperature;
						dlg.m_edTemperature = m_lbTemperature;
						INT_PTR nResponse = dlg.DoModal();
						if (nResponse == IDOK)
						{
							// TODO: 在此放置处理何时用
							//  “确定”来关闭对话框的代码
							// 根据断点对话框参数，设置如何继续干燥
							m_curLinePauseTime = 0;
							m_TotalTimes = timeToSecond(dlg.m_edAllTime);
							m_TotalPauseTime = 0;
							m_curLineNo = dlg.m_curSelLine;
							switch (dlg.m_rdAutoRun){
							case 0:
								m_curLineTime = 0;
								m_Pause = false;
								while (m_lbSettingtemperature < m_lbTemperature){
									OnTimer(1);
								}
								SetTimer(1, 60000, NULL);
								break;
							case 1: // in temperature
								m_curLineTime;
								m_Pause = false;
								while (m_lbSettingtemperature < m_lbTemperature){
									OnTimer(1);
								}
								SetTimer(1, 60000, NULL);
								break;
							case 2: // in time
								m_curLineTime = timeToSecond(dlg.m_edLineTime);
								SetTimer(1, 60000, NULL);
								break;
							case 3: // from current line begin
								goNextLine();
								SetTimer(1, 60000, NULL);
								break;
							case 4: // from head
								OnBnClickedButtonStart();
								break;
							}
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

void  ChostDlg::OnTimer(UINT nIDEvent)
{
	float at = 0.0;
	div_t h_m;
	switch(nIDEvent){
	case 0:
		downSend(cmdGetTemperature,0);// 通知下位机发送当前实际温度
		m_TotalTimes++;
		break;
	case 1:
		if(m_Pause){ // 程序暂停
			m_curLinePauseTime++;
			m_TotalPauseTime++;
		}else{ // 程序未暂停
			m_curLineTime++;
			at = m_dryLines[m_curLineNo][1]/60.0;
		}
		h_m = div((int)(m_TotalTimes/6),60);
		m_edtRunTime.Format(_T("%2d:%02d"),h_m.quot,h_m.rem);
		m_edtAreaTime.Format(_T("%2d:%02d"),m_curLineTime/60,m_curLineTime%60);
		m_edtRunPauseTime.Format(_T("%2d:%02d"),m_TotalPauseTime/60,m_TotalPauseTime%60);
		m_edtAreaPauseTime.Format(_T("%2d:%02d"),m_curLinePauseTime/60,m_curLinePauseTime%60);
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
		Sleep(100);
		downSend(cmdGetRoomTemperature, 0);// 通知下位机发送当前室温
		break;
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
		m_curLineNo = -1;
		SetTimer(0,10000,NULL);
		m_SerialPort.StartMonitoring();  //启动串口通信检测线程函数
		downSend(cmdGetRoomTemperature, 0);// 通知下位机发送当前室温
		Sleep(100);
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
		// here  
		pDC->SetBkColor(m_redcolor);    // change the background  
		// color [background colour  
		// of the text ONLY]  
		pDC->SetTextColor(m_textcolor); // change the text color  
		hbr = (HBRUSH) m_redbrush;    // apply the blue brush  
		// [this fills the control  
		// rectangle]  
		break;    
	   case IDC_EDIT_SETTINGTEMPERATURE:         // 设定温度文本框  
		// but control is still filled with the brush color!  
		pDC->SetBkMode(TRANSPARENT);   // make background transparent [only affects the TEXT itself]  
		pDC->SetTextColor(m_textcolor); // change the text color  
		hbr = (HBRUSH) m_bluebrush;     // apply the red brush [this fills the control rectangle]  
		break;  
	   case IDC_EDIT_AREAPAUSETIME:         // 设定温度文本框  
	   case IDC_EDIT_RUNPAUSETIME:         // 设定温度文本框  
		// but control is still filled with the brush color!  
		pDC->SetBkMode(TRANSPARENT);   // make background transparent [only affects the TEXT itself]  
		//pDC->SetTextColor(m_textcolor); // change the text color  
		hbr = (HBRUSH) m_yellowbrush;     // apply the red brush [this fills the control rectangle]  
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
	UINT TempPos = pScrollBar->GetScrollPos();
	UINT tPos = TempPos;
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
			if(TempPos > 0)
			{
				TempPos--;
			}
		break;
		case SB_LINERIGHT://点击右边的箭头
			if(TempPos<34560)
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
			m_dcMemHG.MoveTo(i-20,0);
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
	if (m_file.m_hFile != CFile::hFileNull && hdc){
		CScrollBar* pScrollBar = (CScrollBar*)GetDlgItem(IDC_SCROLLBAR_HFIGURE);
		int nCurpos = pScrollBar->GetScrollPos();
		int size = sizeof(WORD)* 4;
		ULONGLONG filesize = m_file.GetLength();
		ULONGLONG nSizes = (filesize - sizeof(dryHead)) / size; //m_ptrArray[0].GetSize();
		if (nSizes && nSizes > nCurpos+1){
			WORD record[4];
			ULONGLONG lPos;
			TRY{
				HPEN hPen1 = CreatePen(PS_SOLID, 1, m_bluecolor);
				HPEN hPen2 = CreatePen(PS_SOLID, 1, m_redcolor);
				lPos = m_file.Seek(nCurpos * size + sizeof(dryHead), CFile::begin);
				lPos += m_file.Read(m_record, size);
				toLP(m_record);
				for (ULONGLONG i = 1; i < m_nWidth && i < nSizes - 1; i++){
					if (lPos < filesize){
						lPos += m_file.Read(record, size);
						toLP(record);
						m_dcMem.SelectObject(hPen1);
						m_dcMem.MoveTo(m_record[2] - nCurpos, m_record[0]);
						m_dcMem.LineTo(record[2] - nCurpos, record[0]);
						m_dcMem.SelectObject(hPen2);
						m_dcMem.MoveTo(m_record[2] - nCurpos, m_record[1]);
						m_dcMem.LineTo(record[2] - nCurpos, record[1]);
						memcpy(m_record, record, size);
					}
				} 
				DeleteObject(hPen1);
				DeleteObject(hPen2);
			}
			CATCH(CFileException,e){
				#ifdef _DEBUG
				afxDump << "文件不能打开！" << e->m_cause << "\n";
				#endif
			}END_CATCH
		}
		hdc->BitBlt(m_nLeft, m_nTop, m_nWidth, m_nHeight, &m_dcMem, 0, 0, SRCCOPY);
		ReleaseDC(hdc);
	}
}


void ChostDlg::OnBnClickedButtonOption()
{
	// TODO: 在此添加控件通知处理程序代码
	optionDlg dlg;
	//m_pMainWnd = &dlg;
	dlg.m_dryLines.assign(m_dryLines.begin(),m_dryLines.end());

	dlg.m_cbHandPause = m_allowHandPause? "是":"否";
	dlg.m_cbLowPause = m_allowOperating[0]? "是":"否";
	dlg.m_cbTemperatureFilter = m_allowOperating[1]? "是":"否";
	dlg.m_cbOverHeratingWarning = m_allowOperating[2]? "是":"否";
	dlg.m_cbUltraLimitAlarming = m_allowOperating[3]? "是":"否";
	dlg.m_cbLowPauseValue = m_allowOperatingValue[0];
	dlg.m_cbTemperatureFilterValue = m_allowOperatingValue[1];
	dlg.m_cbOverHeratingValue = m_allowOperatingValue[2];
	dlg.m_cbUltraLimitAlarmingValue = m_allowOperatingValue[3];
	dlg.m_edTemperatureUpTime = m_upTemperatureTime;
	dlg.m_edSetTemperatureDownTime = m_downSetTemperatureTime;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
		m_dryLines.clear();
		m_dryLines.assign(dlg.m_dryLines.begin(),dlg.m_dryLines.end());
		m_allowHandPause = dlg.m_cbHandPause=="是";
		m_allowOperating[0] = dlg.m_cbLowPause=="是";
		m_allowOperating[1] = dlg.m_cbTemperatureFilter=="是";
		m_allowOperating[2] = dlg.m_cbOverHeratingWarning=="是";
		m_allowOperating[3] = dlg.m_cbUltraLimitAlarming=="是";
		m_allowOperatingValue[0] = dlg.m_cbLowPauseValue;
		m_allowOperatingValue[1] = dlg.m_cbTemperatureFilterValue;
		m_allowOperatingValue[2] = dlg.m_cbOverHeratingValue;
		m_allowOperatingValue[3] = dlg.m_cbUltraLimitAlarmingValue;
		m_upTemperatureTime = dlg.m_edTemperatureUpTime;
		m_downSetTemperatureTime = dlg.m_edSetTemperatureDownTime;
		m_startDryMode = dlg.m_rdStartMode;
		saveXML();
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


BOOL ChostDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	spDoc.Release();
	CoUninitialize();

	// 关闭记录文件
	if (m_file.m_hFile != CFile::hFileNull)
		m_file.Close();

	return CDialogEx::DestroyWindow();
}

// 读取干燥配置文件
void ChostDlg::loadXLM(void)
{
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
		CComPtr<IXMLDOMNode> spNode, spNode1, spNode2;
		BSTR tem;
		CString str;
		spDoc->selectSingleNode(OLESTR("/root/uptemperaturetime"), &spNode);
		spNode->get_text(&tem);
		str = tem;
		m_upTemperatureTime = _ttoi(str);// swscanf(str, _T("%d"), &m_upTemperatureTime);
		spDoc->selectSingleNode(OLESTR("/root/downsettemperaturetime"), &spNode1);
		spNode1->get_text(&tem);
		str = tem;
		m_downSetTemperatureTime = _ttoi(str);// swscanf(str, _T("%d"), &m_downSetTemperatureTime);
		spDoc->selectSingleNode(OLESTR("/root/allowhandpause"), &spNode2);
		spNode2->get_text(&tem);
		m_allowHandPause = (CString)tem == "TRUE";

		spDoc->selectNodes(OLESTR("/root/*"), &spNodeList1); //得到node2下的所有子节点 

		for (int i = 0; i < 4; i++){
			CComPtr<IXMLDOMNode> cpNode, spNodeAttrib0, spNodeAttrib1;
			CComPtr<IXMLDOMNamedNodeMap> spNameNodeMap;
			BSTR a1, a2;
			spNodeList1->get_item(i + 4, &cpNode);
			cpNode->get_attributes(&spNameNodeMap);
			spNameNodeMap->get_item(0, &spNodeAttrib0);
			spNodeAttrib0->get_text(&a1);
			m_allowOperating[i] = (CString)a1 == "TRUE";
			spNameNodeMap->get_item(1, &spNodeAttrib1);
			spNodeAttrib1->get_text(&a2);
			str = a2;
			m_allowOperatingValue[i] = _ttoi(str);// swscanf(str, _T("%d"), &m_allowOperatingValue[i]);
		}
	}
}

// 结束干燥
void ChostDlg::endDry(void)
{
	KillTimer(1);
	downSend(cmdSetLineNo,0);// 设置下位机当前段号为 0
	m_curLineNo = -1;
	Sleep(100);
	downSend(cmdRunningStatus, 4);// 设置下位机当前运行状态为 4(结束)
	m_file.Close();
	setCommCtrlEnable(FALSE, 18, 38);
}

// 向下位机发送指令或传送数据
void ChostDlg::downSend(char cmd, WORD degress)
{
	char send[5] = {0xfd,0x0,0x0,0x0,0xfe};
	send[1] = cmd;
	*(WORD*)(send+2) = degress;
	m_SerialPort.WriteToPort(send,5);
}


void ChostDlg::saveXML(void)
{
	CComPtr<IXMLDOMNodeList> spNodeList,spNodeList1; 
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
		linesNode->removeChild(spNode,NULL);
	}
	for (UINT i = 0; i < m_dryLines.size(); ++i) //遍历子节点 
	{ 
		CComPtr<IXMLDOMElement> lineEle,temperatureEle,rateEle,timeEle; 
		CString s[3];
		BSTR bs[3];
		for(int j=0;j<3;j++){
			s[j].Format(_T("%d"),m_dryLines[i][j]);
			bs[j] = s[j].AllocSysString();
		}
		spDoc->createElement(OLESTR("line"),&lineEle);
		spDoc->createElement(OLESTR("temperatureEle"),&temperatureEle);
		spDoc->createElement(OLESTR("rate"),&rateEle);
		spDoc->createElement(OLESTR("time"),&timeEle);
		temperatureEle->put_text(bs[0]);
		rateEle->put_text(bs[1]);
		timeEle->put_text(bs[2]);
		lineEle->appendChild(temperatureEle,NULL);
		lineEle->appendChild(rateEle,NULL);
		lineEle->appendChild(timeEle,NULL);
		
		linesNode->appendChild(lineEle,NULL);
	} 
	CComPtr<IXMLDOMNode> spNode,spNode1,spNode2;
	BSTR tem;
	CString str;
	spDoc->selectSingleNode(OLESTR("/root/uptemperaturetime"),&spNode);
	str.Format(_T("%d"),m_upTemperatureTime);
	tem = str.AllocSysString();
	spNode->put_text(tem);
	SysFreeString(tem); // 用完释放

	spDoc->selectSingleNode(OLESTR("/root/downsettemperaturetime"),&spNode1);
	str.Format(_T("%d"),m_downSetTemperatureTime);
	tem = str.AllocSysString();
	spNode1->put_text(tem);
	SysFreeString(tem); // 用完释放

	spDoc->selectSingleNode(OLESTR("/root/allowhandpause"),&spNode2);
	str = m_allowHandPause? "TRUE":"FALSE";
	tem = str.AllocSysString();
	spNode2->put_text(tem);
	SysFreeString(tem);// 用完释放

	spDoc->selectNodes(OLESTR("/root/*"), &spNodeList1); //得到node2下的所有子节点 

	for(int i=0;i<4;i++){
		CComPtr<IXMLDOMNode> cpNode,spNodeAttrib0,spNodeAttrib1;
		CComPtr<IXMLDOMNamedNodeMap> spNameNodeMap; 
		BSTR a1,a2;
		spNodeList1->get_item(i+4, &cpNode); 
		cpNode->get_attributes(&spNameNodeMap);
		spNameNodeMap->get_item(0, &spNodeAttrib0); 
		CString astr = m_allowOperating[i]?  _T("TRUE"):_T("FALSE");
		a1 = astr.AllocSysString();
		spNodeAttrib0->put_text(a1);
		SysFreeString(a1); // 用完释放

		spNameNodeMap->get_item(1, &spNodeAttrib1); 
		astr.Format(_T("%d"),m_allowOperatingValue[i]);
		a2 = astr.AllocSysString();
		spNodeAttrib1->put_text(a2);
		SysFreeString(a2); // 用完释放
	}
	spDoc->save(CComVariant(OLESTR("dryHost.xml")));
}


void ChostDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_startDryMode)
		dryBegin();
	else{
		SetTimer(2, 1000, NULL);
	}
}


void ChostDlg::goNextLine(void)
{
	m_curLineNo++;
	if(m_curLineNo<m_dryLines.size()){ // 干燥未结束
		int state;
		if(m_dryLines[m_curLineNo][1]>0) state = 0;
		else if(m_dryLines[m_curLineNo][1]==0) state = 1;
		else if(m_dryLines[m_curLineNo][1]<0) state = 2;

		m_curLineTime = 0;
		m_curLinePauseTime = 0;
		if(m_curLineNo==0){
			downSend(cmdSetSettingTemperature,(WORD)(m_lbTemperature*16));// 传送当前设定温度给下位机
			m_lbSettingtemperature = m_lbTemperature;
		}else{
			downSend(cmdSetSettingTemperature,(WORD)(m_dryLines[m_curLineNo-1][0]*16));// 传送当前设定温度给下位机
			m_lbSettingtemperature = m_dryLines[m_curLineNo][0];
		}

		m_edtArea.Format(_T("%d℃ %s"),m_dryLines[m_curLineNo][0],dryRunningStatus[state]);
	}else{
		endDry();
	}
}


void ChostDlg::adjuster(double temperature)
{
	m_Pause = FALSE;
	if(temperature < m_lbSettingtemperature-m_allowOperatingValue[2]){
		// 温度低
		if(m_allowOperating[2])
			Beep (1000,1000);
		m_edtRunning = "温度低";
		m_runbrush = &m_yellowbrush;
		if(temperature < m_lbSettingtemperature-m_allowOperatingValue[3]){
			m_edtRunning = "温度低，暂停";
			m_runbrush = &m_redbrush;
			if(m_allowOperating[3])
				Beep (2000,500);
			if(m_allowOperating[0]) // 是否允许低温暂停
				m_Pause = TRUE;
		}
	}else if(temperature > m_lbSettingtemperature+m_allowOperatingValue[2]){
		// 温度高
		m_edtRunning = "温度高";
		if(m_allowOperating[2])
			Beep (1000,500);
		m_runbrush = &m_yellowbrush;
		if(temperature > m_lbSettingtemperature+m_allowOperatingValue[3]){
			if(m_allowOperating[3])
				Beep (2000,1000);
			m_edtRunning = "温度高!!!";
			m_runbrush = &m_redbrush;
		}
	}else{ // 温度正常
		m_edtRunning = "正常";
		m_runbrush = &m_greenbrush;
	}
}


void ChostDlg::savePoint(WORD temperature)
{
	WORD record[4] = { m_lbSettingtemperature * 16, temperature, m_TotalTimes, m_curLineNo };
	int size = sizeof(WORD) * 4;

	m_file.SeekToEnd();
	m_file.Write(record, size);
	m_file.Flush();

	toLP(record);

	CScrollBar* pScrollBar = (CScrollBar*)GetDlgItem(IDC_SCROLLBAR_HFIGURE);
	if (pScrollBar){
		ULONGLONG filesize = m_file.GetLength();
		int nCurpos = pScrollBar->GetScrollPos();
		int nSize = (filesize - sizeof(dryHead)) / size;// m_ptrArray[0].GetSize();
		if (nSize > nCurpos+1){
			HPEN hPen1 = CreatePen(PS_SOLID, 1, m_bluecolor);
			HPEN hPen2 = CreatePen(PS_SOLID, 1, m_redcolor);
			m_dcMem.SelectObject(hPen1);
			m_dcMem.MoveTo(m_record[2] - nCurpos, m_record[0]);
			m_dcMem.LineTo(record[2] - nCurpos, record[0]);
			m_dcMem.SelectObject(hPen2);
			m_dcMem.MoveTo(m_record[2] - nCurpos, m_record[1]);
			m_dcMem.LineTo(record[2] - nCurpos, record[1]);
			DeleteObject(hPen1);
			DeleteObject(hPen2);
		}
		CDC *hdc = GetDC();
		if (hdc)
			hdc->BitBlt(m_nLeft, m_nTop, m_nWidth, m_nHeight, &m_dcMem, 0, 0, SRCCOPY);
		ReleaseDC(hdc);
	}
	memcpy(m_record, record, size);
	m_restoreTemperaturePoint.clear();
}


UINT ChostDlg::timeToSecond(CString time)
{
	int pos = time.Find(':');
	int hour;
	int minute;
	hour = _ttoi(time.Left(pos));// swscanf(time.Left(pos), _T("%d"), &hour);
	minute = _ttoi(time.Mid(pos + 1));// swscanf(time.Mid(pos + 1), _T("%d"), &minute);
	return (hour * 60 + minute) % 60;
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


WORD* ChostDlg::toLP(WORD * record)
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


void ChostDlg::dryBegin(void)
{
	CTime tm = CTime::GetCurrentTime();
	m_filename = tm.Format(L"dry%Y%m%d.dat");
	CString filehead = tm.Format(L"dry%Y%m%d%H%M%S");
	if (m_file.m_hFile != CFile::hFileNull)
		m_file.Close();
	CFileStatus status;
	CString t_filename = m_filename;
	int index = 0;
	while (CFile::GetStatus(t_filename, status)){
		t_filename.Format(L"%s%d", m_filename, index++);
	}
	if (index) CFile::Rename(m_filename, t_filename);
	m_curLineNo = 0;
	m_curLineTime = 0;
	//m_TotalTime = 0;
	m_TotalTimes = 0;
	m_curLinePauseTime = 0;
	m_TotalPauseTime = 0;
	m_dataInvalid = 0;
	downSend(cmdSetLineNo, 1);// 设置下位机当前段号为 1
	Sleep(100);
	downSend(cmdSetTime, 0);// 清零下位机总干燥时间
	int openState = m_file.Open(m_filename, CFile::typeBinary | CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	if (openState){
		m_file.Write(filehead, filehead.GetLength());
		m_file.Flush();
		SetTimer(1, 60000, NULL);
		setCommCtrlEnable(TRUE, 18, 38);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	}
	else{
		throw L"不能打开文件 " + m_filename;
	}
}
