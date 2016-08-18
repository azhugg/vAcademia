#pragma once

#include "IDesktopSelector.h"
#include "../SharingContext.h"
#include "cursorcapture.h"
#include "selectorbase.h"

class CDesktopFrame;

class CDesktopRectSelector : public CEventSource<IDesktopSelectorEvents>
							,	public IDesktopSelector
							, public CCursorCapture
							, public CSelectorBase
{
	friend class CCaptureWindow;
public:
	CDesktopRectSelector(oms::omsContext* apContext);
	virtual ~CDesktopRectSelector();

	typedef std::vector<MONITORINFOEX>		vecMonitorsInfo;
	typedef MP_VECTOR<CCaptureWindow*>	vecCaptureWindows;

	// ���������� IDesktopSelector
public:
	virtual bool Release()
	{
		MP_DELETE_THIS;
		return true;
	}
	// �������� ������� �������
	virtual bool StartSourceSelection();
	// ����������� ������� �������
	virtual void CancelSourceSelection();
	// ����������� ������� ������� �� ������� ESC
	virtual void EscSourceSelection();
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
	virtual bool IsLive()
	{
		return true;
	}
	virtual int GetType();
	void SetInteractiveBoardCapture(int aMonitorID);

	void EmulateSelection( int aLeft, int aRight, int aTop, int aBottom);
	virtual bool Run(){ return true;};
	void Seek( double aSeek){};
	void Play(){};
	void Pause(){};
	double GetDuration(){return 0;};

private:
	void	CreateCaptureWindows();
	void	CreateMonitorCaptureWindow( MONITORINFOEX& pInfo);
	void	UpdateSelectionRect( int aX, int aY, CCaptureWindow* apCaptureWindow);
	void	HideCaptureWindows();
	void	StartSelection( int aX, int aY, CCaptureWindow* apCaptureWindow);
	void	ClearCapturedImage();
	void	CreateCapturedImage();
	void	StopSelection( int aX, int aY, CCaptureWindow* apCaptureWindow);
	void	HandleMouseMove( int aX, int aY, CCaptureWindow* apCaptureWindow);
	void	DestroyCaptureWindows();
	void	ScaleSelectionRect(float dx, float dy);
	void	FindScale( MONITORINFOEX& info, float& dx, float& dy);

	CCaptureWindow*	GetVirtualBoardMonitor();
	void UpdateCurrentSelectingRect();

	void	CreateDCesIfNeeded();
	void	DeleteDCes();

private:
	vecCaptureWindows	captureWindows;
	vecCaptureWindows	captureBuffers;
	//! selection params
	bool				selectionOnProgress;
	RECT				selectionRect;
	POINT				startPoint;
	POINT				currentPoint;
	int					lButtonDownCount;

	//! Captured Image
	HBITMAP				hCapturedImage;
	CDesktopFrame*		currentFrame;

	int					screenWidth;
	int					screenHeight;

	HDC					m_hScreenDC;
	HDC					m_hBackHDC;

	oms::omsContext*	context;
	bool				m_isInteractiveBoardCapture;
	int					m_interactiveBoardMonitorID;
	bool				m_needUpdateInteractiveBoard;
};