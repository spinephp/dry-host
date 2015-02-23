
// hostDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "host.h"
#include "hostDlg.h"
#include "afxdialogex.h"
#include "windows.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
	ON_CBN_SELCHANGE(IDC_COMBO7, &ChostDlg::OnCbnSelchangeCombo7)

	//{{AFX_MSG_MAP(ChostDlg)
	ON_MESSAGE(WM_COMM_RXCHAR,OnComm)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_OPENPORT, &ChostDlg::OnBnClickedButtonOpenport)
	ON_BN_CLICKED(IDC_BUTTON_CLOSEPORT, &ChostDlg::OnBnClickedButtonCloseport)
	ON_CBN_SELCHANGE(IDC_COMBO_VERIFYBIT, &ChostDlg::OnCbnSelchangeComboVerifybit)
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR ChostDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void ChostDlg::OnCbnSelchangeCombo7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

//�ַ�������Ϣ��Ӧ����
LONG ChostDlg::OnComm(WPARAM ch,LPARAM port)
{
	char send[5] = {0xfd,0x0,0x0,0x0,0xfe};
	v_portin[v_index++] = ch;
	if(v_portin[v_index-1]==0xfe && v_index==5){ //�������
		CString str; 
		BYTE cmd = v_portin[1];
		v_index = 0;
		switch(cmd){
			case cmdGetTemperature: // ��ǰʵ���¶�
				str.Format("%5.1f",(*(WORD*)(v_portin+2))*0.0625); 
				m_lbTemperature = strtod(str,NULL);// = v_portin.one*0.0625; //�����յ����ַ�����༭���Ӧ�ı�����
				if(m_curLineNo>-1){// �����ѿ�ʼ
					if(m_lbTemperature < m_lbSettingtemperature-2){
						// �¶ȵ�
						Beep (1000,1000);
					}else if(m_lbTemperature > m_lbSettingtemperature+2){
						// �¶ȸ�
						Beep (1000,1000);
					}else{ // �¶�����
					}
				}
				UpdateData(FALSE);  //�����յ����ַ���ʾ�ڽ��ܱ༭����
				break;
			case cmdSettingTemperature: // ��λ����ǰ���趨�¶�
				break;
			case cmdSettingLineNo: // ��λ����ǰ�κ�
				switch(v_portin[2]){
				case 0: // ����δ����
					m_curLineNo = 0;
					m_curLineTime = 0;
					send[1] = cmdSettingLineNo;
					send[2] = 1;
					send[3] = 0;
					m_SerialPort.WriteToPort(send,5); // ������λ����ǰ�κ�Ϊ 1
					send[1] = cmdRunningStatus;
					send[2] = 0;
					send[3] = 0;
					m_SerialPort.WriteToPort(send,5); // ������λ����ǰ����״̬Ϊ 0(����)
					SetTimer(1,600000,NULL);
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
	char send[5] = {0xfd,0x0,0x0,0x0,0xfe};
	switch(nIDEvent){
	case 0:
		send[1] = cmdGetTemperature;
		send[2] = 0;
		send[3] = 0;
		m_SerialPort.WriteToPort(send,5); // ֪ͨ��λ�����͵�ǰʵ���¶�
		break;
	case 1:
		m_curLineTime++;
		if(m_dryLine[m_curLineNo][1]!=0){ // ����
			short int tem = m_lbSettingtemperature;
			m_lbSettingtemperature += m_dryLine[m_curLineNo][1]/60.0;
			if((short int)m_lbSettingtemperature<m_dryLine[m_curLineNo][0]){
				if(tem!=(short int)m_lbSettingtemperature){
					send[1] = cmdSettingTemperature;
					*(WORD*)(send+2) = m_lbSettingtemperature;
					m_SerialPort.WriteToPort(send,5);
				}
			}else{
				m_curLineNo++;
				m_curLineTime = 0;
				send[1] = cmdSettingTemperature;
				*(WORD*)(send+2) = m_dryLine[m_curLineNo][0];
				m_SerialPort.WriteToPort(send,5);// ���͵�ǰ�趨�¶ȸ���λ��
			}
			UpdateData(FALSE);     //����༭�������
		}else if(m_curLineTime>m_dryLine[m_curLineNo][2]*60){ // ����
			m_curLineTime = 0;
			if(m_curLineNo<8){
				send[1] = cmdSettingTemperature;
				*(WORD*)(send+2) = m_dryLine[m_curLineNo++][0];
				m_SerialPort.WriteToPort(send,5); // ���͵�ǰ�趨�¶ȸ���λ��
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
		char send[5] = {0xfd,cmdSettingLineNo,0xff,0x0,0xfe};
		m_curLineNo = -1;
		SetTimer(0,10000,NULL);
		m_SerialPort.StartMonitoring();  //��������ͨ�ż���̺߳���
		send[1] = cmdGetTemperature;
		m_SerialPort.WriteToPort(send,5); // ȡ��λ����ǰʵ���¶�
		send[1] = cmdSettingLineNo;
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


void ChostDlg::OnCbnSelchangeComboVerifybit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
