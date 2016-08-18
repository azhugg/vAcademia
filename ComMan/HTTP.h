// HTTP.h: interface for the HTTP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTP_H__14358B89_2945_4450_BC97_FA2B95177016__INCLUDED_)
#define AFX_HTTP_H__14358B89_2945_4450_BC97_FA2B95177016__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "wininet.h"
#include "HttpQuery.h"
//#include "LogFile.h"
//#include "curl/curl.h" 
#include "../curl/include/curl/curl.h"
#include "DataBuffer.h"
#include "consts.h"
#include "ILogWriter.h"
#include "oms_context.h"
#include "ProxySettings.h"
#include "iclientsession.h"
#include "ConnectionManager.h"

class CGuardCS{
	LPCRITICAL_SECTION mpcs;
public:
	CGuardCS(LPCRITICAL_SECTION pcs){
		mpcs=pcs;
		EnterCriticalSection(mpcs);
	}
	~CGuardCS(){
		LeaveCriticalSection(mpcs);
	}
};

// ���� ��������
enum EQueryType {
	qtStandart = 0, qtMultipart = 1, qtGet = 2, qtPost = 3, qtReadFile = 4
};

// ������������� HTTP ������
enum EProxyUsing {
	puDirect = 0, puProxy, puPreconfig
};

struct CHttpQuery;

struct CRequestHandles{
	HINTERNET mhINet;
	HINTERNET mhConnection;
	HINTERNET mhData;
	CRequestHandles(){
		mhINet = NULL;
		mhConnection = NULL;
		mhData = NULL;
	}
};

// ����� ������� � HTTP-�������
class CHTTP:
	public cm::IChangeHttpSettings
{
public:
	CHTTP( oms::omsContext* apContext, ILogWriter* lw);
	virtual ~CHTTP();

	// ������� ������
	EQueryError openQuery(BSTR aQueryStr, BSTR aParams, CHttpQuery*& aQuery);
	EQueryError closeQuery(CHttpQuery*& aQuery);
	EQueryError readQueryData( CHttpQuery& aQuery, DWORD aBufferSize, BYTE aBuffer[], DWORD& aReadSize);
	// ������� ������. ������� ��������� �����
	EQueryError sendQuery(BSTR aQuery, BSTR aParams, CComString &aResult, EQueryType qType=qtStandart);
	EQueryError sendQuerySync(BSTR aQuery, BSTR aParams, CComString &aResult, EQueryType qType=qtStandart);
	// ������� ������. ������� �������� �����
	EQueryError sendQueryData( BSTR aQuery, BSTR aParams, DWORD* aDataSize, BYTE** aData,
		EQueryType qType=qtStandart, LPVOID mpData=NULL, DWORD mpSize=0, bool aThread = true);
	// ������� ������ � ��������� ������
	EQueryError sendQueryDataInThread( BSTR aQuery, BSTR aParams, CURL *pcurl, DWORD* aDataSize, BYTE** aData, EQueryType qType=qtStandart, LPVOID mpData=NULL, DWORD mpSize=0);
	// ������� ������������� �����
	inline long get_sessionID() { return dwSessionID;}
	// ���������� ������������� �����
	inline void put_sessionID( long aVal) { dwSessionID = aVal; sSession.Format( "sessionID=%lu", dwSessionID);}
	// ������� ����� �������
	inline LPCSTR get_server() { return sServerName;}
	// ���������� ����� �������
	inline void put_server( BSTR newVal) { sServerName = newVal;}
	// ������ �����
	void resetSession();
	// ������� ������� ���� ��������
	inline LPCSTR get_queryBase() { return sQueryBase;}
	// ���������� ������� ���� ��������
	inline void put_queryBase( BSTR newVal) { sQueryBase = newVal;}
	// 
	LPTSTR getErrorStr( EQueryError aError, CComString* aErrorStr);
	// 
	inline EProxyUsing get_proxyUsing() { return nProxyUsing;}
	// 
	inline void put_proxyUsing( EProxyUsing aProxyUsing) { nProxyUsing = aProxyUsing;}
	// 
	inline LPCSTR get_proxy() { return sProxy;}
	// 
	inline void put_proxy( LPCSTR aProxy) { sProxy = aProxy;}
	inline void put_proxy( BSTR aProxy) { sProxy = aProxy;}
	// 
	inline LPCSTR get_proxyUser() { return sProxyUser;}
	// 
	inline void put_proxyUser( LPCSTR aProxyUser) { sProxyUser = aProxyUser;}
	inline void put_proxyUser( BSTR aProxyUser) { sProxyUser = aProxyUser;}
	// 
	inline LPCSTR get_proxyPassword() { return sProxyPassword;}
	// 
	inline void put_proxyPassword( LPCSTR aProxyPassword) { sProxyPassword = aProxyPassword;}
	inline void put_proxyPassword( BSTR aProxyPassword) { sProxyPassword = aProxyPassword;}

	void SetILogWriter(ILogWriter *lw) {m_pLW = lw;}

	void applyProxySettings(IProxySettings* aProxySettings);

// cm::IChangeHttpSettings
public:
	virtual void OnChangeConnectionSettings(void* aProxyMode, void* aProxySettings);

protected:
	// ����� �������
	CComString sServerName;
	// ���� �������
	INTERNET_PORT nPort;
	// ������������
	CComString sUser;
	// ������
	CComString sPassword;
	// ������������� ������ �������
	EProxyUsing nProxyUsing;
	// ����� ������ �������
	CComString sProxy;
	// ������������ ������ �������
	CComString sProxyUser;
	// ������ ������ �������
	CComString sProxyPassword;
	// ������������� �����
	DWORD dwSessionID;
	// ������ � ��������������� �����. ����������� � ������� �������
	CComString sSession;
	// ����, ���������� �������� �������� ��� �������
	CComString sQueryBase;
	//
	ILogWriter * m_pLW;
	//
	CRITICAL_SECTION csSendQuery;
	// �������
	oms::omsContext* pContext;

private:
	// ������������ ������ ������� � �����������
	CComString formatQueryString(BSTR aQuery);
	CComString formatParamsString(BSTR aParams);
	// �������� ����� ������ ������� � ��� �����
	//bool GetServerReply( HINTERNET m_hRequest, BYTE** lpData, DWORD* dwSize);
	// ������� ������ �� ������ multipart ��������
	//EQueryError SendMultipartRequest( HINTERNET m_hRequest, LPVOID lpData, DWORD dwSize);
	void SendMultipartRequest( CURL *curl, CDataBuffer *pData);
};

#endif // !defined(AFX_HTTP_H__14358B89_2945_4450_BC97_FA2B95177016__INCLUDED_)
