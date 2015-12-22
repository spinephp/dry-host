#pragma once

class recordFile
{
	CFile m_file;
	int recordSize;
	BOOL openState;
	CString header;
public:
	recordFile(LPCTSTR filename, UINT nOpenFlags);
	virtual ~recordFile();
	BOOL getOpenStatus(void);
	int getHeaderSize(void);
	int getRecordSize(void);
	ULONGLONG recordCount(void);
	ULONGLONG seekRecord(LONGLONG n, UINT nForm);
	UINT readRecord(short int *record, ULONGLONG n);
	void writeHeader(CString header);
	void appendRecord(short int* record);
	CString backup(void);
	CString GetFileName(void);
};

