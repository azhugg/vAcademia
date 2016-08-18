#pragma once

#include "IDesktopSelector.h"
#include "../SharingContext.h"
#include "WndSelectorDialog.h"
#include "cursorcapture.h"
#include "selectorbase.h"

class CDesktopFrame;
class CWndSelectorDialog;
class CCaptureNotepadRunning;

typedef BOOL (WINAPI *PISWINREDIRECTEDFOPRINT)();

class DESKTOPMAN_API CWndSelector: public CEventSource<IDesktopSelectorEvents>
					, public IDesktopSelector 
					, public ISelectorWindowEvents
					, public CCursorCapture
					, public CSelectorBase
{
public:
	CWndSelector(oms::omsContext* apContext, bool NPad);
	virtual ~CWndSelector(void);
	typedef std::vector<MONITORINFOEX>		vecMonitorsInfo;
	bool Release(void);
	/*{
		MP_DELETE_THIS;
		return true;
	}*/
	bool IsLive()
	{
		return true;
	}
	void FromWindow(HWND hWindow);

// ���������� ISelectorWindowEvents
public:
	void OnSelectorWindowOK();
	void OnSelectorWindowCancel();

// ���������� IDesktopSelector
public:
	// �������� ������� �������
	virtual bool StartSourceSelection();
	// ����������� ������� �������
	virtual void CancelSourceSelection();
	// ���������� ��������� ���������� ����
	virtual IDesktopFrame* GetCurrentFrame();
	// ������ ������ ������ ����� � �������� �����������
	virtual IDesktopFrame* RecapFrame();
	// ���������� ������� ������� �������
	virtual RECT GetCurrentSelectingRect();
	//���������� true ���� ������ ��������
	virtual bool IsCaptureAvailable(); 
	//���������� handle ���� � �������� ������������� ����������� ��� NULL � ��������� ������
	virtual HWND GetCapturedWindow();
	//����� �� ���������� ������� � ������� ������ ������
	virtual bool CanShowCaptureRect();
	//���������� ����� ����������� ������� (0-�����, 1-������������, 2-������������ �����������)
	virtual void SetShowCursorState(int state);
	virtual int GetType();
	virtual void EmulateSelection( int aLeft, int aRight, int aTop, int aBottom){};
	virtual bool Run(){ return true;};
	void UpdateCurrentSelectingRect(){};
	void Seek( double aSeek){};
	void Play(){};
	void Pause(){};
	double GetDuration(){return 0;};
public:
	int CaptureNotepadIfNeeded();
	bool OpenNotepad();
	bool CaptureNotepad();
	void ReturnError(int anErrorCode);
	PROCESS_INFORMATION pinf;
	HWND hWnd;
	bool NP;
	bool StartSelection();
	HWND StopSelection();
	HWND DoMouseMove();
	bool IsSelecting()
	{
		return bStartSearchWindow;
	}
	
	void SetMainWindowHandle(HWND hwnd)
	{
		hwndMainWindow = hwnd;
	}

	HWND GetMainWindowHandle()
	{
		return hwndMainWindow;
	}

	void Set_g_hwndFoundWindow(HWND wnd);
	void SetInteractiveBoardCapture(int aMonitorID){}

	static bool SaveBitmapToFile(HBITMAP *pHBmp);
private:
	HWND hwndMainWindow;
	bool bStartSearchWindow;
	bool bCaptureAvailable;

	HWND hwndDialog;
	HWND hwndFoundWindow;
	HWND g_hwndFoundWindow;
	//! Captured Image
	//HDC					hMemDC;
	//HBITMAP				hCapturedImage, hOldBitmap;
	CDesktopFrame*		currentFrame;
	CWndSelectorDialog* wndSelectorDlg;

	int					screenWidth;
	int					screenHeight;

	HDC					m_hScreenDC;
	HDC					m_hBackHDC;

	oms::omsContext*	context;

	unsigned int		m_lastCaptureTime;
	unsigned int		m_updateWindowTime;
	unsigned int		m_blackScreenCount;

private:
	bool GetWindowSize(RECT &rec, int &width, int &height);
	void RefreshWindow(HWND hwndWindowToBeRefreshed, bool aTotal=false);
	bool CheckWindowValidity (HWND hwndToCheck);
	void HighlightFoundWindow (HWND hwndFoundWindow);
	//void CreateHBitmap(int &width, int &height);
	//void DeleteHBitmap();
	void ClearCapturedImage();
	void DefineScreenSize();
	void CreateDCesIfNeeded();
	void DeleteDCes();
	bool IsCapturedImageBlack(unsigned char* imgBits, int width, int height);
	CCaptureNotepadRunning *	m_notepadRunner;
};

class CCaptureNotepadRunning: public ViskoeThread::CThreadImpl<CCaptureNotepadRunning> 
{
public:
	CCaptureNotepadRunning(CWndSelector* selector, bool notifyAboutComplete);
	~CCaptureNotepadRunning(void);
	// ���������� CThreadImpl<CVideoInputGraphRunning>
	DWORD Run();

private:
	CWndSelector *	m_notepadSelector;
	bool			m_notifyAboutComplete;
};
