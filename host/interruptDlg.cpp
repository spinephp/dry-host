// interruptDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "host.h"
#include "interruptDlg.h"
#include "afxdialogex.h"


// interruptDlg 对话框

IMPLEMENT_DYNAMIC(interruptDlg, CDialogEx)

interruptDlg::interruptDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(interruptDlg::IDD, pParent)
	, m_edLineName(_T(""))
	, m_edLineTime(_T(""))
	, m_edTemperature(0)
	, m_edSetingTemperature(0)
	, m_rdAutoRun(0)
	, m_edAllTime(_T(""))
{

}

interruptDlg::~interruptDlg()
{
}

void interruptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_EDIT_CURRENTLINE, m_edLineName);
	DDX_Text(pDX, IDC_EDIT_LINETIME, m_edLineTime);
	DDX_Text(pDX, IDC_EDIT_TEMPERATURE, m_edTemperature);
	DDX_Text(pDX, IDC_EDIT_SETTINGTEMPERATURE, m_edSetingTemperature);
	DDX_Radio(pDX, IDC_RADIO_AUTORUN, m_rdAutoRun);
	DDX_Control(pDX, IDC_COMBO_LINENAME, m_cbLines);
	DDX_Text(pDX, IDC_EDIT_ALLTIME, m_edAllTime);
}


BEGIN_MESSAGE_MAP(interruptDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &interruptDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// interruptDlg 消息处理程序


void interruptDlg::setSettingTemperature(float temperature)
{
	m_edSetingTemperature = temperature;
	UpdateData();
}


BOOL interruptDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	for(int i=0;i<m_lineNames.size();i++){
		CString str;
		m_cbLines.AddString(m_lineNames[i]);
	}
	m_cbLines.SetCurSel(m_curSelLine);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void interruptDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_curSelLine = m_cbLines.GetCurSel();
	CDialogEx::OnOK();
}
