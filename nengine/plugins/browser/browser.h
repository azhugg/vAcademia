#pragma once

#include <windows.h>
#include "webhost.h"
//#include "WebKit.h"
#include "IBrowser.h"
#include "IDynamicTexture.h"
#include <string>

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

class CBrowser : public IBrowser, public IDynamicTexture
{
friend class CBrowserWebHost;
public:
	CBrowser(IBrowserCallBack* apCallBack, HWND ahwndParentWindow);
	~CBrowser();

// ���������� IBrowser
public: 
	// ���������� ������������ ��������
	virtual IDynamicTexture* GetDynamicTexture();
	// ���������� ������ "����" ��������
	virtual bool SetSize(int aiWidth, int aiHeight);
	// ��������� ������� ����
	virtual bool FireMouseDown(int x, int y); // x,y ������������ ������ �������� ���� ���� ��������
	virtual bool FireMouseMove(int x, int y);
	virtual bool FireMouseUp(int x, int y);
	virtual bool FireMouseWheel(int x, int y, int distance);
	// ��������� ������� ����������
	virtual bool KeyDown(unsigned int keyCode, unsigned int keyStates, unsigned int scanCode);
	virtual bool KeyUp(unsigned int keyCode, unsigned int keyStates, unsigned int scanCode);
	virtual bool KeyPress(unsigned int charCode, unsigned long scanCode);
	virtual bool SetFocus();
	virtual bool KillFocus();
	// 
	virtual bool Navigate(const wchar_t* apwcURL, int flags=0, const wchar_t* postData=NULL, const wchar_t* headers=NULL);
	//
	virtual bool ExecScript(const wchar_t* scriptCode, VARIANT* result);
	// ���������� ����������� ������, ������� ���� ������������ ��� ��������� ��������� 
	// ��������������� �������� ���� ��������
	virtual void* GetCSForChildWindows();
	// �������� ����� �������� ����
	virtual bool SetParentWindow(HWND aHwnd);
	// ��������� � �������� HTML-�������� ������
	virtual bool LoadHTMLString(const wchar_t* apwcHTMLString, const wchar_t* apwcEncoding = NULL);

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

private:
	// ��������� �������������
//	bool init();
	HRESULT CreateWebKitObject(REFCLSID rclsid, REFIID riid, LPVOID * ppv);
	void ShowErrorPage(const wchar_t* apwcURL, int aiCode, const wchar_t* apwcError);

private:
	IBrowserCallBack* m_callBack;
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
	//
	HMODULE m_webKit;
	CBrowserWebHost* m_webHost;
	IWebView* m_webView;
	//HWND m_ViewWindow;
	HGDIOBJ m_oldBitmap;

	// ����������� ������, �������������, ��� ��������� ������� � ��������� �� ����� ����������� ������������
	LPCRITICAL_SECTION m_cs;

	HWND m_parentWindow;

	// url, ������� ���� ��������� ����� �������������
	std::wstring m_urlAfterInit;

	void* m_userData[4];
};
