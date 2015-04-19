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
	, m_edBreakFile(_T(""))
{

}

interruptDlg::~interruptDlg()
{
	delete dryline;
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
	DDX_Text(pDX, IDC_EDIT_BREAKFILE, m_edBreakFile);
}


BEGIN_MESSAGE_MAP(interruptDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &interruptDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_OPENFILE, &interruptDlg::OnBnClickedButtonOpenfile)
	ON_WM_PAINT()
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
	for(UINT i=0;i<m_lineNames.size();i++){
		CString str;
		m_cbLines.AddString(m_lineNames[i]);
	}
	m_cbLines.SetCurSel(m_curSelLine);

	m_roomTemperature = 20;

	CRect rect;
	CPaintDC dc(this);
	GetDlgItem(IDC_STATIC_PREPROCESS)->GetWindowRect(&rect);//获取控件相对于屏幕的位置
	ScreenToClient(rect);//转化为对话框上的相对位置
	rect.left = rect.left + 190;
	rect.top = rect.top + 12;
	rect.right = rect.right - 5;
	rect.bottom = rect.bottom - 5;

	dryline = new dryLine(&dc, rect);
	dryline->draw(&dc, m_dryLines, 20);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void interruptDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_curSelLine = m_cbLines.GetCurSel();
	CDialogEx::OnOK();
}

void interruptDlg::OnBnClickedButtonOpenfile()
{
	// TODO:  在此添加控件通知处理程序代码
	CFileDialog opendialog(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY,
		TEXT("干燥数据记录文件(*.dat)|*.dat|所有文件(*.*)|*.*||"), NULL);
	if (opendialog.DoModal() == IDOK)
	{
		m_edBreakFile = opendialog.GetPathName();
		UpdateData(FALSE);
		CFile file;
		int openState = file.Open(m_edBreakFile, CFile::typeBinary | CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
		if (openState){
			CDC *hdc = GetDC();
			dryline->draw(*hdc, file, (UINT)0);
			ReleaseDC(hdc);
		}
	}
}


void interruptDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	dryline->paint(&dc);
	// 不为绘图消息调用 CDialogEx::OnPaint()
}
