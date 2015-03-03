

// hostDlg.cpp : ʵ���ļ�
//

// �ڳ�����ļ���������MSXML3.TLH��MSXML3.TLI�����ļ�;  

#include "stdafx.h"
#include "host.h"
#include "hostDlg.h"
#include "optionDlg.h"
#include "afxdialogex.h"
#include "windows.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <msxml2.h>

class CGraph 
{
public:
	CPoint m_pt; // ���
	UINT m_nDrawType; // �滭����
	CGraph();
	CGraph(UINT m_nDrawType,CPoint m_pt){
		this->m_nDrawType = m_nDrawType;
		this->m_pt = m_pt;
	}; // ��Ϊ���캯����
};

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()

};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    // ����MFCĬ�ϵ����ݽ���   
    CDialogEx::DoDataExchange(pDX);   
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// ChostDlg �Ի���



ChostDlg::ChostDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ChostDlg::IDD, pParent)
	, m_cbxPort(_T(""))
	, m_cbxBaudrate(_T("9600"))
	, m_cbxVerifybit(_T("��"))
	, m_cbxDatabit(_T("8"))
	, m_cbxStopbit(_T("1"))
	, m_lbTemperature(0)
	, m_lbSettingtemperature(-100)
	, m_edtRunning(_T("δ����"))
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

// ChostDlg ��Ϣ�������

BOOL ChostDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	CArray<CString ,CString> comPort;
	GetSystemSerialComport(comPort);
	int size=comPort.GetSize();
	for(int i=0;i<size;i++){
		CString s = comPort.GetAt(i);
		m_cbPort.AddString(s);
	}

	m_cbPort.SetCurSel(0); //��ʼѡ�񴮿�1
	m_bPortOpen = FALSE;


	GetDlgItem(IDC_BUTTON_OPENPORT)->EnableWindow(!m_bPortOpen);
	GetDlgItem(IDC_BUTTON_CLOSEPORT)->EnableWindow(m_bPortOpen);

	v_index = 0;
	v_lastTemperature = -100;
	m_smooth = FALSE;
	m_canPause = TRUE; // �����Զ���ͣ
	m_smoothvalue = 16; // ���� 10 �����¶ȱ仯��ֵ�� 1�� ֮��

	m_redcolor=RGB(255,0,0);                      // ��ɫ  
	m_bluecolor=RGB(0,0,255);                     // ��ɫ  
	m_greencolor=RGB(0,255,0);                     // ��ɫ  
	m_textcolor=RGB(255,255,255);                 // �ı���ɫ����Ϊ��ɫ  
	m_redbrush.CreateSolidBrush(m_redcolor);      // ��ɫ����ɫ  
	m_bluebrush.CreateSolidBrush(m_bluecolor);    // ��ɫ����ɫ  
	m_greenbrush.CreateSolidBrush(m_greencolor);    // ��ɫ����ɫ  
	m_yellowbrush.CreateSolidBrush(RGB(255,255,0));    // ��ɫ����ɫ  

	CRect rect;
	CPaintDC dc(this);
	GetDlgItem(IDC_STATIC_RUNNINGTIME)->GetWindowRect(&rect);//��ȡ�ؼ��������Ļ��λ��
	ScreenToClient(rect);//ת��Ϊ�Ի����ϵ����λ��
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
	pScrollBar->SetScrollRange(0,34560);//�����ƶ���λ��Ϊ0����34560��
	DrawTemperatureLine();
	SetHStaff();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void ChostDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
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

		// ��ʾˮƽ���
		dc.BitBlt(m_nLeft, m_nTop+m_nHeight, m_nWidth, m_nHeight+6, &m_dcMemHG, 0,0, SRCCOPY);// ��ʾ�̶���
		dc.BitBlt(m_tLeft, m_tTop, m_tWidth, m_tHeight, &m_dcMemTime, 0,0, SRCCOPY); // ��ʾ�̶�ʱ��

		// �����¶�����
		dc.BitBlt(m_nLeft, m_nTop, m_nWidth, m_nHeight, &m_dcMem, 0,0, SRCCOPY);

		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR ChostDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//�ַ�������Ϣ��Ӧ����
