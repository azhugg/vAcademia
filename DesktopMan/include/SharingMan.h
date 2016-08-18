#pragma once

#include "selectors/IDesktopSelector.h"
#include "view/DrawWindow.h"
#include "SessionFactory.h"
#include "DestinationsFactory.h"
#include "ConnectionFactory.h"
#include "SessionFactory.h"
#include "SelectorFactory.h"
#include "sharingmousekeyboardhandler.h"
#include "stateCodes.h"
#include "HookMouse.h"
#include <map>

//////////////////////////////////////////////////////////////////////////
namespace oms
{
	class omsContext;
}

class CDesktopRectSelector;
class CSharingSessionView;
class CDrawWindow;
class CDevicesCtrl;

//////////////////////////////////////////////////////////////////////////
class DESKTOPMAN_API CSharingManager : public IDesktopSharingManager
						, public IDesktopSelectorEvents
						, public IPreviewWindowEvents
						, public IHookMouseEvents
						
{
public:
	bool IsNewCodec;

	CSharingManager(oms::omsContext* aContext);
	virtual ~CSharingManager();

	void SendDestinationRequest( LPCSTR alpcTarget);
	void RaiseSessionStartedEvent( LPCSTR alpcTarget, LPCSTR alpcSessionKey, LPCSTR alpcFileName);
	void RaisePreviewCancelEvent( LPCSTR alpcTarget);
	void RaisePreviewOKEvent( LPCSTR alpcTarget);
	void RaiseSharingEventWithCode(LPCSTR alpcTarget, int aEventCode);
	void RecordStartEvent( LPCSTR alpcTarget, LPCSTR alpcFileName);
	void RaiseViewSessionStartedEvent( LPCSTR alpcTarget, LPCSTR alpcSessionKey, bool canRemoteAccess);
	void SessionClosedEvent(LPCSTR alpcTarget);
	void TextureSizeChangedEvent(LPCSTR alpcTarget, int width, int height);
	void ViewSessionClosedEvent(LPCSTR alpcTarget);
	void WaitOwnerEvent(LPCSTR alpcTarget);
	void RemoteAccessModeChangedEvent( LPCSTR alpcTarget, bool canRemoteAccess);

	bool RecordPlaying()
	{
		return m_playing;
	}
	LPCSTR GetTargetGroupNameInRecord()
	{
		if (!potentialTargetGroupNameInRecord.IsEmpty())
			return potentialTargetGroupNameInRecord.GetBuffer();
		else
			return NULL;
	}
	bool StartRecording(unsigned int aRecordID, LPCSTR alpTarget, LPCSTR alpcLocation);
	bool StopRecordingInFileSession();
	void ResumeServerSharingInternal();
	CComString GetNameInZeroReality(LPCSTR alpTarget);
	CComString ConvertNameToRecordReality( LPCSTR alpTarget, unsigned int auBornRealityID);
	CComString ConvertFromUnicName( LPCSTR alpTarget);
	CComString ConvertToUnicName( LPCSTR alpTarget, unsigned int auBornRealityID);
	CComString ConvertToUnicName( LPCSTR alpTarget);
	unsigned int GetRealityIDFromUnicName( LPCSTR alpTarget);
	unsigned int GetObjectRealityIDByTarget( LPCSTR alpTarget);
	CComString GetNameCycleSession( LPCSTR alpTarget);
	void SendNotificationToServerForFileSession(unsigned int idMessage);
	void AddVASEPack( void* pack, bool abKeyFrame, CSharingViewSession* viewSession);

	// ���������� IDesktopSharingManager
public:
	//�������� ��� ����� � ������
	void UpdateFileName( LPCSTR alpTarget, LPCSTR alpFileName);
	//�������� ��� �����
	void GetAllFrames( LPCSTR alpSessionName);
	//�������� ������ �������� �����
	void GetOnlyKeyFrame( LPCSTR alpSessionName);
	//! ������� ������
	void Release();
	//! �������� Sharing �������
	bool StartCapturingRectRequest( LPCSTR alpTarget, LPCSTR alpGroupName);
	//! �������� Sharing ����
	bool StartCapturingWndRequest( LPCSTR alpTarget, LPCSTR alpGroupName);

	bool StartCapturingNotepadWndRequest( LPCSTR alpTarget, LPCSTR alpGroupName);

	bool StartCapturingWnd( LPCSTR alpTarget, LPCSTR alpGroupName, bool NPad);
	//! �������� Sharing c ���������
	bool StartCapturingVideoInputRequest( LPCSTR alpTarget, LPCSTR alpGroupName);
	// �������� Sharing � ������������� �����
	bool StartCapturingInteractiveBoard( LPCSTR alpTarget, LPCSTR alpGroupName);
	//! �������� Sharing ����������
	bool StartCapturingVideoFileRequest( LPCSTR alpTarget, LPCSTR alpGroupName, LPCWSTR alwFileName);
	//! ������������� Sharing
	void StopCapturing( LPCSTR alpTarget);
	void StopCapturingAll();
	
	//���������� ������� ������� �� ����� ������
	void ShowSharingSessionRectangle( LPCSTR alpSessionName);
	void HideSharingSessionRectangle( LPCSTR alpSessionName);
	
	//! �������� �������� Sharing ������
	virtual	bool StartSharingView( LPCSTR alpTarget, LPCSTR alpGroupName, LPCSTR alpFileName);
	//! �������� �������� Sharing ������
	virtual	bool SetSharingViewFps( LPCSTR alpTarget, LPCSTR alpGroupName, unsigned int fps);
	//! ����������� ������ ��� �������� �������
	virtual	bool StopSharing( LPCSTR alpTarget);
	//! ������������� ��������� ��������� ����������� � ������������ �������
	virtual bool	SetSharingServerAddress(LPCSTR alpcGroupName
		, LPCSTR alpcServerAddress, unsigned int aiRecordPort);
	//!
	CSharingSessionFactory& GetSessionsFactory()
	{
		return sessionsFactory;
	}
	void	NotifyOnWaitOwnerEventInternal( LPCSTR alpcTarget);
	//
	void	SetDestinationPointer( LPCSTR alpcTarget, IDesktopSharingDestination* apIDesktopSharingDestination);
	//! ��������� ���������� ��������������� �������
	bool AddCallback( desktopIDesktopManCallbacks* apCallback);
	//! ������� ���������� ��������������� �������
	bool RemoveCallback( desktopIDesktopManCallbacks* apCallback);
	bool Update();

	//! �������� ������ ������� ��� ������� ������
	virtual bool StartRecording(unsigned int aRecordID, LPCSTR alpcLocation);
	
	//! ������ � ����� ��������� �������
	virtual bool StartListenRecording( unsigned int aRecordID, LPCSTR alpcLocation);

	//! ����������� ������ ������� ��� ������� ������
	virtual bool StopRecording(unsigned int aRecordID);

	//! ������� �� ������ ��������� �������
	virtual bool StopListenRecording( unsigned int aRecordID);

	//! �������� ��������������� ��������� ������ ��� ��������� ����������
	virtual bool StartPlayingSharing(unsigned int aRecordID, unsigned int aRealityID);
	//! ����������� ��������������� ������������ ������
	virtual bool StopPlayingSharing(); 

	//! �������� �������� ������ �������
	virtual	bool StartSharingPlay( LPCSTR alpTarget, LPCSTR alpGroupName, LPCSTR alpFileName, bool isPaused, unsigned int aRecordID, unsigned int aShiftTime, unsigned int aServerVersion);
	//! ���������������� ��������������� ������������ ������ ��� ���� ������, ����������� � ������
	virtual void PauseServerSharing();

	//! ���������� ��������������� ������������ ������, ����������� � ������
	virtual void ResumeServerSharing();

	//! ������������ ��������������� ������������ ������, ����������� � ������
	virtual void RewindServerSharing();

	//! �������� ������� ������� �� ������������ ��������������� ������������ �����
	virtual bool PauseUserSharing( LPCSTR alpSessionName);
	//! �������� ������� ������� �� ����������� ��������������� ������������ �����
	virtual bool ResumeUserSharing( LPCSTR alpSessionName);
	//! ������� ������ (�������� ������ �� ������)
	virtual bool DeleteRecord(unsigned int aRecordID);
	//
	virtual	bool SetBoardOwnerChanged( LPCSTR alpTarget, LPCSTR alpGroupName, unsigned int auState);
	
	virtual void AllowRemoteControl( LPCSTR alpSessionName);
	virtual void DenyRemoteControl( LPCSTR alpSessionName);
	virtual void* GetIMouseKeyboardHandler();
	virtual void TraceAllSession();
	virtual void NotifyVideoDeviceInitialized(int resultCode);
	virtual void StopWaitWebCameraResponce(LPCSTR alpTarget);
	virtual void SetCursorMode(LPCSTR alpTarget, int mode);
	virtual void FullControlChanged(LPCSTR alpTarget, int mode);

	virtual std::string GetAllViewSession();
	virtual std::string GetAllSharingSession();
	virtual std::string GetSessionInfo(LPCSTR alpTarget);
	virtual void ShowLastFrame(LPCSTR alpTarget);

	bool SetupInteractiveBoardSettings();
	bool EmulateRectRequest( LPCSTR alpTarget, LPCSTR alpGroupName);
	
	bool Seek( LPCSTR alpTarget, double aSecond);
	bool Play( LPCSTR alpTarget);
	bool Pause( LPCSTR alpTarget);
	double GetDuration( LPCSTR alpTarget);

	// ���������� IDesktopSelectorEvents
public:
	void OnSelectionCompleted(int resultCode);
	void OnSelectionEscape();
	void OnSetBarPosition( LPCSTR aTarget, std::vector<double> v1);
	void OnVideoEnd( LPCSTR aTarget);

	// ���������� IPreviewWindowEvents
public:
	void OnPreviewWindowOK(int aScaleMode);
	void OnPreviewWindowCancel();	
	// IHookMouseEvents
public:
	void OnGlobalMouseEvent(MouseEvent aMouseEvent);

public:
	void OnSetResolutionCompleted(int resultCode);

private:
	bool	ShowPreviewWindow(int resultCode);
	void	SetRecordState(unsigned int aRecordID, LPCSTR alpcLocation, bool aRecording, bool anAuthor);
	void	FreeCaptureBoardByPreview(LPCSTR alpNewTarget);
	void	CreateAndStartCapturingSession(int aQuality);
	void	BeforeStartNewSelector(LPCSTR alpTarget, LPCSTR alpGroupName);
	int		GetInteractiveBoardMonitorID();
	CSharingSession* GetInteractiveBoardSession();
	void	SendHookMouseEvent();

	IDesktopSelector *GetSelector( LPCSTR alpTarget);

private:
	bool m_playing;
	bool m_recording;
	bool m_enteredInRecord;
	bool m_isRecordAuthor;
	unsigned int m_recordID;
	CComString	m_recordLocation;
	unsigned int m_playOnSharingRecordID;
	unsigned int m_playOnSharingRealityID;
	SHARING_CONTEXT				context;

	std::map<CComString, unsigned int>	m_mapPlayRecIDByLocation;

	// Desktop selectors
	CComString					potentialTarget;
	CComString					potentialTargetGroupName;
	CComString					potentialTargetGroupNameInRecord;
	IDesktopSelector*			currentSelector;
	IWebCameraSelector*			webCameraSelector;

	// PreviewWindow
	CDrawWindow*				previewWindow;

	CSharingSessionFactory		sessionsFactory;
	CSharingConnectionFactory	connectionsFactory;
	CDestinationsFactory		destinationsFactory;
	SelectorFactory				selectorFactory;

	CSharingMouseKeyboardHandler* sharingMouseKeyboardHandler;

	CDevicesCtrl*				m_deviceController;

	//! Callbacks
	typedef	MP_VECTOR<desktopIDesktopManCallbacks*>	VecCallbacks;
	VecCallbacks	vecCallbacks;

	CCriticalSection csVecCallbacks;

	CHookMouse* m_hookMouse;
	MP_VECTOR<MouseEvent> m_hookMouseEvents;
};