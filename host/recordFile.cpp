#include "stdafx.h"
#include "recordFile.h"


recordFile::recordFile(LPCTSTR filename,UINT nOpenFlags)
{
	openState = m_file.Open(filename, nOpenFlags);
	int len = m_file.GetLength();
	if (m_file.GetLength() > 16){
		TCHAR s[18];
		m_file.Read(s, 17*sizeof(TCHAR));
		s[17] = '\0';
		header = s;
	}
	recordSize = sizeof(WORD) * 4;
}

recordFile::~recordFile()
{
	if (openState)
		m_file.Close();
}

BOOL recordFile::getOpenStatus(void)
{
	return openState;
}

int recordFile::getHeaderSize(void)
{
	return header.GetLength()*sizeof(TCHAR);
}

int recordFile::getRecordSize(void)
{
	return recordSize;
}

ULONGLONG recordFile::recordCount(void)
{
	ULONGLONG size = 0;
	if (this->openState){
		size = (m_file.GetLength() - getHeaderSize()) / recordSize;
		if (size < 0)
			size = 0;
	}
	return size;
}

ULONGLONG recordFile::seekRecord(LONGLONG n, UINT nForm)
{
	UINT headsize = (nForm == CFile::begin) ? getHeaderSize() : 0;
	return m_file.Seek(headsize+n*recordSize, nForm);
}

UINT recordFile::readRecord(short int *record, ULONGLONG n)
{
	return m_file.Read(record, n*recordSize);
}

void recordFile::writeHeader(CString header)
{
	this->header = header;
	m_file.Write(header, wcslen(header)*sizeof(wchar_t));
}

void recordFile::appendRecord(short int* record)
{
	m_file.SeekToEnd();
	m_file.Write(record, recordSize);
	m_file.Flush();
}

CString recordFile::backup(void)
{
	CString t_filename = NULL;
	if (m_file.m_hFile != CFile::hFileNull && m_file.GetLength()){
		CString m_filename = m_file.GetFileName();
		CFileStatus status;
		t_filename = m_filename;
		int index = 0;
		while (CFile::GetStatus(t_filename, status)){
			t_filename.Format(L"%s%d", m_filename, index++);
		}
	}
	return t_filename;
}


CString recordFile::GetFileName(void)
{
	return m_file.GetFileName();
}