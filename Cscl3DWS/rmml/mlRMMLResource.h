#pragma once

namespace rmml {

/**
 * ����� �������� ��� ��������/����������/�������� ��������
 */

class mlRMMLResource :
					public mlRMMLElement,
					public mlJSClass,
					public mlRMMLLoadable,
					public res::resIAsynchResource
{
	res::resIResource* mpResource;
	char mchUnpackState; // 0 - �� ���� �������������, 1 - ������������ ���, 2 - ���������������, 3 - ����������
	bool mbUploading;
	res::IAsyncCheckExist *pAsynchCheckExist;
	MP_WSTRING mwsWarning;
	
	void Reset();
	unsigned int GetState();
public:
	mlRMMLResource(void);
	void destroy() { MP_DELETE_THIS }
	~mlRMMLResource(void);
MLJSCLASS_DECL
protected:
	enum {
		JSPROP_size = 1,	// RO ������ �����
		JSPROP_downloaded,	// RO ������� ���� ��������
		JSPROP_state, 		// RO ��������� (���������� ������|���� � ��|���� � �� � ����������)
		JSPROP_downloading, // RW ���� ������������/�� ������������
		JSPROP_uploaded,	// RO ������� ���� �������� � ������
		JSPROP_uploading,   // RW ���� ������������/�� ������������
		JSPROP_warningText,	// RO 
		JSPROP_RS_NOT_FOUND,	// 0 - ������ �� ������
		JSPROP_RS_REMOTE,		// 1 - ������ ������ �� �������, �� �� ������� � �� ������������
		JSPROP_RS_DOWNLOADING,	// 2 - ������ ������������ � �������
		JSPROP_RS_LOCAL,		// 3 - ������ ������ �� ��������� ����� ��� �� CD
		JSPROP_RS_UNPACKING,	// 4 - ������ �� ��������� ����� � ������ ���������������
		JSPROP_RS_READY,		// 5 - ������ �� ��������� �����, ���������� � ����� � �������������
		JSPROP_RS_UPLOADING,	// 10 - ������ ������������ � ������ (���� � ���������, ���� �� ������)
		JSPROP_RS_UPLOADING_PAUSE,	// 11 - ������� ������� ��������������
		JSPROP_RS_UPLOADED,		// 12 - ������ �������
		JSPROP_RS_ERROR,		// 13 - ������ �� �������
		JSPROP_RS_REGISTERING,  // 14 - ������ ��������������
		JSPROP_RS_REGISTERED,   // 15 - ������ ���������������
    };
//?	bool downloading;

	JSFUNC_DECL(isExist)	// ���������, ���������� �� ������
	JSFUNC_DECL(download)	// ������ ����������� ������� (�� �� �������������)
	JSFUNC_DECL(downloadAndUnpack)	// ������ ����������� � ����������
	JSFUNC_DECL(getProgress)	// �������� ��������� �������/���������� � ���������
	JSFUNC_DECL(copyTo)		// ����������� ���������� � ������������� ������ � �������� �����������
	JSFUNC_DECL(registerFileAtResServer)	// ���������������� ������ �� ������� ��������

	bool IsExist( const wchar_t* apwcPath, bool abLocal = false, bool abSync = false);
	bool StartDownloading( char aUnpackState, const wchar_t* apwcLocalPath=NULL);
	void StateChanged();
	bool RegisterFileAtResServer(const wchar_t* apwcPath);

public:
	bool StartUploading(const wchar_t* apwcDestPath, const wchar_t* apwcSrcPath, bool abPackSource = false);

// ���������� mlRMMLElement
MLRMMLELEMENT_IMPL
	mlresult CreateMedia(omsContext* amcx);
	mlresult Load();
	mlRMMLElement* Duplicate(mlRMMLElement* apNewParent);
	mlresult Update(const __int64 alTime);
	bool Freeze();
	bool Unfreeze();

// ����������  mlILoadable
MLILOADABLE_IMPL

// ���������� moILoadable
	virtual bool SrcChanged();

// ���������� res::resIAsynchResource
	virtual omsresult OnLoaded();
	virtual omsresult OnDownloaded();
	virtual omsresult OnCached();
	virtual omsresult OnUnpacked();
	virtual omsresult OnError( unsigned int auiCode, unsigned int auiServerCode = 0, wchar_t* aNotifyServerText = NULL);
	virtual omsresult OnUploaded(wchar_t* aNotifyServerText);
	virtual omsresult OnExisted( bool auiCode);
	virtual omsresult OnRegistered(wchar_t* aNotifyServerText);
};

}