// interruptDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "host.h"
#include "interruptDlg.h"
#include "afxdialogex.h"


// interruptDlg �Ի���

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


// interruptDlg ��Ϣ�������


void interruptDlg::setSettingTemperature(float temperature)
{
	m_edSetingTemperature = temperature;
	UpdateData();
}


BOOL interruptDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	for(UINT i=0;i<m_lineNames.size();i++){
		CString str;
		m_cbLines.AddString(m_lineNames[i]);
	}
	m_cbLines.SetCurSel(m_curSelLine);

	m_roomTemperature = 20;

	CRect rect;
	CPaintDC dc(this);
	GetDlgItem(IDC_STATIC_PREPROCESS)->GetWindowRect(&rect);//��ȡ�ؼ��������Ļ��λ��
	ScreenToClient(rect);//ת��Ϊ�Ի����ϵ����λ��
	rect.left = rect.left + 190;
	rect.top = rect.top + 12;
	rect.right = rect.right - 5;
	rect.bottom = rect.bottom - 5;

	dryline = new dryLine(&dc, rect);
	dryline->draw(&dc, m_dryLines, 20);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void interruptDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_curSelLine = m_cbLines.GetCurSel();
	CDialogEx::OnOK();
}

void interruptDlg::OnBnClickedButtonOpenfile()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CFileDialog opendialog(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY,
		TEXT("�������ݼ�¼�ļ�(*.dat)|*.dat|�����ļ�(*.*)|*.*||"), NULL);
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
	// TODO:  �ڴ˴������Ϣ����������
	dryline->paint(&dc);
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}
