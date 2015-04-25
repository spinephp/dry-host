// optionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "host.h"
#include "optionDlg.h"
#include "afxdialogex.h"

// optionDlg �Ի���

IMPLEMENT_DYNAMIC(optionDlg, CDialogEx)

optionDlg::optionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(optionDlg::IDD, pParent)
	, m_cbLowPause(_T("��"))
	, m_cbLowPauseValue(2)
	, m_cbTemperatureFilterValue(0)
	, m_cbTemperatureFilter(_T("��"))
	, m_cbOverHeratingWarning(_T("��"))
	, m_cbOverHeratingValue(1)
	, m_cbUltraLimitAlarming(_T("��"))
	, m_cbUltraLimitAlarmingValue(2)
	, m_cbHandPause(_T("��"))
	, m_edLineTemperature(_T(""))
	, m_edLineHeatingRate(_T(""))
	, m_edLineTimeLength(_T(""))
	, m_ltLine(_T(""))
	, m_edTemperatureUpTime(0)
	, m_edSetTemperatureDownTime(0)
	, m_rdStartMode(0)
{
	m_cbLowPauseValue = 3;
	m_cbTemperatureFilterValue = 1;
	m_cbOverHeratingValue = 2;
	m_cbUltraLimitAlarmingValue = 3;
}

optionDlg::~optionDlg()
{
}

void optionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO_LOWPAUSE, m_cbLowPause);
	DDX_CBIndex(pDX, IDC_COMBO_LOWPAUSEVALUE, m_cbLowPauseValue);
	DDV_MinMaxInt(pDX, m_cbLowPauseValue, 1, 5);
	DDX_CBIndex(pDX, IDC_COMBO_TEMPERATUREFILTERVALUE, m_cbTemperatureFilterValue);
	DDV_MinMaxInt(pDX, m_cbTemperatureFilterValue, 1, 5);
	DDX_CBString(pDX, IDC_COMBO_TEMPERATUREFILTER, m_cbTemperatureFilter);
	DDX_CBString(pDX, IDC_COMBO_OVERHERATINGWarning, m_cbOverHeratingWarning);
	DDX_CBIndex(pDX, IDC_COMBO_OVERHEATINGWARNINGVALUE, m_cbOverHeratingValue);
	DDV_MinMaxInt(pDX, m_cbOverHeratingValue, 2, 4);
	DDX_CBString(pDX, IDC_COMBO_ULTRALIMITALARMING, m_cbUltraLimitAlarming);
	DDX_CBIndex(pDX, IDC_COMBO_ULTRALIMITALARMVALUE, m_cbUltraLimitAlarmingValue);
	DDV_MinMaxInt(pDX, m_cbUltraLimitAlarmingValue, 3, 5);
	DDX_CBString(pDX, IDC_COMBO_HANDPAUSE, m_cbHandPause);
	DDX_Text(pDX, IDC_EDIT_ENDTEMPERATURE, m_edLineTemperature);
	DDX_Text(pDX, IDC_EDIT_HEATINGRATE, m_edLineHeatingRate);
	DDX_Text(pDX, IDC_EDIT_TIMELENGTH, m_edLineTimeLength);
	DDX_LBString(pDX, IDC_LIST_LINE, m_ltLine);
	DDX_Control(pDX, IDC_EDIT_ENDTEMPERATURE, m_edtLineTemperature);
	DDX_Control(pDX, IDC_LIST_LINE, m_lstLine);
	DDX_Text(pDX, IDC_EDIT_ENDTEMPERATURE2, m_edTemperatureUpTime);
	DDV_MinMaxInt(pDX, m_edTemperatureUpTime, 1, 60);
	DDX_Text(pDX, IDC_EDIT_ENDTEMPERATURE3, m_edSetTemperatureDownTime);
	DDV_MinMaxInt(pDX, m_edSetTemperatureDownTime, 1, 3600);
	DDX_Radio(pDX, IDC_RADIO_STARTANYTIME, m_rdStartMode);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dtcDryStart);
}


