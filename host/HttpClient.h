#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <afxinet.h>
#include <string>
using namespace std;
#define  IE_AGENT  _T("Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727)")

// �����ɹ�
#define SUCCESS        0
// ����ʧ��
#define FAILURE        1
// ������ʱ www.it165.net
#define OUTTIME        2

class CHttpClient
{
public:
		CHttpClient(LPCTSTR strAgent = IE_AGENT);
		virtual ~CHttpClient(void);
		int HttpGet(LPCTSTR strUrl, LPCTSTR strPostData, CString &strResponse);
		int HttpPost(LPCTSTR strUrl, LPCTSTR strPostData, CString &strResponse);
		int HttpPut(LPCTSTR strUrl, LPCTSTR strPostData, CString &strResponse);
private:
		int ExecuteRequest(LPCTSTR strMethod, LPCTSTR strUrl, LPCTSTR strPostData, CString &strResponse);
		void Clear();
private:
		CInternetSession *m_pSession;
		CHttpConnection *m_pConnection;
		CHttpFile *m_pFile;
};

#endif // HTTPCLIENT_H
