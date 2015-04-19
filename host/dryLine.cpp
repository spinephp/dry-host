#include "stdafx.h"
#include "dryLine.h"


dryLine::dryLine(CPaintDC *mdc, CRect rect)
{
	m_nLeft = rect.left;
	m_nTop = rect.top;
	m_nWidth = rect.Width();
	m_nHeight = rect.Height();

	m_dcMem.CreateCompatibleDC(mdc);
	CBitmap tmp, tmpp;
	tmpp.CreateCompatibleBitmap(mdc, m_nWidth, m_nHeight);
	m_dcMem.SelectObject(&tmpp);
	tmpp.DeleteObject();

	m_dcMem.SetMapMode(MM_ANISOTROPIC);
	m_dcMem.SetViewportExt(m_nWidth, m_nHeight);
	m_dcMem.SetWindowExt(m_nWidth, -m_nHeight);
	m_dcMem.SetViewportOrg(0, 0);
	m_dcMem.SetWindowOrg(0, m_nHeight);

	m_redcolor = RGB(255, 0, 0);                      // 红色  
	m_bluecolor = RGB(0, 0, 255);                     // 蓝色  
	m_greencolor = RGB(0, 255, 0);                     // 绿色  
	m_textcolor = RGB(255, 255, 255);                 // 文本颜色设置为白色  
}


dryLine::~dryLine()
{
	m_dcMem.DeleteDC();
}

void dryLine::draw(CDC *hdc,CFile file,int nCurpos=0)
{
	m_dcMem.FillSolidRect(0, 0, m_nWidth, m_nHeight, RGB(255, 255, 255));
	if (file.m_hFile != CFile::hFileNull && hdc){
		int size = sizeof(WORD) * 4;
		ULONGLONG filesize = file.GetLength();
		ULONGLONG nSizes = (filesize - sizeof(dryHead)) / size; //m_ptrArray[0].GetSize();
		if (nSizes && nSizes > nCurpos + 1){
			WORD record[4];
			ULONGLONG lPos;
			TRY{
				HPEN hPen1 = CreatePen(PS_SOLID, 1, m_redcolor);
				HPEN hPen2 = CreatePen(PS_SOLID, 1, m_bluecolor);
				lPos = file.Seek(nCurpos * size + sizeof(dryHead), CFile::begin);
				file.Read(m_record, size);
				for (ULONGLONG i = 1; i < m_nWidth && i < nSizes - 1; i++){
					lPos = file.Seek(size, CFile::current);
					if (lPos < filesize){
						file.Read(record, size);
						m_dcMem.SelectObject(hPen1);
						m_dcMem.MoveTo(m_record[2] - nCurpos, m_record[0]);
						m_dcMem.LineTo(record[2] - nCurpos, record[0]);
						m_dcMem.SelectObject(hPen2);
						m_dcMem.MoveTo(m_record[2] - nCurpos, m_record[1]);
						m_dcMem.LineTo(record[2] - nCurpos, record[1]);
						memcpy(m_record, record, size);
					}
				}
				DeleteObject(hPen1);
				DeleteObject(hPen2);
			}
				CATCH(CFileException, e){
#ifdef _DEBUG
				afxDump << "文件不能打开！" << e->m_cause << "\n";
#endif
			}END_CATCH
		}
		hdc->BitBlt(m_nLeft, m_nTop, m_nWidth, m_nHeight, &m_dcMem, 0, 0, SRCCOPY);
	}
}

void dryLine::draw(CDC *hdc, CFile file, WORD *record, int nCurpos = 0)
{
	int size = sizeof(WORD) * 4;
	ULONGLONG filesize = file.GetLength();
	int nSize = (filesize - sizeof(dryHead)) / size;// m_ptrArray[0].GetSize();
	if (nSize > nCurpos + 1){
		HPEN hPen1 = CreatePen(PS_SOLID, 1, m_redcolor);
		HPEN hPen2 = CreatePen(PS_SOLID, 1, m_bluecolor);
		m_dcMem.SelectObject(hPen1);
		m_dcMem.MoveTo(m_record[2] - nCurpos, m_record[0]);
		m_dcMem.LineTo(record[2] - nCurpos, record[0]);
		m_dcMem.SelectObject(hPen2);
		m_dcMem.MoveTo(m_record[2] - nCurpos, m_record[1]);
		m_dcMem.LineTo(record[2] - nCurpos, record[1]);
		DeleteObject(hPen1);
		DeleteObject(hPen2);
	}
	if (hdc)
		hdc->BitBlt(m_nLeft, m_nTop, m_nWidth, m_nHeight, &m_dcMem, 0, 0, SRCCOPY);
	memcpy(m_record, record, size);
}

