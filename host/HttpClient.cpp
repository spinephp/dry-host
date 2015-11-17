#include "StdAfx.h"
#include "HttpClient.h"
//#include "yazuoLog.h"
#define  BUFFER_SIZE       1024
#define  NORMAL_CONNECT             INTERNET_FLAG_KEEP_CONNECTION
#define  SECURE_CONNECT                NORMAL_CONNECT | INTERNET_FLAG_SECURE
#define  NORMAL_REQUEST             INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE
#define  SECURE_REQUEST             NORMAL_REQUEST | INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID
CHttpClient::CHttpClient(LPCTSTR strAgent)
{
		m_pSession = new CInternetSession(strAgent);
		m_pConnection = NULL;
		m_pFile = NULL;
}
CHttpClient::~CHttpClient(void)
{
		Clear();
		if (NULL != m_pSession)
		{
				m_pSession->Close();
				delete m_pSession;
				m_pSession = NULL;
		}
}
void CHttpClient::Clear()
{
		if (NULL != m_pFile)
		{
				m_pFile->Close();
				delete m_pFile;
				m_pFile = NULL;
		}
		if (NULL != m_pConnection)
		{
				m_pConnection->Close();
				delete m_pConnection;
				m_pConnection = NULL;
		}
}
int CHttpClient::ExecuteRequest(LPCTSTR strMethod, LPCTSTR strUrl, LPCTSTR strPostData, CString &strResponse)
{
		CString strServer;
		CString strObject;
		CString strMethodOverride;
		DWORD dwServiceType;
		INTERNET_PORT nPort;
		strResponse = "";

		if (strMethod == L"PUT" || strMethod == L"DELETE"){
			strMethodOverride.Format(L"X-HTTP-Method-Override:%s",strMethod);
			strMethod = L"POST";
		}

		AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort);
		if (AFX_INET_SERVICE_HTTP != dwServiceType && AFX_INET_SERVICE_HTTPS != dwServiceType)
		{
				return FAILURE;
		}
		try
	{
			m_pConnection = m_pSession->GetHttpConnection(strServer,
			dwServiceType == AFX_INET_SERVICE_HTTP ? NORMAL_CONNECT : SECURE_CONNECT,
			nPort);
			m_pFile = m_pConnection->OpenRequest(strMethod, strObject,
				NULL, 1, NULL, TEXT("HTTP/1.1"),
				 (dwServiceType == AFX_INET_SERVICE_HTTP ? NORMAL_REQUEST : SECURE_REQUEST));

			//DWORD buflen = 0;
			//CString strCookie;
			//m_pFile->QueryInfo(HTTP_QUERY_SET_COOKIE, strCookie, &buflen);

			//DWORD dwFlags;
			//m_pFile->QueryOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);
			//dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
			////set web server option
			//m_pFile->SetOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);
			m_pFile->AddRequestHeaders(L"Accept: *,*/*");
			m_pFile->AddRequestHeaders(L"Accept-Language:utf-8");
			m_pFile->AddRequestHeaders(L"Content-Type: application/x-www-form-urlencoded");
			//m_pFile->AddRequestHeaders(L"Content-Type: application/json");
			//m_pFile->AddRequestHeaders(L"Accept-Encoding: gzip, deflate,sdch");

			//m_pFile->AddRequestHeaders(L"Cache-Control: no-cache");
			if (strMethodOverride != L"")
				m_pFile->AddRequestHeaders(strMethodOverride);
			int num = WideCharToMultiByte(CP_OEMCP, NULL, strPostData, -1, NULL, 0, NULL, FALSE);
			char *pchar = new char[num];
			WideCharToMultiByte(CP_OEMCP, NULL, strPostData, -1, pchar, num, NULL, FALSE);
			m_pFile->SendRequest(NULL, 0, (LPVOID)pchar, num-1);
			DWORD dwRet;
			m_pFile->QueryInfoStatusCode(dwRet);

			if (dwRet >= 300)
			{
				CString errText;
				errText.Format(L"POST³ö´í£¬´íÎóÂë£º%d", dwRet);
				AfxMessageBox(errText);
			}

			char szChars[BUFFER_SIZE + 1] = { 0 };
			string strRawResponse = "";
			UINT nReaded = 0;
			while ((nReaded = m_pFile->Read((void*)szChars, BUFFER_SIZE)) > 0)
			{
					szChars[nReaded] = '\0';
					strRawResponse += szChars;
					memset(szChars, 0, BUFFER_SIZE + 1);
			}
			int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, strRawResponse.c_str(), -1, NULL, 0);
			WCHAR *pUnicode = new WCHAR[unicodeLen + 1];
			memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
			MultiByteToWideChar(CP_UTF8, 0, strRawResponse.c_str(), -1, pUnicode, unicodeLen);
			CString cs(pUnicode);
			delete[]pUnicode;
			pUnicode = NULL;
			strResponse = cs;
			Clear();
	}
		catch (CInternetException* e)
	{
			Clear();
			DWORD dwErrorCode = e->m_dwError;
			e->Delete();
			DWORD dwError = GetLastError();
			//PRINT_LOG("dwError = %d", dwError, 0);
			if (ERROR_INTERNET_TIMEOUT == dwErrorCode)
			{
					return OUTTIME;
			}
			else
			{
					return FAILURE;
			}
	}
		return SUCCESS;
}
int CHttpClient::HttpGet(LPCTSTR strUrl, LPCTSTR strPostData, CString &strResponse)
{
		return ExecuteRequest(L"GET", strUrl, strPostData, strResponse);
}
int CHttpClient::HttpPost(LPCTSTR strUrl, LPCTSTR strPostData, CString &strResponse)
{
	return ExecuteRequest(L"POST", strUrl, strPostData, strResponse);
}
int CHttpClient::HttpPut(LPCTSTR strUrl, LPCTSTR strPostData, CString &strResponse)
{
	return ExecuteRequest(L"PUT", strUrl, strPostData, strResponse);
}