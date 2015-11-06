#pragma once
#include   <vector>   
using   namespace   std;

struct dryHead{
	char mark[3];
	char date[8];
	char time[6];
};

class dryLine
{
	int m_nLeft, m_nTop, m_nWidth, m_nHeight; // ²ðÏß»æÍ¼ÇøÓò
	int m_time;
	WORD m_record[4];
	WORD* toLP(WORD * record, double sx , double sy, int dx, int dy);
protected:
	CDC m_dcMem, m_dcMemTime, m_dcMemHG; //»º³åDCºÍ±³¾°DC
	COLORREF m_redcolor, m_bluecolor, m_greencolor, m_textcolor;
public:
	dryLine(CPaintDC *mdc, CRect rect);
	~dryLine();

	void setStartPoint(CDC *hdc,int time);

	void draw(CDC *hdc, CFile &file, int nCurpos);
	void draw(CDC *hdc, CFile &file, WORD *record, int nCurpos);
	void draw(CDC *hdc, vector< vector<int> > dryLines, WORD roomTemperature);
	void draw(CDC &hdc, CFile &file, UINT status);
	void paint(CDC *hdc);
};

