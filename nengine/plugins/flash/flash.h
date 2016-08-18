#pragma once

#include "FlashNP.h"
#include "WindowSO.h"
#include <map>

class CGuardByCS
{
public:
	CGuardByCS::CGuardByCS(LPCRITICAL_SECTION apCS){
		m_pCS = apCS;
		if(m_pCS == NULL)
			return;
		EnterCriticalSection(m_pCS);
	}
	CGuardByCS::~CGuardByCS(){
		if(m_pCS == NULL)
			return;
		LeaveCriticalSection(m_pCS);
	}
private:
	LPCRITICAL_SECTION m_pCS;
};

class CFlash : 
	public IFlash, 
	public IDynamicTexture, 
	public IAsynchResourceHandler
{
friend class CFlashNP;
// ����� ��� ������������� ��������� Flash;
	class CFlashHeader
	{
	public:
		CFlashHeader(FILE *in = 0);
		CFlashHeader(const void* apData, unsigned int auSize);
		bool is_valid() const { return _version != 0;}

	public:
		long  _version,
			_x_min, _x_max, _y_min, _y_max,
			_frame_rate,_frame_count;
		bool  _is_compresed;

	private:
		bool decompressSwf(FILE *in);
		bool decompressSwf(const unsigned char* apData, unsigned int auSize);
		void parseSwfHeader(const unsigned char *buff);

		// 32 ����� ������ ������� �� ���������
		unsigned char _rowSwfHeader[32];

	private:
		unsigned char bitPos;     // bitPos is our "position" in out bitBuffer "array", valid values are 0-8 (8 means get a new buffer)
		unsigned char bitBuffer;  // this is a byte that we'll be treating like an array
	};

public:
	CFlash(IFlashCallBack* apCallBack, HWND ahwndParentWindow, IResLibrary* apResLibrary);
	~CFlash();

// ���������� IFlash
public: 
	// ���������� ������������ ��������
	virtual IDynamicTexture* GetDynamicTexture();
	// ���������� ������ "����" ��������
	virtual bool SetSize(int aiWidth, int aiHeight);
	virtual bool LoadMovie(const wchar_t* apwcSrc); 

	virtual bool SetVariable(const wchar_t *var, const wchar_t *value);

	virtual int GetScaleMode();
	virtual void SetScaleMode(int);

	virtual int GetWMode();
	virtual void SetWMode(int);

	virtual const wchar_t* GetVariable(const wchar_t *var);
	virtual const wchar_t* CallFunction(const wchar_t *request);

	virtual bool SetFocus();
	virtual bool KillFocus();

// ���������� IDynamicTexture
public: 

	// �������� ��������� ������� (�� ��������)
	virtual void Update();
	// �������� ��������
	virtual void UpdateFrame();

	// ������� ������ �� ������ � ���������� ���������� �����	
	virtual void* GetFrameData();

	// ������� ������ �������� �������� � ��������
	virtual unsigned short GetSourceWidth();
	// ������� ������ �������� �������� � ��������
	virtual unsigned short GetSourceHeight();
	// ������� ������ �������������� �������� � ��������
	virtual unsigned short GetTextureWidth();
	// ������� ������ �������������� �������� � ��������
	virtual unsigned short GetTextureHeight();
	// ���������� ������ �������������� ��������
	virtual void SetTextureSize(int width, int height);
	// ������� ���������� �������� �������, 3 ��� RGB, 4 ��� RGBA
	virtual unsigned char GetColorChannelCount();
	// ������� �������� ������� ������ (RGB) ��� �������� (BGR)
	virtual bool IsDirectChannelOrder();

	// ����� �� ��������� �������� (��� ������ � ���������� �� ����������?)
	virtual bool IsNeedUpdate();

	// ������� ����
	// (��������� ��� ����, ����� nengine ��� ��� ������ ����� ��� ��������� �������)
	virtual void Destroy();

	void* 	GetUserData(unsigned int userDataID);
	void 	SetUserData(unsigned int userDataID, void* data);

	void SetFreezState(bool /*isFreezed*/){};
	bool IsFreezed(){return false;};
	bool IsFirstFrameSetted(){return false;};

// IAsynchResourceHandler
public:
	// �������� �������� �������� �������
	virtual void OnAsynchResourceLoadedPersent(IAsynchResource* asynch, unsigned char percent);
	// ������ ��������
	virtual void OnAsynchResourceLoaded(IAsynchResource* asynch);
	// ������ ��������
	virtual void OnAsynchResourceError(IAsynchResource* asynch);

// NPN
public:
	NPError getURLNotify(const char* url, const char* target, void* notifyData){
		return load(url, target, true, notifyData);
	}
	NPError getURL(const char* url, const char* target){
		return load(url, target, false, 0);
	}
	NPError postURLNotify(const char* url, const char* target, uint32 len, const char* buf, NPBool file, void* notifyData){
		return NPERR_GENERIC_ERROR;
	}
	NPError postURL(const char* url, const char* target, uint32 len, const char* buf, NPBool file){
		return NPERR_GENERIC_ERROR;
	}

	NPError newStream(NPMIMEType type, const char* target, NPStream** stream){
		return NPERR_GENERIC_ERROR;
	}

	int32 write(NPStream* stream, int32 len, void* buffer){
		return NPERR_GENERIC_ERROR;
	}

	NPError destroyStream(NPStream* stream, NPReason reason){
		return NPERR_GENERIC_ERROR;
	}

	static const char* userAgentStatic();

	const char* userAgent();

	void status(const char* message){
		// ??
	}

	void invalidateRect(NPRect* invalidRect);

	void invalidateRegion(NPRegion invalidRegion);

	void forceRedraw();

	static NPError getValueStatic(NPNVariable variable, void* value){
		// ??
		return NPERR_GENERIC_ERROR;
	}

	NPError getValue(NPNVariable variable, void* value);

	NPError setValue(NPPVariable variable, void* value);

private:
	NPError load(const char* url, const char* target, bool sendNotification, void* notifyData);
	NPError loadAndNotify(ifile* apFile, NPStream* pStream, bool sendNotification, void* notifyData);

private:
	IFlashCallBack* m_callBack;
	bool m_initialized;
	bool m_paintAll; // ������������ ��� �������� (true) ��� ������ ��������� (false)

	unsigned char m_colorChannelCount;
	unsigned char* m_frameData;
	//	
	unsigned short m_width;
	unsigned short m_height;
	unsigned short m_textureWidth;
	unsigned short m_textureHeight;
	//
	HBITMAP m_bitmap;
	HDC m_hdc;

	// ����� �������, ������� ������ ���� ������������ � UpdateFrame()
	unsigned int m_updateWidth;
	unsigned int m_updateHeight;
	unsigned int m_updateTextureWidth;
	unsigned int m_updateTextureHeight;
	//
	HDC m_updateHDC;
	HBITMAP m_updateBitmap;

	HGDIOBJ m_oldBitmap;

	// ����������� ������, �������������, ��� ��������� ������� � ��������� �� ����� ����������� ������������
	LPCRITICAL_SECTION m_cs;

	NPP m_instance;
	NPP_t m_instanceStruct;
	NPWindow m_npWindow;

	CWindowSO m_window;

	HWND m_parentWindow;
	bool m_needToUpdate; // ���� ������������ ��������

	bool m_isWindowed;
	bool m_isTransparent;

	IResLibrary* m_resLib;
	IAsynchResourceManager*	m_asynchResMan;

	struct FlashStream{
		NPStream* stream;
		IAsynchResource* asynchRes;
		bool sendNotification;
		void* notifyData;
		FlashStream(NPStream* apStream, IAsynchResource* apAsynchRes, bool abSendNotification, void* aNotifyData): 
			stream(apStream), asynchRes(apAsynchRes), 
			sendNotification(abSendNotification), 
			notifyData(aNotifyData)
		{
		}
	};
	// ����������� stream-�
	typedef std::map<IAsynchResource*, FlashStream> MapAsynchRes;
	MapAsynchRes m_mapAsynchRes;

	std::wstring m_wsLastGetVarRetVal;

	void* m_userData[4];
};