BEGIN_MESSAGE_MAP(optionDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_HANDPAUSE, &optionDlg::OnCbnSelchangeComboHandpause)
	ON_BN_CLICKED(ID_ADD, &optionDlg::OnBnClickedAdd)
	ON_EN_CHANGE(IDC_EDIT_ENDTEMPERATURE, &optionDlg::OnChangeEditLine)
	ON_EN_CHANGE(IDC_EDIT_HEATINGRATE, &optionDlg::OnChangeEditLine)
	ON_EN_CHANGE(IDC_EDIT_TIMELENGTH, &optionDlg::OnChangeEditLine)
	ON_BN_CLICKED(ID_SAVE, &optionDlg::OnBnClickedSave)
	ON_LBN_SELCHANGE(IDC_LIST_LINE, &optionDlg::OnSelchangeListLine)
	//ON_BN_CLICKED(ID_SAVE2, &optionDlg::OnBnClickedSave2)
	ON_BN_CLICKED(ID_EDIT, &optionDlg::OnBnClickedEdit)
	ON_BN_CLICKED(ID_DELETE, &optionDlg::OnBnClickedDelete)
	ON_BN_CLICKED(ID_UP, &optionDlg::OnBnClickedUp)
	ON_BN_CLICKED(ID_DOWN, &optionDlg::OnBnClickedDown)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDOK, &optionDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_STARTANYTIME, &optionDlg::OnBnClickedRadioStartanytime)
	ON_BN_CLICKED(IDC_RADIO_STARTSETINGTIME, &optionDlg::OnBnClickedRadioStartanytime)
END_MESSAGE_MAP()


// optionDlg ��Ϣ�������


void optionDlg::OnCbnSelchangeComboHandpause()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void optionDlg::OnBnClickedAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_LineEditMode = -1;
	m_edLineTemperature = _T("");
	m_edLineHeatingRate = _T("");
	m_edLineTimeLength = _T("");
	UpdateData(FALSE); 
	GetDlgItem(ID_SAVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_ENDTEMPERATURE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_HEATINGRATE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_TIMELENGTH)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_ENDTEMPERATURE)->SetFocus();
}


void optionDlg::OnChangeEditLine()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(); 
	GetDlgItem(ID_SAVE)->EnableWindow(!m_edLineTemperature.IsEmpty()  && !m_edLineHeatingRate.IsEmpty() && !m_edLineTimeLength.IsEmpty());
}



void optionDlg::OnBnClickedSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(); 
	int n;
	TCHAR *fmt = L"%%%ds%%%ds%%%ds";
	TCHAR fmt1[13];
	TCHAR record[60];
	swprintf(fmt1,13,fmt,13-m_edLineTemperature.GetLength() ,21-m_edLineHeatingRate.GetLength() ,19-m_edLineTimeLength.GetLength());
	swprintf(record,60,fmt1,m_edLineTemperature,m_edLineHeatingRate,m_edLineTimeLength);
	GetDlgItem(IDC_LIST_LINE)->EnableWindow(TRUE);
	if(m_LineEditMode>=0){
		m_lstLine.DeleteString(m_LineEditMode);
		n = m_lstLine.InsertString(m_LineEditMode,record);
	}else
		n = m_lstLine.AddString(record);
	//n = m_lstLine.AddString((LPCTSTR)record);
	m_lstLine.SelectString(n, record);
	GetDlgItem(IDC_EDIT_ENDTEMPERATURE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_HEATINGRATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_TIMELENGTH)->EnableWindow(FALSE);
	m_edLineTemperature = _T("");
	m_edLineHeatingRate = _T("");
	m_edLineTimeLength = _T("");
	UpdateData(FALSE); 
	GetDlgItem(ID_SAVE)->EnableWindow(FALSE);
	OnSelchangeListLine();
}

// ���� listbox �У��κ�ѡ��ı��¼�
void optionDlg::OnSelchangeListLine()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int i = m_lstLine.GetCurSel();
	BOOL c = m_lstLine.GetSelCount();
	GetDlgItem(ID_EDIT)->EnableWindow(c);
	GetDlgItem(ID_DELETE)->EnableWindow(c);
	GetDlgItem(ID_UP)->EnableWindow(c && i>0);
	GetDlgItem(ID_DOWN)->EnableWindow(c && i<m_lstLine.GetCount()-1);
}


