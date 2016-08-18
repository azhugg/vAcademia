// ResourceImpl.h: interface for the CResourceImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESOURCEIMPL_H__182A18AD_976F_49A8_955E_B8FCEB26FEB3__INCLUDED_)
#define AFX_RESOURCEIMPL_H__182A18AD_976F_49A8_955E_B8FCEB26FEB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <iostream>
#include <vector>
using namespace std;
#include "ResManCreation.h"

#include "res_consts.h"
#include "Comman.h"

using namespace res;


class CResMan;
void makeDir( const wchar_t* aDirPath);

// ��������� ������� ������� IResource
// ������-������� ������ ����������� �������� ������ ����� ������
// � ������ ��� �������� E_NOTIMPL ��������� ���� ����������������
class CResourceImpl : 
	//public CComObjectRootEx<CComMultiThreadModel>,
	public IResource,
	public CResManCreation
{
friend CResMan;
public:
	CResourceImpl();
	virtual ~CResourceImpl();

public:
	virtual void open( const wchar_t*  aResPath, DWORD aFlags, IResourceNotify *aNotify = NULL, const wchar_t*  aResRefinePath = NULL);
	const wchar_t* getStrStatus();
	virtual void getTime( FILETIME& aTime) { if(aTime.dwLowDateTime == 0) return; return;};

public:
	// IResource
	virtual void destroy();
	virtual long close();
	virtual long read(BYTE* aBuffer, DWORD aBufferSize, DWORD* aReadSize, DWORD *aError = NULL);
	virtual long write(const BYTE* aBuffer, DWORD aBufferSize, DWORD * aWroteSize, DWORD aTotalSize = 0);
	virtual long seek(DWORD aPos, BYTE aFrom);
	virtual long getPos(DWORD* aPos);
	virtual long getSize(DWORD* aSize);
	//virtual long getLastModifyDateTime(DWORD* aDateTime);
	long getCachedSize(DWORD* aSize);
	long getCacheProgress( BYTE* aPercent);
	virtual long getStatus(DWORD* aStatus);
	virtual long setSize(DWORD aSize);
	long unpack( const wchar_t* aPath, BOOL aWait, __int64 aFreeBytesAvailable);
	long getUnpackProgress( BYTE* aPercent, DWORD *pwUnpackTotalSize = NULL);
	long getURL(BSTR* a_ppURL);
	bool getRefineLoadedResourcePath(wchar_t* pBuffer, unsigned int auBufferSize);
	long unpackToMemory(/*[in]*/ BSTR aFileName, /*[out]*/ BOOL* aSuccessful, /*[in, out]*/ DWORD* aDataSize, /*[out, size_is( *aDataSize)]*/ BYTE** aData);
	// ������� ������ ����������� ����� ������
	long  getDownloadedSize( /*[out]*/ DWORD* aSize);
	// ��������� ������
	long  download( /*[in]*/ IResourceNotify* aNotify, /*[in]*/const wchar_t* apwcLocalPath);
	// ������� ������� �������� ������
	long  getDownloadProcess( /*[out]*/ BYTE* aPercent);
	long unpack2(const wchar_t *aPath,  /*[in]*/ IResourceNotify* aNotify);
	// ��������� ������
	long  downloadToCache( /*[in]*/ IResourceNotify* aNotify);
	long unpackToCache(/*[in]*/ BSTR aPath,  /*[in]*/ IResourceNotify* aNotify);
	long  saveToCache(/*[in]*/void* pCacheData, DWORD dwCacheDataSize);
	long upload(/*[in]*/ IResource* aSrcRes,  /*[in]*/ IResourceNotify* aNotify);
	virtual long getUploadedSize( /* [out] */ DWORD *aSize);
	long registerAtResServer( IResourceNotify* aNotify){ if(aNotify == NULL) return E_FAIL;return E_NOTIMPL;};

protected:
	void initAfterOpen( DWORD aStatus);
	void createCache( DWORD aCachingBlock, bool* aEnabled);
	virtual void unpackRes( DWORD aCachingBlock, bool* aEnabled);
	void unpackFile( const wchar_t* aZipFile);
	BOOL unpackFileToMemory( const wchar_t*  aZipFile, const wchar_t*   aFileName, DWORD *aDataSize, BYTE **aData);
	void notifyResMan( DWORD aNotifyCode, DWORD aNotifyServerCode = 0, wchar_t *aErrorText = NULL);
	inline bool isMemCache() { return (dwStatus & RES_MEM_CACHE) != 0 && !bMemCacheCreating && pCacheData != NULL;};
	inline bool isFileCache() { return (dwStatus & RES_FILE_CACHE) != 0 && !bFileCacheCreating && hCacheFile != INVALID_HANDLE_VALUE;};
	virtual int cacheThread();
	virtual int unpackThread();
	bool downloadRes( bool aCacheOnly = false);
	virtual int downloadThread();
	virtual int downloadToCacheThread();

	bool isCashingNow();

	/*Jedi*/
	void GetListFileFromDir( const wchar_t* aExtractDir, vector<std::wstring> &FileExtractDir);
	BOOL CompareFileInList(const wchar_t* aZipFile, const wchar_t* aUnpackPath);
	void GetZipList( const wchar_t* aZipFile, vector<std::wstring> &FileZip);
	/**/

protected:
	CWComString	sExtractFile;
	CWComString	sExtractDir;
	// ���� � �����
	CWComString sResPath;
	// ���� ��� ���������� ����� (�����������)
	CWComString sResLocalPath;
	// ������ �� ����������� �����������
	bool bSpecExtension;
	// ���� � refine �����
	CWComString sResRefinePath;
	// ������ ���� � �����
	CWComString sFullResPath;
	// ������ �������
	DWORD dwStatus;
	// ������ ������� (��� ���������� ���������)
	DWORD dwStatusUploader;
	// ���. ���� ���������� ���� ����������� � ������, �� ��������� ������ ����������� �����
	BYTE* pCacheData;
	DWORD dwCacheDataSize;
	DWORD dwCachePos;
	// ���� � ����
	HANDLE hCacheFile;
	// ������ ���� ����� � ����
	CWComString sCacheFilePath;
	// ����� �������� ����
	bool bMemCacheCreating;
	bool bFileCacheCreating;
	// ������ ����������� � ��� ����� �������
	DWORD dwCachedSize;
	// ���������� ������ ������������ �����������
	volatile HANDLE hCacheThread;
	// ��������� ������ ������ ������������ �����������
	bool bEnableCacheThread;
	// ����� ����������
	volatile HANDLE hUnpackThread;
	// ����� ������ ���������� �� ������
	DWORD dwWriteCount;
	// ��������� ������ ������ ����������
	volatile bool bEnableUnpackThread;
	// ��������� ������ ������ �������� �� ������
	volatile bool bEnableUploadThread;
	// �������� � �������� ������� ��� ����������
	bool bNotifyDownload;
	// ���� ����������
	CWComString sUnpackPath;
	// ���������� ������ � ������
	DWORD dwUnpackTotalNumber;
	// ���������� ������������� ������ � ������
	DWORD dwUnpackNumber;
	// ��������� ������ ������ � ������
	DWORD dwUnpackTotalSize;
	// ��������� ������ ������������� ������ � ������
	DWORD dwUnpackSize;
	__int64 freeBytesAvailable;
	//
	volatile HANDLE hDownloadThread;
	//
	volatile HANDLE hUploadThread;
	volatile HANDLE hDownloadToCacheThread;
	//
	bool bIsCashingNow;

	// ������ �����
	DWORD dwFileSize;
	CMutex mutexClose;

private:
	volatile bool m_needDestroyFromDestroy;
	volatile bool m_canDestroyFromCache;
	volatile bool m_canDestroyFromDownload;
	volatile bool m_canDestroyFromDownloadCache;
	
	volatile bool m_canDestroyFromUnzip;
	

protected:
	volatile bool m_canDestroyFromUpload;
	volatile bool m_deleteResultGiven;
	CMutex m_mutexDestroy;
	//
	CWComString sStatus;
	IResourceNotify*	spResourceNotify;
	IResource* pSrcResource; // �������� ������ ��� Upload-�
private:
	friend int WINAPI unzService(LPCSTR aName, unsigned long aSize);
	static DWORD WINAPI cacheThread_(LPVOID param);
	static DWORD WINAPI unpackThread_(LPVOID param);
	static DWORD WINAPI downloadThread_(LPVOID param);
	static DWORD WINAPI downloadToCacheThread_(LPVOID param);	

	bool CanDelete();

protected:
	void CheckAndDestroyIfNeeded(volatile bool *aFlagToSet);

};

#endif // !defined(AFX_RESOURCEIMPL_H__182A18AD_976F_49A8_955E_B8FCEB26FEB3__INCLUDED_)
