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
	, m_rdAutoRun(4)
	, m_edAllTime(_T(""))
	, m_edBreakFile(_T(""))
{

}

interruptDlg::~interruptDlg()
{
	//delete dryline;
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
	ON_BN_CLICKED(IDC_RADIO_AUTORUN, &interruptDlg::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO2, &interruptDlg::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO3, &interruptDlg::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO4, &interruptDlg::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO5, &interruptDlg::OnBnClickedRadio5)
	ON_CBN_SELCHANGE(IDC_COMBO_LINENAME, &interruptDlg::OnBnClickedRadio5)
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
	for(UINT i=0;i<m_lineNames.size();i++)
		m_cbLines.AddString(m_lineNames[i]);
	
	m_cbLines.SetCurSel(m_curSelLine);

	bool fileNoOpened = m_file->m_hFile == CFile::hFileNull;
	GetDlgItem(IDC_BUTTON_OPENFILE)->EnableWindow(fileNoOpened);
	//if (fileNoOpened)
		//OnBnClickedButtonOpenfile();

	CPaintDC dc(this);
	CRect rect;
	GetDlgItem(IDC_STATIC_PREPROCESS)->GetWindowRect(&rect);//获取控件相对于屏幕的位置
	ScreenToClient(rect);//转化为对话框上的相对位置
	rect.left = rect.left + 190;
	rect.top = rect.top + 12;
	rect.right = rect.right - 5;
	rect.bottom = rect.bottom - 5;
	dryline = new dryLine(&dc, rect);
	//dryline->paint(&dc);
	dryline->draw(&dc, m_dryLines, 20);
	dryline->setStartPoint(&dc, 0);
	//if (m_file->m_hFile != CFile::hFileNull && m_file->GetLength())
		//dryline->draw(&dc, *m_file, (UINT)0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void interruptDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_file->m_hFile == CFile::hFileNull){
		OnBnClickedButtonOpenfile();
		return;
	}
	m_curSelLine = m_cbLines.GetCurSel();
	CDialogEx::OnOK();
}

void interruptDlg::setFile(CFile *file){
	m_file = file;
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
		int openState = m_file->Open(m_edBreakFile, CFile::typeBinary | CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
		if (openState){
			CDC *hdc = GetDC();
			dryline->draw(*hdc, *m_file, (UINT)0);
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

void interruptDlg::drawDryLine()
{
	CPaintDC dc(this);
	CRect rect;
	GetDlgItem(IDC_STATIC_PREPROCESS)->GetWindowRect(&rect);//获取控件相对于屏幕的位置
	ScreenToClient(rect);//转化为对话框上的相对位置
	rect.left = rect.left + 190;
	rect.top = rect.top + 12;
	rect.right = rect.right - 5;
	rect.bottom = rect.bottom - 5;
	dryline = new dryLine(&dc, rect);
	//dryline->paint(&dc);
	dryline->draw(&dc, m_dryLines, 20);
	dryline->setStartPoint(&dc, 0);
	if (m_file->m_hFile != CFile::hFileNull && m_file->GetLength())
		dryline->draw(&dc, *m_file, (UINT)0);
	delete dryline;
}

void interruptDlg::OnBnClickedRadio5()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int time;
	int lineNo = m_cbLines.GetCurSel();
	switch (m_rdAutoRun){
	case 0:
		break;
	case 1:
		time = lineNoToTime(lineNo, 20);
		break;
	case 2:
		time = lineNoToTime(lineNo, 20);
		time += _ttoi(m_edLineTime);
		break;
	case 3:
		time = lineNoToTime(lineNo, 20);
		break;
	case 4:
		time = 0;
		break;
	}
	CDC *hdc = GetDC();
	dryline->setStartPoint(hdc, time);
	ReleaseDC(hdc);

}


int interruptDlg::lineNoToTime(int lineNo,int roomTemperature=20)
{
	int times = 0;
	int preTemperature = roomTemperature;
	for (int i = 0; i < lineNo && i<m_dryLines.size(); i++){
		int time = m_dryLines[i][2];
		if (time == 0){
			time = (m_dryLines[i][0] - preTemperature) / m_dryLines[i][1];
		}
		times += time;
		preTemperature = m_dryLines[i][0];
	}
	return times;
}


BOOL interruptDlg::DestroyWindow()
{
	// TODO:  在此添加专用代码和/或调用基类
	delete dryline;
	return CDialogEx::DestroyWindow();
}
