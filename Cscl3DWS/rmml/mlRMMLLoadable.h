#pragma once

namespace rmml {

// ����� �������
#define MLELPN_SRC "src"
//#define MLELPN_WIDTH "_width"
//#define MLELPN_HEIGHT "_height"

// ����� �������
#define EVNM_onLoad L"onLoad"
#define EVNM_onLoading L"onLoading"
#define EVNM_onLoadAll L"onLoadAll"
#define EVNM_onUnload L"onUnload"
#define EVNM_onLoadError L"onLoadError"

/**
 * RMML-�������� ��� RMML-�������, ������� ����� ���� ��������� �� �����
 * (Image, Animation, ...)
 * (�� ������ � ����������� JavaScript!)
 */

class mlRMMLLoadable: 
									//public mlJSClassProto,
									public mlILoadable,
									moILoadable
{
friend class mlRMMLElement;
public:
	mlRMMLLoadable(void);
	~mlRMMLLoadable(void);
MLJSPROTO_DECL
protected:
	enum { JSPROP0_mlRMMLLoadable=mlRMMLElement::TEVIDE_mlRMMLElement-1,
		JSPROP_src,
		//JSPROP_bytesLoaded,
		//JSPROP_bytesTotal,
		JSPROP_streaming,
		TIDE_mlRMMLLoadable
	};
	enum { EVID_mlRMMLLoadable=TIDE_mlRMMLLoadable-1,
		EVID_onLoad,	// �������� �������� ������ (��������� � src) (RMML_LOAD_STATE_SRC_LOADED)
		EVID_onLoading,	// ������� ������� �������� ��������� ������� (��������� � src) (RMML_LOAD_STATE_SRC_LOADING)
		EVID_onLoadAll,	// ��������� ��� ����������� �������, ������� ������� �������� ��������� (RMML_LOAD_STATE_ALL_RESOURCES_LOADED)
		EVID_onUnload,	// ����������� ��� ������� (����� ��������� ������ ������� ��� ��� ����������� �������)
		EVID_onLoadError,	// ��� �������� ������ �� �������� ��������� ������ (RMML_LOAD_STATE_LOADERROR)
		TEVIDE_mlRMMLLoadable
	};

//	#define	LST_NOT_LOADED							6
	#define	LST_SRC_NOT_SET							5
	#define	LST_NOT_LOADED							4
	#define	LST_LOADING								3
	#define	LST_LOAD_ERROR							2
	#define	LST_RES_LOADED							1
	#define	LST_LOADED								0 // ������ ���������� � onLoad ���-�� � �������

protected:
	JSString* src;
	MP_WSTRING msSrcFull;

	unsigned char miSrcLoaded;	// ������, ��������� � src ��� ��������
	unsigned long miLoadedBytes; // ���-�� ����������� ������ �����
	unsigned long miTotalBytes; // ����� ����� ������ (������) �����
	bool mbStreaming; // ���������, ��� ������� ������ � �������� �������� �� ������ �������� ��� � ���������

	// 1 - "UI" - ��������� �������� ������� ����������. � ��� �� ��������� ������� � ����������� ����.
	// 2 - "ForShowNow" - ��� ���������� ������ �� ��������. ��� ��, ��� �� ����� ���������� �� �����. � ���� ��������� ���� ��������� �������� � �������.
	// 3 - "ForShowLater" - ��� �������� �� � ����������. ��� ��, ��� �� �������� ����� ����� ����������. ��� ��������� ������. � ���� ��������� ���� ��������� �������� � �������.
	// 4 - "ForShowSomeDay" - ��� ������� ��������. ��� ��, ��� � ��������� ����� ��� �� ����. �������� ������ ������������ ������ �����������.
	ELoadingPriority	mLoadingPriority;	// ��������� �������� �������

	JSFUNC_DECL(getBytesLoaded)
	JSFUNC_DECL(getBytesTotal)
	JSFUNC_DECL(reload)
	JSFUNC_DECL(getLoadingPriority)
	JSFUNC_DECL(setLoadingPriority)

protected:
	virtual mlRMMLElement* GetElem_mlRMMLLoadable()=0;
	bool Lod_SetEventData(char aidEvent, mlSynchData &Data);
	void Set_Event_state(mlRMMLElement* apMLEl, int aiState, wchar_t *aUserErrorString, wchar_t *aWarningText);
	void Set_Event_errorCode(mlRMMLElement* apMLEl, int aiErrorCode, int aiUserErrorCode, wchar_t *aUserErrorString);
public:
	virtual void ResourceLoadedAsynchronously(mlSynchData &Data){}

// ����������  mlILoadable
public:
	// ��������
	const wchar_t* GetRawSrc() const;
	const wchar_t* GetSrc();
	ELoadingPriority GetLoadingPriority() const;
	bool IsStreaming();
	bool IsLoading();
	// �������
	void onLoad(int aiState);
	void onLoading(int amiLoadedBytes, int amiTotalBytes);
	void onUnload();
	void SetLoaded();
// ���������� moILoadable
protected:
	moILoadable* GetmoILoadable();
	// ��������
	virtual bool SrcChanged();
	// ������
	virtual bool Reload();
	// ����� ���������� �������� �������
	void LoadingPriorityChanged( ELoadingPriority aPriority);
};

#define MLILOADABLE_IMPL \
public: \
	mlRMMLElement* GetElem_mlRMMLLoadable(){ return this; } \
	mlILoadable* GetILoadable(){ return this; } \
	mlRMMLLoadable* GetLoadable(){ return this; }

//	mlRMMLElement* GetElem(){ return this; } \
//	void OnLoad(){ mlRMMLLoadable::onLoad(this); } \
//	void OnUnload(){ mlRMMLLoadable::onUnload(this); }

}
