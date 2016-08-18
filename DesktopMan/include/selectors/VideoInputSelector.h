#pragma once

#include "IDesktopSelector.h"
#include "../SharingContext.h"
#include "selectorbase.h"

class CDesktopFrame;
class CDevicesCtrl;
class CVideoInputSelectorDialog;
class CVideoInputRunChecker;
class CVideoInputGraphRunning;

class CVideoInputSelector: public CEventSource<IDesktopSelectorEvents>
						, public IDesktopSelector, public IWebCameraSelector
						, public CSelectorBase
{
public:
	CVideoInputSelector(oms::omsContext* apContext, CDevicesCtrl* aDeviceController);
	~CVideoInputSelector(void);

	// ���������� IDesktopSelector
public:
	virtual bool Release();
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
	virtual void SetShowCursorState(int state){}
	virtual bool IsLive();
	virtual int GetType();
	virtual void EmulateSelection( int aLeft, int aRight, int aTop, int aBottom){};
	virtual bool Run(){ return true;};
	void SetInteractiveBoardCapture(int aMonitorID){};
	void UpdateCurrentSelectingRect(){};
	void Seek( double aSeek){};
	void Play(){};
	void Pause(){};
	double GetDuration(){return 0;};
	//���������� IWebCameraSelector
public:
	//��������� ����� ��������
	virtual int GetWebNumsCameraDevices();
	//��������� ���������� ���������
	virtual std::vector<Resolution> GetWebCameraResolutions(int numDevice);
	virtual bool SetResolution(Resolution newResolution, int numDevice, bool reRun);
	virtual std::string GetWebCameraName(int numDevice);
	virtual bool SetActiveDevice(int numDevice);

public:
	CDevicesCtrl* GetDeviceController();
	void SelectionComplete(int resultCode);
	void SetResolutionComplete(int resultCode);
	void VideoInputGraphRunEnd();

private:
	void ClearCapturedImage();
	void Destroy();
	void CleanUp();

public:
	bool				m_firstCaptureFrame;
	CRect				m_currentRect;
	int					m_numActiveDevice;
	MP_STRING			m_deviceDisplayName;
	CCriticalSection	csDestroy;
	bool				m_deviceInitializing;
	bool				m_needDeleteFromDeviceRunner;
	bool				m_deviceWasStarted;
	bool				m_isDeleted; // ����� ������� ������ ����� ������������� ������ ����������

private:
	CDesktopFrame*		m_currentFrame;
	CDevicesCtrl*		m_deviceController;
	int					m_size;
	CVideoInputSelectorDialog* m_viDialog;
	CVideoInputGraphRunning*	m_deviceRunner;
	CVideoInputRunChecker* m_runChecker;

	oms::omsContext*	context;
};

class CVideoInputRunChecker: public ViskoeThread::CThreadImpl<CVideoInputRunChecker>
{
public:
	CVideoInputRunChecker(CVideoInputSelector* selector);
	~CVideoInputRunChecker(void);
	// ���������� CThreadImpl<CVideoInputGraphRunning>
	DWORD Run();

private:
	CVideoInputSelector*		m_VISelector;
	CVideoInputGraphRunning*	m_graphRunningThread;
};

class CVideoInputGraphRunning: public ViskoeThread::CThreadImpl<CVideoInputGraphRunning> 
{
public:
	CVideoInputGraphRunning(CVideoInputSelector* selector, bool notifyAboutComplete);
	~CVideoInputGraphRunning(void);
	// ���������� CThreadImpl<CVideoInputGraphRunning>
	DWORD Run();

private:
	CVideoInputSelector*		m_VISelector;
	bool						m_notifyAboutComplete;
};
