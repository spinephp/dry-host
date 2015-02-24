
// hostDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "host.h"
#include "hostDlg.h"
#include "afxdialogex.h"
#include "windows.h"

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

	m_redcolor=RGB(255,0,0);                      // 红色  
	m_bluecolor=RGB(0,0,255);                     // 蓝色  
	m_greencolor=RGB(0,255,0);                     // 绿色  
	m_textcolor=RGB(255,255,255);                 // 文本颜色设置为白色  
	m_redbrush.CreateSolidBrush(m_redcolor);      // 红色背景色  
	m_bluebrush.CreateSolidBrush(m_bluecolor);    // 蓝色背景色  
	m_greenbrush.CreateSolidBrush(m_greencolor);    // 蓝色背景色  
	m_yellowbrush.CreateSolidBrush(RGB(0,255,255));    // 黄色背景色  

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
	char send[5] = {0xfd,0x0,0x0,0x0,0xfe};
	v_portin[v_index++] = ch;
	if(v_portin[v_index-1]==0xfe && v_index==5){ //接收完毕
		CString str; 
		BYTE cmd = v_portin[1];
		v_index = 0;
		switch(cmd){
			case cmdGetTemperature: // 当前实际温度
				str.Format("%5.1f",(*(WORD*)(v_portin+2))*0.0625); 
				m_lbTemperature = strtod(str,NULL);// = v_portin.one*0.0625; //将接收到的字符存入编辑框对应的变量中
				if(m_curLineNo>-1){// 干燥已开始
					m_Pause = FALSE;
					if(((int)m_lbSettingtemperature)==-100){
						char tem[20];
						m_lbSettingtemperature = m_lbTemperature;
						sprintf(tem,"%d℃ %s",m_dryLine[m_curLineNo][0],dryRunningStatus[0]);
						m_edtArea = tem;
					}
					if(m_lbTemperature < m_lbSettingtemperature-2){
						// 温度低
						Beep (1000,1000);
						m_edtRunning = "温度低";
						m_runbrush = &m_yellowbrush;
						if(m_lbTemperature < m_lbSettingtemperature-3){
							m_edtRunning = "温度低，暂停";
							m_runbrush = &m_redbrush;
							m_Pause = TRUE;
						}
					}else if(m_lbTemperature > m_lbSettingtemperature+2){
						// 温度高
						m_edtRunning = "温度高";
						Beep (1000,1000);
						m_runbrush = &m_yellowbrush;
						if(m_lbTemperature > m_lbSettingtemperature+3){
							m_edtRunning = "温度高!!!";
							m_runbrush = &m_redbrush;
						}
					}else{ // 温度正常
						m_edtRunning = "正常";
						m_runbrush = &m_greenbrush;
					}
				}else{
					m_edtRunning = "未开始";
					m_runbrush = NULL;
				}
				UpdateData(FALSE);  //将接收到的字符显示在接受编辑框中
				break;
			case cmdSettingTemperature: // 下位机当前的设定温度
				break;
			case cmdSettingLineNo: // 下位机当前段号
				switch(v_portin[2]){
				case 0: // 干燥未进行
					m_curLineNo = 0;
					m_curLineTime = 0;
					m_TotalTime = 0;
					send[1] = cmdSettingLineNo;
					send[2] = 1;
					send[3] = 0;
					m_SerialPort.WriteToPort(send,5); // 设置下位机当前段号为 1
					send[1] = cmdRunningStatus;
					send[2] = 0;
					send[3] = 0;
					m_SerialPort.WriteToPort(send,5); // 设置下位机当前运行状态为 0(升温)
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
	char send[5] = {0xfd,0x0,0x0,0x0,0xfe};
	float at = 0.0;
	switch(nIDEvent){
	case 0:
		send[1] = cmdGetTemperature;
		send[2] = 0;
		send[3] = 0;
		m_SerialPort.WriteToPort(send,5); // 通知下位机发送当前实际温度
		break;
	case 1:
		m_TotalTime++;
		if(!m_Pause){ // 程序未暂停
			m_curLineTime++;
			at = m_dryLine[m_curLineNo][1]/60.0;
		}
		if(m_dryLine[m_curLineNo][1]){ // 升温
			float t = m_lbSettingtemperature;
			t += at;
			if((short int)t<m_dryLine[m_curLineNo][0]){// 未升到段指定温度
				if((short int)t > (short int)m_lbSettingtemperature){
					send[1] = cmdSettingTemperature;
					*(WORD*)(send+2) = (short int)t;
					m_SerialPort.WriteToPort(send,5);// 传送当前设定温度给下位机
				}
				m_lbSettingtemperature = t;
			}else{ // 升温结束
				m_curLineNo++;
				m_curLineTime = 0;
				send[1] = cmdSettingTemperature;
				*(WORD*)(send+2) = m_dryLine[m_curLineNo][0];
				m_SerialPort.WriteToPort(send,5);// 传送当前设定温度给下位机

				sprintf(tem,"%d%s",m_dryLine[m_curLineNo][0],dryRunningStatus[1]);
				m_edtArea = tem;
			}
		}else if(m_curLineTime>m_dryLine[m_curLineNo][2]*60){ // 保温时间到
			m_curLineTime = 0;
			if(m_curLineNo<8){ // 干燥未结束
				send[1] = cmdSettingTemperature;
				*(WORD*)(send+2) = m_dryLine[m_curLineNo++][0];
				m_SerialPort.WriteToPort(send,5); // 传送当前设定温度给下位机

				sprintf(tem,"%d%s",m_dryLine[m_curLineNo][0],dryRunningStatus[0]);
				m_edtArea = tem;
			}else{
				KillTimer(1);
				send[1] = cmdSettingLineNo;
				*(WORD*)(send+2) = 0;
				m_SerialPort.WriteToPort(send,5); // 设置下位机当前段号为 1
				send[1] = cmdRunningStatus;
				*(WORD*)(send+2) = 4;
				m_SerialPort.WriteToPort(send,5); // 设置下位机当前运行状态为 4(结束)
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
		char send[5] = {0xfd,0x0,0x0,0x0,0xfe};
		m_curLineNo = -1;
		SetTimer(0,10000,NULL);
		m_SerialPort.StartMonitoring();  //启动串口通信检测线程函数
		send[1] = cmdGetTemperature;
		m_SerialPort.WriteToPort(send,5); // 取下位机当前实际温度
		send[1] = cmdSettingLineNo;
		send[2] = 0xff;
		m_SerialPort.WriteToPort(send,5); // 取下位机当前段号
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