LONG ChostDlg::OnComm(WPARAM ch,LPARAM port)
{
	char send[5] = {0xfd,0x0,0x0,0x0,0xfe};
	v_portin[v_index++] = ch;
	if(v_portin[v_index-1]==0xfe && v_index==5){ //�������
		CString str; 
		BYTE cmd = v_portin[1];
		short int temperature;
		v_index = 0;
		switch(cmd){
			case cmdGetTemperature: // ��ǰʵ���¶�
				temperature = *(short int*)(v_portin+2);
				if(v_lastTemperature==-100)
					v_lastTemperature = temperature;
				if(m_smooth && temperature > v_lastTemperature-m_smoothvalue && temperature < v_lastTemperature+m_smoothvalue || !m_smooth && temperature > -800 && temperature < 3200){ // ��Ϊ������������Ϊ��������������ɵ����
					v_lastTemperature = temperature;
					str.Format("%5.1f",temperature*0.0625); 
					m_lbTemperature = strtod(str,NULL);// = v_portin.one*0.0625; //�����յ����ַ�����༭���Ӧ�ı�����
					if(m_curLineNo>-1){// �����ѿ�ʼ
						m_Pause = FALSE;
						if(((int)m_lbSettingtemperature)==-100){
							m_lbSettingtemperature = m_lbTemperature;
							m_edtArea.Format("%d�� %s",m_dryLine[m_curLineNo][0],dryRunningStatus[0]);
						}
						
						if(m_lbTemperature < m_lbSettingtemperature-2){
							// �¶ȵ�
							Beep (1000,1000);
							m_edtRunning = "�¶ȵ�";
							m_runbrush = &m_yellowbrush;
							if(m_lbTemperature < m_lbSettingtemperature-3){
								m_edtRunning = "�¶ȵͣ���ͣ";
								m_runbrush = &m_redbrush;
								if(m_canPause)
									m_Pause = TRUE;
							}
						}else if(m_lbTemperature > m_lbSettingtemperature+2){
							// �¶ȸ�
							m_edtRunning = "�¶ȸ�";
							Beep(1000,1000);
							m_runbrush = &m_yellowbrush;
							if(m_lbTemperature > m_lbSettingtemperature+3){
								m_edtRunning = "�¶ȸ�!!!";
								m_runbrush = &m_redbrush;
							}
						}else{ // �¶�����
							m_edtRunning = "����";
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
						m_edtRunning = "δ��ʼ";
						m_runbrush = NULL;
					}
					UpdateData(FALSE);  //�����յ����ַ���ʾ�ڽ��ܱ༭����
				}else{
					m_dataInvalid++;
				}
				break;
			case cmdSettingTemperature: // ��λ����ǰ���趨�¶�
				break;
			case cmdSettingLineNo: // ��λ����ǰ�κ�
				switch(v_portin[2]){
				case 0: // ����δ����
					m_curLineNo = 0;
					m_curLineTime = 0;
					m_TotalTime = 0;
					m_TotalTimes = 0;
					m_curLinePauseTime = 0;
					m_TotalPauseTime = 0;
					m_dataInvalid = 0;
					send[1] = cmdSettingLineNo;
					send[2] = 1;
					send[3] = 0;
					m_SerialPort.WriteToPort(send,5); // ������λ����ǰ�κ�Ϊ 1
					send[1] = cmdRunningStatus;
					send[2] = 0;
					send[3] = 0;
					m_SerialPort.WriteToPort(send,5); // ������λ����ǰ����״̬Ϊ 0(����)
					SetTimer(1,60000,NULL);
					break;
				}
				break;
			case cmdRunningStatus: // ��λ����ǰ������ʱ��,��λ����
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
		m_SerialPort.WriteToPort(send,5); // ֪ͨ��λ�����͵�ǰʵ���¶�
		m_TotalTimes++;
		break;
	case 1:
		m_TotalTime++;
		if(m_Pause){ // ������ͣ
			m_curLinePauseTime++;
			m_TotalPauseTime++;
		}else{ // ����δ��ͣ
			m_curLineTime++;
			at = m_dryLine[m_curLineNo][1]/60.0;
		}
		m_edtRunTime.Format("%2d:%02d",m_TotalTime/60,m_TotalTime%60);
		m_edtAreaTime.Format("%2d:%02d",m_curLineTime/60,m_curLineTime%60);
		m_edtRunPauseTime.Format("%2d:%02d",m_TotalPauseTime/60,m_TotalPauseTime%60);
		m_edtAreaPauseTime.Format("%2d:%02d",m_curLinePauseTime/60,m_curLinePauseTime%60);
		if(m_dryLine[m_curLineNo][1]){ // ����
			float t = m_lbSettingtemperature;
			t += at;
			if((short int)t<m_dryLine[m_curLineNo][0]){// δ������ָ���¶�
				if((short int)t > (short int)m_lbSettingtemperature){
					send[1] = cmdSettingTemperature;
					*(WORD*)(send+2) = (short int)t;
					m_SerialPort.WriteToPort(send,5);// ���͵�ǰ�趨�¶ȸ���λ��
				}
				m_lbSettingtemperature = t;
			}else{ // ���½���
				m_curLineNo++;
				m_curLineTime = 0;
				m_curLinePauseTime = 0;
				send[1] = cmdSettingTemperature;
				*(WORD*)(send+2) = m_dryLine[m_curLineNo][0];
				m_SerialPort.WriteToPort(send,5);// ���͵�ǰ�趨�¶ȸ���λ��

				m_edtArea.Format("%d%s",m_dryLine[m_curLineNo][0],dryRunningStatus[1]);
			}
		}else if(m_curLineTime>m_dryLine[m_curLineNo][2]*60){ // ����ʱ�䵽
			m_curLineTime = 0;
			m_curLinePauseTime = 0;
			if(m_curLineNo<8){ // ����δ����
				send[1] = cmdSettingTemperature;
				*(WORD*)(send+2) = m_dryLine[m_curLineNo++][0];
				m_SerialPort.WriteToPort(send,5); // ���͵�ǰ�趨�¶ȸ���λ��

				m_edtArea.Format("%d%s",m_dryLine[m_curLineNo][0],dryRunningStatus[0]);
			}else{
				KillTimer(1);
				send[1] = cmdSettingLineNo;
				*(WORD*)(send+2) = 0;
				m_SerialPort.WriteToPort(send,5); // ������λ����ǰ�κ�Ϊ 1
				send[1] = cmdRunningStatus;
				*(WORD*)(send+2) = 4;
				m_SerialPort.WriteToPort(send,5); // ������λ����ǰ����״̬Ϊ 4(����)
			}
		}
		UpdateData(FALSE);     //����༭�������
		break;
	}
	v_index = 0;
	CDialogEx::OnTimer(nIDEvent);
}

void ChostDlg::OnButtonSend() 
{
 // TODO: Add your control notification handler code here
 if(!m_SerialPort.IsOpen())
 return ;      //��⴮���Ƿ��
 UpdateData(TRUE);     //����༭�������
 //m_SerialPort.WriteToPort((LPCTSTR)m_strTxMsg);  //��������
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


void ChostDlg::OnBnClickedButtonOpenport() //�򿪴��ڰ�ť��Ϣ��Ӧ����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str,str1;
	UINT nPort=m_cbPort.GetCurSel()+1;   //�õ����ں�
	m_cbBaudrate.GetLBText(m_cbBaudrate.GetCurSel(),str);   //�õ�������
	UINT nBuad = atoi(str);
	UINT iParity = m_cbVerifybit.GetCurSel();
	m_cbDatabit.GetLBText(m_cbDatabit.GetCurSel(),str1);   //�õ�����λ
	UINT nDatabit = atoi(str1);
	UINT nStopbit = m_cbStopbit.GetCurSel();
	if(m_SerialPort.InitPort(m_hWnd,nPort,nBuad,"NOE"[iParity],nDatabit,nStopbit,EV_RXFLAG | EV_RXCHAR,512))
	{
		char send[5] = {0xfd,0x0,0x0,0x0,0xfe};
		m_curLineNo = -1;
		SetTimer(0,10000,NULL);
		m_SerialPort.StartMonitoring();  //��������ͨ�ż���̺߳���
		send[1] = cmdGetTemperature;
		m_SerialPort.WriteToPort(send,5); // ȡ��λ����ǰʵ���¶�
		send[1] = cmdSettingLineNo;
		send[2] = 0xff;
		m_SerialPort.WriteToPort(send,5); // ȡ��λ����ǰ�κ�
	}
	else
	{
		MessageBox(_T("û�з��ִ˴��ڻ��߱�ռ��"));
	}
	m_bPortOpen = m_SerialPort.IsOpen();
	GetDlgItem(IDC_BUTTON_OPENPORT)->EnableWindow(!m_bPortOpen);
	GetDlgItem(IDC_BUTTON_CLOSEPORT)->EnableWindow(m_bPortOpen);
}


void ChostDlg::OnBnClickedButtonCloseport()  //�رմ��ڰ�ť��Ϣ��Ӧ����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_SerialPort.ClosePort();
	m_bPortOpen = m_SerialPort.IsOpen();
	GetDlgItem(IDC_BUTTON_OPENPORT)->EnableWindow(!m_bPortOpen);
	GetDlgItem(IDC_BUTTON_CLOSEPORT)->EnableWindow(m_bPortOpen);
}



HBRUSH ChostDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here  
	switch (nCtlColor) //������ͬһ���͵Ŀؼ������ж�  
	{  
	   // process my edit controls by ID.  
	case CTLCOLOR_EDIT:  
	case CTLCOLOR_MSGBOX://����ؼ����ı��������Ϣ���������һ��switch  
	   switch (pWnd->GetDlgCtrlID())//��ĳһ���ض��ؼ������ж�  
	   {      
	   case IDC_EDIT_TEMPERATURE:         // ʵ���¶��ı���  
		// here  
		pDC->SetBkColor(m_redcolor);    // change the background  
		// color [background colour  
		// of the text ONLY]  
		pDC->SetTextColor(m_textcolor); // change the text color  
		hbr = (HBRUSH) m_redbrush;    // apply the blue brush  
		// [this fills the control  
		// rectangle]  
		break;    
	   case IDC_EDIT_SETTINGTEMPERATURE:         // �趨�¶��ı���  
		// but control is still filled with the brush color!  
		pDC->SetBkMode(TRANSPARENT);   // make background transparent [only affects the TEXT itself]  
		pDC->SetTextColor(m_textcolor); // change the text color  
		hbr = (HBRUSH) m_bluebrush;     // apply the red brush [this fills the control rectangle]  
		break;  
	   case IDC_EDIT_AREAPAUSETIME:         // �趨�¶��ı���  
	   case IDC_EDIT_RUNPAUSETIME:         // �趨�¶��ı���  
		// but control is still filled with the brush color!  
		pDC->SetBkMode(TRANSPARENT);   // make background transparent [only affects the TEXT itself]  
		//pDC->SetTextColor(m_textcolor); // change the text color  
		hbr = (HBRUSH) m_yellowbrush;     // apply the red brush [this fills the control rectangle]  
		break;  
	   case IDC_EDIT_RUNNING:         // �趨�¶��ı���  
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int TempPos = pScrollBar->GetScrollPos();
	switch(nSBCode)
	{
		case SB_THUMBPOSITION://�϶�����
			pScrollBar->SetScrollPos(nPos);
			break;
		case SB_LINELEFT://�����ߵļ�ͷ
			if(TempPos > 0)
			{
				TempPos--;
			}
			pScrollBar->SetScrollPos(TempPos);
		break;
		case SB_LINERIGHT://����ұߵļ�ͷ
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

// ����ˮƽ���
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

// �����¶�����
void ChostDlg::DrawTemperatureLine(void)
{
	CScrollBar* pScrollBar = (CScrollBar*)GetDlgItem(IDC_SCROLLBAR_HFIGURE);
	int nCurpos=pScrollBar->GetScrollPos();
	int nSizes=m_ptrArray[0].GetSize();
	m_dcMem.FillSolidRect(0,0, m_nWidth, m_nHeight,RGB(255,255,255));
	if(nSizes && nSizes > nCurpos){
		//m_dcMem.BitBlt(0, 0, m_nWidth, m_nHeight, &m_dcBack, 0, 0, SRCCOPY);// ���Ʊ���
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	optionDlg dlg;
	//m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}
}
