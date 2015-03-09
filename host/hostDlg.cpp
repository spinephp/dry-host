

// hostDlg.cpp : 实现文件
//

// 在程序的文件夹下生成MSXML3.TLH和MSXML3.TLI两个文件;  

#include "stdafx.h"
#include "host.h"
#include "hostDlg.h"
#include "optionDlg.h"
#include "afxdialogex.h"
#include "windows.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CGraph 
{
public:
	CPoint m_pt; // 起点
	UINT m_nDrawType; // 绘画类型
	CGraph();
	CGraph(UINT m_nDrawType,CPoint m_pt){
		this->m_nDrawType = m_nDrawType;
		this->m_pt = m_pt;
	}; // 此为构造函数。
};

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
	DDX_Text(pDX, IDC_EDIT_RUNNING,m_edtRunning);
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


	GetDlgItem(IDC_BUTTON_OPENPORT)->EnableWindow(!m_bPortOpen);
	GetDlgItem(IDC_BUTTON_CLOSEPORT)->EnableWindow(m_bPortOpen);

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
			char s[5];
			dc.MoveTo(m_nLeft-5,i);
			dc.LineTo(m_nLeft,i);
			if(n%50==0){
			sprintf(s,"%d",n);
			size = dc.GetTextExtent(s,strlen(s));
			dc.TextOutA(m_nLeft-7-size.cx,i-size.cy/2,s,strlen(s));
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
	if(v_portin[v_index-1]==0xfe && v_index==5){ //接收完毕
		CString str; 
		BYTE cmd = v_portin[1];
		short int temperature;
		v_index = 0;
		switch(cmd){
			case cmdGetTemperature: // 当前实际温度
				temperature = *(short int*)(v_portin+2);
				if(v_lastTemperature==-100)
					v_lastTemperature = temperature;
				if(m_allowOperating[1] && temperature > v_lastTemperature-m_smoothvalue && temperature < v_lastTemperature+m_smoothvalue || !m_allowOperating[1] && temperature > -800 && temperature < 3200){ // ，为正常，否则认为是由其它因素造成的误差
					v_lastTemperature = temperature;
					str.Format("%5.1f",temperature*0.0625); 
					m_lbTemperature = strtod(str,NULL);// = v_portin.one*0.0625; //将接收到的字符存入编辑框对应的变量中
					if(m_curLineNo>-1){// 干燥已开始
						m_Pause = FALSE;
						if(((int)m_lbSettingtemperature)==-100){
							m_lbSettingtemperature = m_lbTemperature;
							m_edtArea.Format("%d℃ %s",m_dryLines[m_curLineNo][0],dryRunningStatus[0]);
						}
						
						if(m_lbTemperature < m_lbSettingtemperature-m_allowOperatingValue[2]){
							// 温度低
							if(m_allowOperating[2])
								Beep (1000,1000);
							m_edtRunning = "温度低";
							m_runbrush = &m_yellowbrush;
							if(m_lbTemperature < m_lbSettingtemperature-m_allowOperatingValue[3]){
								m_edtRunning = "温度低，暂停";
								m_runbrush = &m_redbrush;
								if(m_allowOperating[3])
									Beep (2000,500);
								if(m_allowOperating[0]) // 是否允许低温暂停
									m_Pause = TRUE;
							}
						}else if(m_lbTemperature > m_lbSettingtemperature+m_allowOperatingValue[2]){
							// 温度高
							m_edtRunning = "温度高";
							if(m_allowOperating[2])
								Beep (1000,500);
							m_runbrush = &m_yellowbrush;
							if(m_lbTemperature > m_lbSettingtemperature+m_allowOperatingValue[3]){
								if(m_allowOperating[3])
									Beep (2000,1000);
								m_edtRunning = "温度高!!!";
								m_runbrush = &m_redbrush;
							}
						}else{ // 温度正常
							m_edtRunning = "正常";
							m_runbrush = &m_greenbrush;
						}

						CPoint pt(m_TotalTimes,m_lbTemperature+50);
						CPoint pt1(m_TotalTimes,m_lbSettingtemperature+50);
						m_dcMem.DPtoLP(&pt);
						m_dcMem.DPtoLP(&pt1);
						m_ptrArray[0].Add(new CGraph(0,pt));
						m_ptrArray[1].Add(new CGraph(0,pt1));
						DrawTemperatureLine();
					}else{
						m_edtRunning = "未开始";
						m_runbrush = NULL;
					}
					UpdateData(FALSE);  //将接收到的字符显示在接受编辑框中
				}else{
					m_dataInvalid++;
				}
				break;
			case cmdSettingTemperature: // 下位机当前的设定温度
				break;
			case cmdSettingLineNo: // 下位机当前段号
				switch(v_portin[2]){
				case 0: // 干燥未进行
					m_curLineNo = 0;
					m_curLineTime = 0;
					m_TotalTime = 0;
					m_TotalTimes = 0;
					m_curLinePauseTime = 0;
					m_TotalPauseTime = 0;
					m_dataInvalid = 0;
					downSend(cmdSettingLineNo,1);// 设置下位机当前段号为 1
					downSend(cmdRunningStatus,0);// 设置下位机当前运行状态为 0(升温)
					SetTimer(1,60000,NULL);
					break;
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
	char tem[20];
	float at = 0.0;
	switch(nIDEvent){
	case 0:
		downSend(cmdGetTemperature,0);// 通知下位机发送当前实际温度
		m_TotalTimes++;
		break;
	case 1:
		m_TotalTime++;
		if(m_Pause){ // 程序暂停
			m_curLinePauseTime++;
			m_TotalPauseTime++;
		}else{ // 程序未暂停
			m_curLineTime++;
			at = m_dryLines[m_curLineNo][1]/60.0;
		}
		m_edtRunTime.Format("%2d:%02d",m_TotalTime/60,m_TotalTime%60);
		m_edtAreaTime.Format("%2d:%02d",m_curLineTime/60,m_curLineTime%60);
		m_edtRunPauseTime.Format("%2d:%02d",m_TotalPauseTime/60,m_TotalPauseTime%60);
		m_edtAreaPauseTime.Format("%2d:%02d",m_curLinePauseTime/60,m_curLinePauseTime%60);
		if(m_dryLines[m_curLineNo][1]){ // 升温
			float t = m_lbSettingtemperature;
			t += at;
			if((short int)t<m_dryLines[m_curLineNo][0]){// 未升到段指定温度
				if((short int)t > (short int)m_lbSettingtemperature){
					downSend(cmdSettingTemperature,t);// 传送当前设定温度给下位机
				}
				m_lbSettingtemperature = t;
			}else{ // 升温结束
				m_curLineNo++;
				if(m_curLineNo<m_dryLines.size()){ // 干燥未结束
					m_curLineTime = 0;
					m_curLinePauseTime = 0;
					downSend(cmdSettingTemperature,m_dryLines[m_curLineNo][0]);// 传送当前设定温度给下位机
					m_lbSettingtemperature = m_dryLines[m_curLineNo][0];

					m_edtArea.Format("%d℃ %s",m_dryLines[m_curLineNo][0],dryRunningStatus[1]);
				}else{
					endDry();
				}
			}
		}else if(m_curLineTime>m_dryLines[m_curLineNo][2]*60){ // 保温时间到
			m_curLineNo++;
			m_curLineTime = 0;
			m_curLinePauseTime = 0;
			if(m_curLineNo<m_dryLines.size()){ // 干燥未结束
				downSend(cmdSettingTemperature,m_dryLines[m_curLineNo][0]);// 传送当前设定温度给下位机

				m_edtArea.Format("%d℃ %s",m_dryLines[m_curLineNo][0],dryRunningStatus[0]);
			}else{
				endDry();
			}
		}
		UpdateData(FALSE);     //读入编辑框的数据
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
  comarray.Add(CString(databuffer));
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
	UINT nBuad = atoi(str);
	UINT iParity = m_cbVerifybit.GetCurSel();
	m_cbDatabit.GetLBText(m_cbDatabit.GetCurSel(),str1);   //得到数据位
	UINT nDatabit = atoi(str1);
	UINT nStopbit = m_cbStopbit.GetCurSel();
	if(m_SerialPort.InitPort(m_hWnd,nPort,nBuad,"NOE"[iParity],nDatabit,nStopbit,EV_RXFLAG | EV_RXCHAR,512))
	{
		m_curLineNo = -1;
		SetTimer(0,10000,NULL);
		m_SerialPort.StartMonitoring();  //启动串口通信检测线程函数
		downSend(cmdGetTemperature,0); // 取下位机当前实际温度
		downSend(cmdSettingLineNo,0xff);  // 取下位机当前段号
	}
	else
	{
		MessageBox(_T("没有发现此串口或者被占用"));
	}
	m_bPortOpen = m_SerialPort.IsOpen();
	GetDlgItem(IDC_BUTTON_OPENPORT)->EnableWindow(!m_bPortOpen);
	GetDlgItem(IDC_BUTTON_CLOSEPORT)->EnableWindow(m_bPortOpen);
}


void ChostDlg::OnBnClickedButtonCloseport()  //关闭串口按钮消息响应函数
{
	// TODO: 在此添加控件通知处理程序代码
	m_SerialPort.ClosePort();
	m_bPortOpen = m_SerialPort.IsOpen();
	GetDlgItem(IDC_BUTTON_OPENPORT)->EnableWindow(!m_bPortOpen);
	GetDlgItem(IDC_BUTTON_CLOSEPORT)->EnableWindow(m_bPortOpen);
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
	int TempPos = pScrollBar->GetScrollPos();
	switch(nSBCode)
	{
		case SB_THUMBPOSITION://拖动滑块
			pScrollBar->SetScrollPos(nPos);
			break;
		case SB_LINELEFT://点击左边的箭头
			if(TempPos > 0)
			{
				TempPos--;
			}
			pScrollBar->SetScrollPos(TempPos);
		break;
		case SB_LINERIGHT://点击右边的箭头
			if(TempPos<34560)
			{
				TempPos++;
			}
			pScrollBar->SetScrollPos(TempPos);
		break;
	} 
	DrawTemperatureLine();
	SetHStaff();
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
			char s[6];
			m_dcMemHG.MoveTo(i-20,0);
			m_dcMemHG.LineTo(i-20,5);
			sprintf(s,"%d:%02d",n/6,(n%6)*10);
			CSize size = m_dcMemTime.GetTextExtent(s,strlen(s));
			m_dcMemTime.TextOutA(i-size.cx/2,0,s,strlen(s));
		}
	}
	GetDC()->BitBlt(m_nLeft, m_nTop+m_nHeight, m_nWidth, m_nHeight+6, &m_dcMemHG, 0,0, SRCCOPY);
	GetDC()->BitBlt(m_tLeft, m_tTop, m_tWidth, m_tHeight, &m_dcMemTime, 0,0, SRCCOPY);
}

// 绘制温度曲线
void ChostDlg::DrawTemperatureLine(void)
{
	CScrollBar* pScrollBar = (CScrollBar*)GetDlgItem(IDC_SCROLLBAR_HFIGURE);
	int nCurpos=pScrollBar->GetScrollPos();
	int nSizes=m_ptrArray[0].GetSize();
	m_dcMem.FillSolidRect(0,0, m_nWidth, m_nHeight,RGB(255,255,255));
	if(nSizes && nSizes > nCurpos){
		//m_dcMem.BitBlt(0, 0, m_nWidth, m_nHeight, &m_dcBack, 0, 0, SRCCOPY);// 绘制背景
		for(int j=0;j<2;j++){
			m_dcMem.SelectObject(CreatePen(PS_SOLID,1,j? m_bluecolor:m_redcolor));
			CGraph *g = (CGraph *)m_ptrArray[j].GetAt(nCurpos);
			m_dcMem.MoveTo(g->m_pt.x-nCurpos,g->m_pt.y);
			for(int i=nCurpos+1;i<m_ptrArray[j].GetSize() && i<nCurpos+m_nWidth;i++){
				g = (CGraph *)m_ptrArray[j].GetAt(i);
				m_dcMem.LineTo(g->m_pt.x-nCurpos,g->m_pt.y);
			}
		}
	}
	GetDC()->BitBlt(m_nLeft, m_nTop, m_nWidth, m_nHeight, &m_dcMem, 0,0, SRCCOPY);
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
	CoUninitialize();

	return CDialogEx::DestroyWindow();
}


void ChostDlg::loadXLM(void)
{
	//读取XML 
	spDoc.CoCreateInstance(CLSID_DOMDocument); 
	VARIANT_BOOL vb; 
	spDoc->load(CComVariant(OLESTR("dryHost.xml")), &vb); //加载XML文件 
	spDoc->get_documentElement(&spRootEle); //根节点 
	CComPtr<IXMLDOMNodeList> spNodeList,spNodeList1; 

	// 取干燥曲线参数
	spDoc->selectNodes(OLESTR("/root/lines/*"), &spNodeList); //得到node2下的所有子节点 
	long nLen; 
	spNodeList->get_length(&nLen); //子节点数 
	for (long i = 0; i < nLen; ++i) //遍历子节点 
	{ 
		CComPtr<IXMLDOMNode> spNode,chNode,chNode1,chNode2; 
		spNodeList->get_item(i, &spNode); 
		BSTR temperature,rate,time;
		spNode->get_firstChild(&chNode);
		chNode->get_text(&temperature); //节点值 
		chNode->get_nextSibling(&chNode1);
		chNode1->get_text(&rate); //节点值 
		chNode1->get_nextSibling(&chNode2);
		chNode2->get_text(&time); //节点值 

		CString s0(temperature),s1(rate),s2(time);
		vector <int> line; 
		line.push_back(atoi(s0));
		line.push_back(atoi(s1));
		line.push_back(atoi(s2));
		m_dryLines.push_back(line);
	
	} 
	CComPtr<IXMLDOMNode> spNode,spNode1,spNode2;
	BSTR tem;
	CString str;
	spDoc->selectSingleNode(OLESTR("/root/uptemperaturetime"),&spNode);
	spNode->get_text(&tem);
	str = tem;
	m_upTemperatureTime = atoi(str);
	spDoc->selectSingleNode(OLESTR("/root/downsettemperaturetime"),&spNode1);
	spNode1->get_text(&tem);
	str = tem;
	m_downSetTemperatureTime = atoi(str);
	spDoc->selectSingleNode(OLESTR("/root/allowhandpause"),&spNode2);
	spNode2->get_text(&tem);
	m_allowHandPause = (CString)tem=="TRUE";

	spDoc->selectNodes(OLESTR("/root/*"), &spNodeList1); //得到node2下的所有子节点 

	for(int i=0;i<4;i++){
		CComPtr<IXMLDOMNode> cpNode,spNodeAttrib0,spNodeAttrib1;
		CComPtr<IXMLDOMNamedNodeMap> spNameNodeMap; 
		BSTR a1,a2;
		spNodeList1->get_item(i+4, &cpNode); 
		cpNode->get_attributes(&spNameNodeMap);
		spNameNodeMap->get_item(0, &spNodeAttrib0); 
		spNodeAttrib0->get_text(&a1);
		m_allowOperating[i] = (CString)a1 == "TRUE";
		spNameNodeMap->get_item(1, &spNodeAttrib1); 
		spNodeAttrib1->get_text(&a2);
		str = a2;
		m_allowOperatingValue[i] = atoi(str);
	}
}

// 结束干燥
void ChostDlg::endDry(void)
{
	KillTimer(1);
	downSend(cmdSettingLineNo,0);// 设置下位机当前段号为 0
	downSend(cmdRunningStatus,4);// 设置下位机当前运行状态为 4(结束)
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

	// 取干燥曲线参数
	spDoc->selectNodes(OLESTR("/root/lines/*"), &spNodeList); //得到node2下的所有子节点 
	
	long nLen; 
	spNodeList->get_length(&nLen); //子节点数 
	for (long i = 0; i < nLen; ++i) //遍历子节点 
	{ 
		CComPtr<IXMLDOMNode> spNode;
		spNodeList->get_item(i, &spNode);
		if(i<m_dryLines.size()){
			CComPtr<IXMLDOMNode> chNode,chNode1,chNode2; 
			CString s[3];
			BSTR bs[3];
			for(int j=0;j<3;j++){
				s[j].Format("%d",m_dryLines[i][j]);
				bs[j] = s[j].AllocSysString();
			}
			spNode->get_firstChild(&chNode);
			chNode->put_text(bs[0]); //节点值 
			chNode->get_nextSibling(&chNode1);
			chNode1->put_text(bs[1]); //节点值 
			chNode1->get_nextSibling(&chNode2);
			chNode2->put_text(bs[2]); //节点值 
		}	
	} 
	CComPtr<IXMLDOMNode> spNode,spNode1,spNode2;
	BSTR tem;
	CString str;
	spDoc->selectSingleNode(OLESTR("/root/uptemperaturetime"),&spNode);
	str.Format("%d",m_upTemperatureTime);
	tem = str.AllocSysString();
	spNode->put_text(tem);
	SysFreeString(tem); // 用完释放

	spDoc->selectSingleNode(OLESTR("/root/downsettemperaturetime"),&spNode1);
	str.Format("%d",m_downSetTemperatureTime);
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
		CString astr = m_allowOperating[i]?  "TRUE":"FALSE";
		a1 = astr.AllocSysString();
		spNodeAttrib0->put_text(a1);
		SysFreeString(a1); // 用完释放

		spNameNodeMap->get_item(1, &spNodeAttrib1); 
		astr.Format("%d",m_allowOperatingValue[i]);
		a2 = astr.AllocSysString();
		spNodeAttrib1->put_text(a2);
		SysFreeString(a2); // 用完释放
	}
}