void dryLine::draw(CDC *hdc, vector< vector<int> > dryLines, WORD roomTemperature)
{
	m_dcMem.FillSolidRect(0, 0, m_nWidth, m_nHeight, RGB(255, 255, 255));
	int size = sizeof(WORD) * 4;
	UINT times = 0;
	UINT time;
	WORD preTemperature = roomTemperature;
	HPEN hPen2 = CreatePen(PS_SOLID, 1, m_bluecolor);
	CPoint pt0(0, roomTemperature + 0);
	m_dcMem.DPtoLP(&pt0);
	m_dcMem.SelectObject(hPen2);
	m_dcMem.MoveTo(pt0);
	for (int i = 0; i < dryLines.size(); i++){
		time = dryLines[i][2];
		if (time == 0){
			time = (dryLines[i][0] - preTemperature) / dryLines[i][1];
		}
		times += time*3;
		CPoint pt(times, dryLines[i][0] + 0);
		m_dcMem.DPtoLP(&pt);
		m_dcMem.LineTo(pt);
		preTemperature = dryLines[i][0];
	}
	time = (preTemperature - roomTemperature) / 10;
	times += time * 3;
	CPoint pt(times, roomTemperature + 0);
	m_dcMem.DPtoLP(&pt);
	m_dcMem.LineTo(pt);
	DeleteObject(hPen2);
	if (hdc)
		hdc->BitBlt(m_nLeft, m_nTop, m_nWidth, m_nHeight, &m_dcMem, 0, 0, SRCCOPY);
}
void dryLine::draw(CDC &hdc, CFile &file,UINT status)
{
	if (file.m_hFile != CFile::hFileNull && hdc){
		int size = sizeof(WORD) * 4;
		ULONGLONG filesize = file.GetLength();
		ULONGLONG nSizes = (filesize - sizeof(dryHead)) / size; //m_ptrArray[0].GetSize();
		if (nSizes){
			WORD record[4];
			ULONGLONG lPos;
			TRY{
				HPEN hPen1 = CreatePen(PS_SOLID, 1, m_redcolor);
				lPos = file.Seek(sizeof(dryHead), CFile::begin);
				lPos += file.Read(m_record, size);
				toLP(m_record,1.0/40,0.0625,0,0);
				for (ULONGLONG i = 1; i < m_nWidth && i < nSizes - 1; i++){
					if (lPos < filesize){
						lPos += file.Read(record, size);
						toLP(record, 1.0/40, 0.0625, 0, 0);
						if (record[2] == m_record[2]) continue;
						m_dcMem.SelectObject(hPen1);
						m_dcMem.MoveTo(m_record[2], m_record[1]);
						m_dcMem.LineTo(record[2], record[1]);
						memcpy(m_record, record, size);
					}
				}
				DeleteObject(hPen1);
			}
				CATCH(CFileException, e){
#ifdef _DEBUG
				afxDump << "文件不能打开！" << e->m_cause << "\n";
#endif
			}END_CATCH
		}
		hdc.BitBlt(m_nLeft, m_nTop, m_nWidth, m_nHeight, &m_dcMem, 0, 0, SRCCOPY);
	}
}

void dryLine::paint(CDC *hdc)
{
	if (hdc)
		hdc->BitBlt(m_nLeft, m_nTop, m_nWidth, m_nHeight, &m_dcMem, 0, 0, SRCCOPY);
}

WORD* dryLine::toLP(WORD * record, double sx = 1, double sy = 0.0625, int dx = 0, int dy = 50)
{
	CPoint pt(record[2]*sx+dx, record[0] * sy + dy);
	CPoint pt1(record[2]*sx+dx, record[1] * sy + dy);
	m_dcMem.DPtoLP(&pt);
	m_dcMem.DPtoLP(&pt1);

	record[0] = pt.y;
	record[1] = pt1.y;
	record[2] = pt.x;
	return record;
}