void optionDlg::OnBnClickedSave2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void optionDlg::OnBnClickedEdit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString s;
	vector<int> v;
	int flag=0,k=0;
	m_LineEditMode = m_lstLine.GetCurSel();
	m_lstLine.GetText(m_LineEditMode,s);
	strToArray(s,v);
	m_edLineTemperature.Format(_T("%d"),v[0]);
	m_edLineHeatingRate.Format(_T("%d"), v[1]);
	m_edLineTimeLength.Format(_T("%d"), v[2]);

	UpdateData(FALSE); 
	GetDlgItem(ID_SAVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_ENDTEMPERATURE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_HEATINGRATE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_TIMELENGTH)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_ENDTEMPERATURE)->SetFocus();
}


void optionDlg::OnBnClickedDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UINT i = m_lstLine.GetCurSel();
	if(i>=0){
		m_lstLine.DeleteString(i);
		OnSelchangeListLine();
	}
}


void optionDlg::OnBnClickedUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UINT i = m_lstLine.GetCurSel();
	if(i>0){
		CString s0,s1;
		m_lstLine.GetText(i-1,s0);
		m_lstLine.GetText(i,s1);
		m_lstLine.DeleteString(i);
		m_lstLine.DeleteString(i-1);
		m_lstLine.InsertString(i-1,s0);
		m_lstLine.InsertString(i-1,s1);
		m_lstLine.SelectString(i-1,s1);
		OnSelchangeListLine();

	}
}


void optionDlg::OnBnClickedDown()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UINT i = m_lstLine.GetCurSel();
	if(i<m_lstLine.GetCount()-1){
		CString s0,s1;
		m_lstLine.GetText(i+1,s0);
		m_lstLine.GetText(i,s1);
		m_lstLine.DeleteString(i+1);
		m_lstLine.DeleteString(i);
		m_lstLine.InsertString(i,s1);
		m_lstLine.InsertString(i,s0);
		m_lstLine.SelectString(i,s1);
		OnSelchangeListLine();

	}
}


int optionDlg::loadXML(void)
{
	wchar_t *fmt = L"%%%dd%%%dd%%%dd";
	int nLen = m_dryLines.size();
	for (int i = 0; i < nLen; ++i) //�����ӽڵ� 
	{ 
		wchar_t fmt1[20]={0};
		wchar_t record[60]={0};
		swprintf(fmt1,20,fmt,13-digits(m_dryLines[i][0]) ,21-digits(m_dryLines[i][1]) ,19-digits(m_dryLines[i][2]));
		swprintf(record, 60,fmt1, m_dryLines[i][0], m_dryLines[i][1], m_dryLines[i][2]);
		m_lstLine.AddString(record);
	} 
	return 0;
}


BOOL optionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	loadXML();
	m_dtcDryStart.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	if (!m_noDrring){
		GetDlgItem(IDC_LIST_LINE)->EnableWindow(FALSE);
		GetDlgItem(ID_ADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_UPTEMPERATURETIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DOWNTEMPERATURETIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_STARTANYTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_STARTSETINGTIME)->EnableWindow(FALSE);

	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


int optionDlg::digits(int n)
{
	int digit=1;
    while(n/=10)++digit;
	return digit;
}


void optionDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_dtcDryStart.GetTime(m_startDryTime);
	if (m_rdStartMode == 1){
		if (CTime::GetCurrentTime() >= m_startDryTime){
			MessageBox(L"��ʱʱ�����ô������������á�", L"����", MB_ICONEXCLAMATION | MB_OK);
			return;
		}
	}
	int nLen = m_lstLine.GetCount();
	m_dryLines.clear();
	for(int i=0;i<nLen;i++){
		CString str;
		vector<int> line;
		m_lstLine.GetText(i,str);
		strToArray(str,line);
		m_dryLines.push_back(line);
	}
	UpdateData(); 
	CDialogEx::OnOK();
}


void optionDlg::strToArray(CString str, vector<int>& intArray)
{
	char v[3][4]={'\0'};
	int flag=0,k=0;
	for(int i=0,j=0;i<str.GetLength();i++){
		if(str[i]==' '){
			if(flag==1){
				j++;
				k = 0;
			}
			flag = 0;
		}else{
			flag = 1;
			v[j][k++] = str[i];
		}
	}
	intArray.push_back(atoi(v[0]));
	intArray.push_back(atoi(v[1]));
	intArray.push_back(atoi(v[2]));
}


void optionDlg::OnBnClickedRadioStartanytime()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_dtcDryStart.EnableWindow(m_rdStartMode);
}
