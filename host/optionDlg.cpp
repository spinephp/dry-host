// optionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "host.h"
#include "optionDlg.h"
#include "afxdialogex.h"


// optionDlg 对话框

IMPLEMENT_DYNAMIC(optionDlg, CDialogEx)

optionDlg::optionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(optionDlg::IDD, pParent)
	, m_cbLowPause(_T("是"))
	, m_cbLowPauseValue(2)
	, m_cbTemperatureFilterValue(0)
	, m_cbTemperatureFilter(_T("否"))
	, m_cbOverHeratingWarning(_T("是"))
	, m_cbOverHeratingValue(1)
	, m_cbUltraLimitAlarming(_T("是"))
	, m_cbUltraLimitAlarmingValue(2)
	, m_cbHandPause(_T("否"))
	, m_edLineTemperature(_T(""))
	, m_edLineHeatingRate(_T(""))
	, m_edLineTimeLength(_T(""))
	, m_ltLine(_T(""))
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
	DDX_CBIndex(pDX, IDC_COMBO_LOWPAUSEVALUE,m_cbLowPauseValue);
	DDV_MinMaxInt(pDX, m_cbLowPauseValue,1,5);
	DDX_CBIndex(pDX, IDC_COMBO_TEMPERATUREFILTERVALUE,m_cbTemperatureFilterValue);
	DDV_MinMaxInt(pDX, m_cbTemperatureFilterValue, 1, 5);
	DDX_CBString(pDX, IDC_COMBO_TEMPERATUREFILTER, m_cbTemperatureFilter);
	DDX_CBString(pDX, IDC_COMBO_OVERHERATINGWarning, m_cbOverHeratingWarning);
	DDX_CBIndex(pDX, IDC_COMBO_OVERHEATINGWARNINGVALUE,m_cbOverHeratingValue);
	DDV_MinMaxInt(pDX, m_cbOverHeratingValue,2,4);
	DDX_CBString(pDX, IDC_COMBO_ULTRALIMITALARMING, m_cbUltraLimitAlarming);
	DDX_CBIndex(pDX, IDC_COMBO_ULTRALIMITALARMVALUE, m_cbUltraLimitAlarmingValue);
	DDV_MinMaxInt(pDX, m_cbUltraLimitAlarmingValue, 3, 5);
	DDX_CBString(pDX, IDC_COMBO_HANDPAUSE, m_cbHandPause);
	DDX_Text(pDX, IDC_EDIT_ENDTEMPERATURE, m_edLineTemperature);
	DDX_Text(pDX, IDC_EDIT_HEATINGRATE, m_edLineHeatingRate);
	DDX_Text(pDX, IDC_EDIT_TIMELENGTH, m_edLineTimeLength);
	DDX_LBString(pDX, IDC_LIST_LINE, m_ltLine);
	DDX_Control(pDX, IDC_EDIT_ENDTEMPERATURE, m_edtLineTemperature);
}


BEGIN_MESSAGE_MAP(optionDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_HANDPAUSE, &optionDlg::OnCbnSelchangeComboHandpause)
	ON_BN_CLICKED(ID_ADD, &optionDlg::OnBnClickedAdd)
	ON_EN_CHANGE(IDC_EDIT_ENDTEMPERATURE, &optionDlg::OnChangeEditLine)
	ON_EN_CHANGE(IDC_EDIT_HEATINGRATE, &optionDlg::OnChangeEditLine)
	ON_EN_CHANGE(IDC_EDIT_TIMELENGTH, &optionDlg::OnChangeEditLine)
END_MESSAGE_MAP()


// optionDlg 消息处理程序


void optionDlg::OnCbnSelchangeComboHandpause()
{
	// TODO: 在此添加控件通知处理程序代码
}


void optionDlg::OnBnClickedAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	m_edLineTemperature = _T("");
	m_edLineHeatingRate = _T("");
	m_edLineTimeLength = _T("");
	UpdateData(FALSE); 
	GetDlgItem(IDC_EDIT_ENDTEMPERATURE)->SetFocus();
	GetDlgItem(ID_SAVE)->EnableWindow(FALSE);
}


void optionDlg::OnChangeEditLine()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(); 
	GetDlgItem(ID_SAVE)->EnableWindow(!m_edLineTemperature.IsEmpty()  && !m_edLineHeatingRate.IsEmpty() && !m_edLineTimeLength.IsEmpty());
}

