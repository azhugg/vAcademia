// ResModule.h: interface for the CResModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESMODULE_H__75BF4B13_857C_418D_AEE9_A0419467DF3F__INCLUDED_)
#define AFX_RESMODULE_H__75BF4B13_857C_418D_AEE9_A0419467DF3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ResManCreation.h"
#include <libxml/xpath.h>
#include "comman.h"
#include "BigFileman.h"

using namespace res;

class CResModule : 
	//public CComObjectRootEx<CComMultiThreadModel>,
	public IResModule,
	public IResModuleMetadata,
	public CResManCreation
{
public:
	CResModule();
	virtual ~CResModule();

	/*DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CResModule)
		COM_INTERFACE_ENTRY(IResModule)
		COM_INTERFACE_ENTRY(IResModuleMetadata)
	END_COM_MAP()*/

public:
	virtual void open( const wchar_t* aModuleID, const wchar_t* aModulePath, const wchar_t* aModuleFile, DWORD aFlags);

	void setStatus( DWORD aStatus){ dwStatus = aStatus;};
public:
// IResModule
	// ������� ������ � ������� ������ �� ������
	void destroy();
	// ������� ������
	long close();
	// ������� ������
	long openResource(/*[in]*/ BSTR aResPath, /*[in]*/ DWORD aFlags, /*[out]*/ IResource** aRes);
	// ������� ������ ����������
	long openAsynchResource( BSTR aResPath,  DWORD aFlags, IResourceNotify* aNotify, IResource** aRes);
	// ��������� ���������� �� ������ � ������
	long resourceExists(/*[in]*/ BSTR aResPath, /*[in]*/ DWORD aFlags, /*[out]*/ DWORD* aStatus, bool abCheckOnlyExistInList = false);
	// ������� ������ ������
	long getStatus(/*[out]*/ DWORD* aStatus);
	// ������� ������ ������
	long getSize( /*[out]*/ DWORD* aSize);
	// ������� ������ ����������� ����� ������
	long getDownloadSize( /*[out]*/ DWORD* aSize);
	// ��������� ������
	long download( /*[in]*/ BOOL aWait);
	// ������� ������� �������� ������
	long getDownloadProcess( /*[out]*/ BYTE* aPercent);
	// ����������� ������
	long unpack( /*[in]*/ BOOL aWait);
	// ������� ������� ���������� ������
	//long getUnpackProgress( /*[out]*/ BYTE* aPercent, DWORD *pwUnpackTotalSize = NULL);
	// ���������� ������
	//long pack( /*[in]*/ BOOL aWait);
	// ������� ������� ��������� ������
	//long getPackProgress( /*[out]*/ BYTE* aPercent);
	long getMetadata( /*[out]*/ IResModuleMetadata** aMetadata);
	long copyResource(/*[in]*/ BSTR aSrcPath, /*[in]*/ IResModule* aDestModule, /*[in]*/ BSTR aDestPath);
	long copyFile(/*[in]*/ BSTR aSrcPath, /*[in]*/ BSTR aDestPath);
	long moveFile(/*[in]*/ BSTR aSrcPath, /*[in]*/ BSTR aDestPath, /*[out]*/ BOOL* aSuccess);
	long getFullPath(/*[in]*/ BSTR aResPath, /*[in]*/ DWORD aFlags, CWComString &sFullPath);
	long removeResource(/*[in]*/ BSTR aPath, /*[out]*/ BOOL* aSuccess);
	long downloadMetadata(BOOL aWait);
	long getRelation(/*[in]*/ int aRelN, CWComString &aModuleID);
	long extract(/*[in]*/ BSTR aSrcPath, /*[in]*/ BSTR aDestSubPath, CWComString &aFullPath);
	long resumeDownload();
	long pauseDownload();
	long getManifestDownloadedSize(/*[out]*/ DWORD* aSize);
	long getManifestSize(/*[out]*/ DWORD* aSize);

public:
	long removeFolder(/*[in]*/ BSTR aPath, /*[in]*/ BOOL aYesToAll, /*[out]*/ BOOL* aSuccess);
// IResModuleMetadata
	// ������� ������������� ������
	long getIdentifier(/*[out]*/ BSTR* aID);
	// ������� �������� ������
	long getTitle(/*[out]*/ BSTR* aTitle, /*[in, out]*/ BSTR* aLanguage);
	// ������� ���� ������
	long getLanguage(/*[out]*/ BSTR* aLanguage);
	// ������� �������� ������
	long getDescription(/*[out]*/ BSTR* aDescription, /*[in, out]*/ BSTR* aLanguage);
	// ������� ������ �� ����������
	long getMetadataString(/*[in]*/ BSTR aXPath, /*[out]*/ BSTR*aStr);
	long getTaxonomy( /*[out]*/ CWComString &aTaxonomy);

protected:
	bool loadMetadata();
	bool getMetadataStr( const wchar_t* aXPath,  wchar_t** aStr, xmlXPathObjectPtr* aResult = NULL, xmlNodePtr aRoot = NULL);
	bool getMetadataLangStr( const wchar_t* aXPath, wchar_t** aStr, BSTR* aLang);	
	bool downloadModule();
	bool downloadMetadata();
	static DWORD WINAPI downloadThread_(LPVOID param);
	virtual int downloadThread();
	static DWORD WINAPI downloadMetadataThread_(LPVOID param);
	virtual int downloadMetadataThread();
	bool openModule();
	long openFileInPackFile( BSTR aResPath, DWORD aFlags, IResourceNotify* aNotify, IResource** aRes);
	bool isFileInPack( DWORD aFlags);

protected:
	// ������ ������
	DWORD dwStatus;
	// ������������� ������
	CWComString sModuleID;
	// ������� ���� ��� �������� ������
	CWComString sModulePath;
	// ���� � ���������� ����� ������
	CWComString sModuleFile;
	// ������ ������
	DWORD dwSize;
	// ������ ����������� ����� ������
	DWORD dwDownloadedSize;
	// ���������� ������ ����������� ��������
	HANDLE hDownloadThread;
	// ��������� ������ ������ ����������� ��������
	bool bEnableDownloadThread;
	// ���������� ������ ����������� ��������
	HANDLE hDownloadMetadataThread;
	// ��������� ������ ������ ����������� ��������
	bool bEnableDownloadMetadataThread;
	// DOM ���������
	xmlDocPtr pXmlManifest;
	// ������ ���������
	DWORD dwManifestSize;
	// ������ ����������� ����� ���������
	DWORD dwManifestDownloadedSize;
	// ������� ��� ����������� ��������
	HANDLE hDownloadEvent;

	bigfile::IBigFileManagerReader *pBigFileManagerReader;

private:
	static int openThread_( CResModule* aThis);
	bool bUserModule;
};

#endif // !defined(AFX_RESMODULE_H__75BF4B13_857C_418D_AEE9_A0419467DF3F__INCLUDED_)
