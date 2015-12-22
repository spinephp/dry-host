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

	m_time = -1;
}


dryLine::~dryLine()
{
	m_dcMem.DeleteDC();
}

void dryLine::draw(CDC *hdc, recordFile &file, int nCurpos = 0)
{
	m_dcMem.FillSolidRect(0, 0, m_nWidth, m_nHeight, RGB(255, 255, 255));
	ULONGLONG nSizes = file.recordCount();
	if (nSizes && hdc){
		if (nSizes && nSizes > nCurpos + 1){
			short int record[4];
			ULONGLONG lPos;
			TRY{
				HPEN hPen1 = CreatePen(PS_SOLID, 1, m_redcolor);
				HPEN hPen2 = CreatePen(PS_SOLID, 1, m_bluecolor);
				lPos = file.seekRecord(nCurpos, CFile::begin);
				file.readRecord(m_record, 1);
				for (ULONGLONG i = 1; i < m_nWidth && i < nSizes - 1; i++){
					lPos = file.seekRecord(1, CFile::current);
					//if (lPos < filesize){
						file.readRecord(record, 1);
						HPEN oldPen = (HPEN)m_dcMem.SelectObject(hPen1);
						m_dcMem.MoveTo(m_record[2] - nCurpos, m_record[0]);
						m_dcMem.LineTo(record[2] - nCurpos, record[0]);
						m_dcMem.SelectObject(hPen2);
						m_dcMem.MoveTo(m_record[2] - nCurpos, m_record[1]);
						m_dcMem.LineTo(record[2] - nCurpos, record[1]);
						memcpy(m_record, record, file.getRecordSize());
						m_dcMem.SelectObject(oldPen);
					//}
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

void dryLine::draw(CDC *hdc, recordFile &file, WORD *record, int nCurpos = 0)
{
	ULONGLONG nSize = file.recordCount();
	if (nSize > nCurpos + 1){
		HPEN hPen1 = CreatePen(PS_SOLID, 1, m_redcolor);
		HPEN hPen2 = CreatePen(PS_SOLID, 1, m_bluecolor);
		HPEN oldPen = (HPEN)m_dcMem.SelectObject(hPen1);
		m_dcMem.MoveTo(m_record[2] - nCurpos, m_record[0]);
		m_dcMem.LineTo(record[2] - nCurpos, record[0]);
		m_dcMem.SelectObject(hPen2);
		m_dcMem.MoveTo(m_record[2] - nCurpos, m_record[1]);
		m_dcMem.LineTo(record[2] - nCurpos, record[1]);
		m_dcMem.SelectObject(oldPen);
		DeleteObject(hPen1);
		DeleteObject(hPen2);
	}
	if (hdc)
		hdc->BitBlt(m_nLeft, m_nTop, m_nWidth, m_nHeight, &m_dcMem, 0, 0, SRCCOPY);
	memcpy(m_record, record, file.getRecordSize());
}

void dryLine::draw(CDC *hdc, vector< vector<int> > dryLines, WORD roomTemperature)
{
	m_dcMem.FillSolidRect(0, 0, m_nWidth, m_nHeight, RGB(255, 255, 255));
	int size = sizeof(WORD) * 4;
	UINT times = 0;
	UINT time;
	WORD preTemperature = roomTemperature;
	HPEN hPen2 = CreatePen(PS_SOLID, 1, m_bluecolor);
	CPoint pt0(10, roomTemperature + 0);
	m_dcMem.DPtoLP(&pt0);
	HPEN oldPen = (HPEN)m_dcMem.SelectObject(hPen2);
	m_dcMem.MoveTo(pt0);
	for (int i = 0; i < dryLines.size(); i++){
		time = dryLines[i][2];
		if (time == 0){
			time = (dryLines[i][0] - preTemperature) / dryLines[i][1];
		}
		times += time*3;
		CPoint pt(times+10, dryLines[i][0] + 0);
		m_dcMem.DPtoLP(&pt);
		m_dcMem.LineTo(pt);
		preTemperature = dryLines[i][0];
	}
	time = (preTemperature - roomTemperature) / 10;
	times += time * 3;
	CPoint pt(times+10, roomTemperature + 0);
	m_dcMem.DPtoLP(&pt);
	m_dcMem.LineTo(pt);
	m_dcMem.SelectObject(oldPen);
	DeleteObject(hPen2);
	if (hdc)
		hdc->BitBlt(m_nLeft, m_nTop, m_nWidth, m_nHeight, &m_dcMem, 0, 0, SRCCOPY);
}
void dryLine::draw(CDC &hdc, recordFile &file, UINT status)
{
	ULONGLONG nSizes = file.recordCount();
	if (nSizes && hdc){
			short int record[4];
			ULONGLONG lPos;
			TRY{
				HPEN hPen1 = CreatePen(PS_SOLID, 1, m_redcolor);
				HPEN oldPen = (HPEN)m_dcMem.SelectObject(hPen1);
				lPos = file.seekRecord(0, CFile::begin);
				lPos += file.readRecord(m_record, 1);
				toLP(m_record,1.0/40,0.0625,10,0);
				for (ULONGLONG i = 1; i < m_nWidth && i < nSizes - 1; i++){
					//if (lPos < filesize){
						lPos += file.readRecord(record, 1);
						toLP(record, 1.0/40, 0.0625, 10, 0);
						if (record[2] == m_record[2]) continue;
						m_dcMem.MoveTo(m_record[2], m_record[1]);
						m_dcMem.LineTo(record[2], record[1]);
						memcpy(m_record, record, file.getRecordSize());
					//}
				}
				m_dcMem.SelectObject(oldPen);
				DeleteObject(hPen1);
			}
				CATCH(CFileException, e){
#ifdef _DEBUG
				afxDump << "文件不能打开！" << e->m_cause << "\n";
#endif
			}END_CATCH
		hdc.BitBlt(m_nLeft, m_nTop, m_nWidth, m_nHeight, &m_dcMem, 0, 0, SRCCOPY);
	}
}

void dryLine::paint(CDC *hdc)
{
	if (hdc)
		hdc->BitBlt(m_nLeft, m_nTop, m_nWidth, m_nHeight, &m_dcMem, 0, 0, SRCCOPY);
}

short int* dryLine::toLP(short int * record, double sx = 1, double sy = 0.0625, int dx = 0, int dy = 50)
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

void dryLine::setStartPoint(CDC *hdc,int time = 0)
{
	if (time != m_time){
		short int record[4] = { 0, m_nHeight, m_time*3, 0 };
		HPEN hPen = CreatePen(PS_SOLID, 1, m_greencolor);
		HPEN oldPen = (HPEN)m_dcMem.SelectObject(hPen);
		int nOldMode = m_dcMem.SetROP2(R2_NOTXORPEN);
		if (m_time != -1){
			toLP(record, 1, 1, 10, 0);
			m_dcMem.MoveTo(record[2], record[0]);
			m_dcMem.LineTo(record[2], record[1]);
		}
		record[0] = 0;
		record[1] = m_nHeight;
		record[2] = time*3;
		toLP(record, 1, 1, 10, 0);
		m_dcMem.MoveTo(record[2],record[0]);
		m_dcMem.LineTo(record[2], record[1]);
		m_dcMem.SetROP2(nOldMode);
		m_dcMem.SelectObject(oldPen);
		DeleteObject(hPen);
		m_time = time;
		paint(hdc);
	}
}