#ifndef __DESKTOPMAN_H_
#define __DESKTOPMAN_H_

#ifdef DESKTOPMAN_EXPORTS
#define DESKTOPMAN_API __declspec(dllexport)
#else
#define DESKTOPMAN_API __declspec(dllimport)
#endif

#ifdef WIN32
#define DESKTOPMAN_NO_VTABLE __declspec(novtable)
#else
#define DESKTOPMAN_NO_VTABLE
#endif

#include "oms_context.h"
#include "rmml.h"

class IDynamicTexture;

namespace desktop
{
	typedef DWORD sharingError;

	// 
	struct IDesktopSharingDestination
	{
		virtual bool SetDynamicTexture( IDynamicTexture* apIDynamicTexture) = 0;
		virtual bool DynamicTextureDeleting( IDynamicTexture* apIDynamicTexture) = 0;
		virtual bool ResizeImage() = 0;
		virtual void ResetResize() = 0;
	};

	struct DESKTOPMAN_NO_VTABLE desktopIDesktopManCallbacks
	{
		virtual void onApplyFrameRequest( LPCSTR lpcTarget, int bpp, int width, int height, const void* bits, int quality) = 0;
		virtual void onDesktopDestinationRequest( LPCSTR lpcTarget) = 0;
		virtual void onRaiseSessionStartedEvent( LPCSTR lpcTarget, LPCSTR alpcSessionKey, LPCSTR alpcFileName) = 0;
		virtual void onRaisePreviewCancelEvent(LPCSTR lpcTarget) = 0;
		virtual void onRaisePreviewOKEvent(LPCSTR lpcTarget) = 0;
		virtual void onSharingServerFileSaved(LPCSTR lpcTarget, LPCSTR lpcFileName) = 0;
		virtual void onRaiseViewSessionStartedEvent( LPCSTR lpcTarget, LPCSTR alpcSessionKey, bool canRemoteAccess) = 0;
		virtual void onTextureSizeChangedEvent(LPCSTR lpcTarget, int width, int height) = 0;
		virtual void onSessionClosedEvent(LPCSTR lpcTarget) = 0;
		virtual void onViewSessionClosedEvent(LPCSTR lpcTarget) = 0;
		virtual void onWaitOwnerEvent(LPCSTR lpcTarget) = 0;
		virtual void onRemoteAccessModeChangedEvent( LPCSTR lpcTarget, bool canRemoteAccess) = 0;
		virtual void onVideoInputDeviceInitializedEvent(LPCSTR lpcTarget, int resultCode) = 0;
		virtual void onSharingEventByCode(LPCSTR alpcTarget, int aCode) = 0;
		virtual void onRaiseGlobalMouseEvent(unsigned int wParam, float aX, float aY) = 0;
		virtual void onSetBarPositionEvent(LPCSTR lpcTarget, double aSecond, double aDuration) = 0;
		virtual void onVideoEndEvent(LPCSTR lpcTarget) = 0;
	};

	struct /*DESKTOPMAN_NO_VTABLE */IDesktopManager
	{
		//! ������� ������
		virtual void Release() = 0;

		//! ��������� ������ ��� ��������� ������ ������
		virtual void SetScreenCaptureObjectTarget( LPCSTR lpcTarget, float proportion = 1.33f) = 0;

		virtual void SetScreenCaptureObjectTarget( LPCSTR lpcTarget, int mode,  float proportion = 1.33f, LPCWSTR filePath = NULL ) = 0;

		//! ��������� ���������� ��������������� �������
		virtual bool AddCallback( desktopIDesktopManCallbacks* apCallback) = 0;

		//! ������� ���������� ��������������� �������
		virtual bool RemoveCallback( desktopIDesktopManCallbacks* apCallback) = 0;

		//! �������� ������ 
		virtual bool Update() = 0;
	};

	DESKTOPMAN_API omsresult CreateDesktopManager( oms::omsContext* aContext);
	DESKTOPMAN_API void	DestroyDesktopManager( oms::omsContext* aContext);


	/************************************************************************/
	/* Desktop sharing
	/************************************************************************/

	// ��������� ������� �� IDesktopSharingSession
	struct IDesktopSharingSessionCallback
	{
		virtual void OnFrameReady( LPCSTR alpTarget, int bpp, int width, int height, const void* bits, int quality) = 0;
	};

	// ���� Sharing c����� - ������� ���� ����������
	struct IDesktopSharingSession
	{
		//! ���������� �������� ����������� (�� ���� �������� ������ ��� JPEG)
		virtual void SetQuality( unsigned int auQuality) = 0;
		//! ���������� ������������ FPS
		virtual void SetMaxFPS( unsigned int auFPS) = 0;
		//! �������� �� �������
		virtual void Subscribe( IDesktopSharingSessionCallback* apCallback) = 0;
		virtual void Unsubscribe( IDesktopSharingSessionCallback* apCallback) = 0;
	};

	struct IMouseKeyboardHandler
	{	
		virtual void SetActiveTarget(LPCSTR alpTarget) = 0;
		virtual void SetRealityId(unsigned int aRealityID) = 0;
		virtual void OnLeftMouseDown(UINT aFirstParam, UINT aSecondParam) = 0;
		virtual void OnLeftMouseUp(UINT aFirstParam, UINT aSecondParam) = 0;
		virtual void OnLeftMouseDblClk(UINT aFirstParam, UINT aSecondParam) = 0;
		virtual void OnRightMouseDown(UINT aFirstParam, UINT aSecondParam) = 0;
		virtual void OnRightMouseUp(UINT aFirstParam, UINT aSecondParam) = 0;
		virtual void OnRightMouseDblClk(UINT aFirstParam, UINT aSecondParam) = 0;
		virtual void OnMouseMove(UINT aFirstParam, UINT aSecondParam) = 0;
		virtual void OnKeyUp(UINT aFirstParam, UINT aSecondParam) = 0;
		virtual void OnKeyDown(UINT aFirstParam, UINT aSecondParam) = 0;	
	};

	struct IDesktopSharingManager
	{
		//! �������� ������ 
		virtual bool Update() = 0;
		//! ������� ������
		virtual void Release() = 0;
		//! �������� ����� ������� ��� Sharing`�
		virtual	bool StartCapturingRectRequest( LPCSTR alpTarget, LPCSTR alpGroupName) = 0;
		//! �������� ����� ���� ��� Sharing`�
		virtual	bool StartCapturingWndRequest( LPCSTR alpTarget, LPCSTR alpGroupName) = 0;
		//! �������� ����� ���� ��� Sharing`�
		virtual	bool StartCapturingNotepadWndRequest( LPCSTR alpTarget, LPCSTR alpGroupName) = 0;
		//! �������� Sharing c ���������
		virtual bool StartCapturingVideoInputRequest( LPCSTR alpTarget, LPCSTR alpGroupName) = 0;
		//! �������� ����� �������� ��� Sharing`�
		virtual	bool StartCapturingInteractiveBoard( LPCSTR alpTarget, LPCSTR alpGroupName) = 0;
		//! �������� Sharing ����������
		virtual bool StartCapturingVideoFileRequest( LPCSTR alpTarget, LPCSTR alpGroupName, LPCWSTR alwFileName) = 0;
		//! ������������� Sharing
		virtual	void StopCapturing( LPCSTR alpTarget) = 0;
		virtual	void StopCapturingAll() = 0;
		//! ����������� ������ ��� �������� �������
		virtual	bool StopSharing( LPCSTR alpTarget) = 0;
		//! ������������� ��������� ��������� ����������� � ������������ �������
		virtual bool	SetSharingServerAddress(LPCSTR alpcGroupName
											, LPCSTR alpcServerAddress, unsigned int aiRecordPort) = 0;
		//
		virtual void	SetDestinationPointer( LPCSTR alpcTarget, IDesktopSharingDestination* apIDesktopSharingDestination) = 0;
		virtual void	NotifyOnWaitOwnerEventInternal( LPCSTR alpcTarget) = 0;
		//! ��������� ���������� ��������������� �������
		virtual bool AddCallback( desktopIDesktopManCallbacks* apCallback) = 0;

		//! ������� ���������� ��������������� �������
		virtual bool RemoveCallback( desktopIDesktopManCallbacks* apCallback) = 0;

		/*
		��� ������ ���� �������, ������� �������� �� ������ ��������� � ������ ������ ������, �������� ��� ���������.
		������ � ����� ��������� ������� � ������ ������ � �����, � ������� ��� ������������. ������, ������� ���
		��������� ������� � ������ ������� EVID_onSharingFileSaved, �� �������� ������������� ���������������� ��������:
			synch.slideParams.sharingSessionKey = ��� �����
			synch.slideParams.sharingSessionSaved = 1
		*/
		// �������� ����� ������
		//! �������� ������ ������� ��� ������� ������
		virtual bool StartRecording(unsigned int aRecordID, LPCSTR alpcLocation) = 0;
		// �������� �� ����� ������, �� ������� ���� ����� �������
		//! ������ � ����� ��������� �������
		virtual bool StartListenRecording( unsigned int aRecordID, LPCSTR alpcLocation) = 0;

		//! ����������� ������ ������� ��� ������� ������
		// �������� �� ����� ������, �� ������� ���� ����� �������
		/*
		�������� ��������� �� ������ �� ��������� ������ ����� ������ �������.
		*/
		virtual bool StopRecording(unsigned int aRecordID) = 0;
		// �������� �� ����� ������, �� ������� ���� ����� �������
		//! ������� �� ������ ��������� �������
		virtual bool StopListenRecording( unsigned int aRecordID) = 0;

		//! ������������� fps �������� ������ �������
		virtual	bool SetSharingViewFps( LPCSTR alpTarget, LPCSTR alpGroupName, unsigned int auFps) = 0;
		//! �������� �������� ������ �������
		virtual	bool StartSharingView( LPCSTR alpTarget, LPCSTR alpGroupName, LPCSTR alpFileName) = 0;

		//! �������� ��������������� ��������� ������ ��� ��������� ����������
		virtual bool StartPlayingSharing(unsigned int aRecordID, unsigned int aRealityID) = 0;
		//! ����������� ��������������� ������������ ������
		virtual bool StopPlayingSharing() = 0;
		//! �������� �������� ������ �������
		virtual	bool StartSharingPlay( LPCSTR alpTarget, LPCSTR alpGroupName, LPCSTR alpFileName, bool isPaused, unsigned int aRecordID, unsigned int aShiftTime, unsigned int aServerVersion) = 0;

		//! ���������������� ��������������� ������������ ������ ��� ���� ������, ����������� � ������
		virtual void PauseServerSharing() = 0;
		//! ���������� ��������������� ������������ ������, ����������� � ������
		virtual void ResumeServerSharing() = 0;
		//! ������������ ��������������� ������������ ������, ����������� � ������
		virtual void RewindServerSharing() = 0;
		//! �������� ������� ������� �� ������������ ��������������� ������������ �����
		virtual bool PauseUserSharing( LPCSTR alpSessionName) = 0;
		//! �������� ������� ������� �� ����������� ��������������� ������������ �����
		virtual bool ResumeUserSharing( LPCSTR alpSessionName) = 0;
		//! ������� ������ (�������� ������ �� ������)
		virtual bool DeleteRecord(unsigned int aRecordID) = 0;
		//
		virtual	bool SetBoardOwnerChanged( LPCSTR alpTarget, LPCSTR alpGroupName, unsigned int auState) = 0;
		
		virtual void ShowSharingSessionRectangle( LPCSTR alpSessionName) = 0;
		
		virtual void HideSharingSessionRectangle( LPCSTR alpSessionName) = 0;
		virtual void GetOnlyKeyFrame( LPCSTR alpSessionName) = 0;
		virtual void GetAllFrames( LPCSTR alpSessionName) = 0;
		//�������� ��� ����� � ������
		virtual void UpdateFileName( LPCSTR alpTarget, LPCSTR alpFileName) = 0;
		virtual void AllowRemoteControl( LPCSTR alpSessionName) = 0;
		virtual void DenyRemoteControl( LPCSTR alpSessionName) = 0;
		virtual void* GetIMouseKeyboardHandler() = 0;
		virtual void TraceAllSession() = 0;
		virtual void NotifyVideoDeviceInitialized(int resultCode) = 0;
		virtual void StopWaitWebCameraResponce(LPCSTR alpTarget) = 0;
		virtual void SetCursorMode(LPCSTR alpTarget, int mode) = 0;
		virtual void FullControlChanged(LPCSTR alpTarget, int mode) = 0;

		virtual std::string GetAllViewSession() = 0;
		virtual std::string GetAllSharingSession() = 0;
		virtual std::string GetSessionInfo(LPCSTR alpTarget) = 0;
		virtual void ShowLastFrame(LPCSTR alpTarget) = 0;

		//! ��������� ��������� ������������� �����
		virtual	bool SetupInteractiveBoardSettings() = 0;	

		virtual	bool EmulateRectRequest( LPCSTR alpTarget, LPCSTR alpGroupName) = 0;

		virtual	bool Seek( LPCSTR alpTarget, double aSecond) = 0;
		virtual	bool Play( LPCSTR alpTarget) = 0;
		virtual	bool Pause( LPCSTR alpTarget) = 0;
		virtual	double GetDuration( LPCSTR alpTarget) = 0;
	};


	DESKTOPMAN_API omsresult CreateSharingManager( oms::omsContext* aContext);
	DESKTOPMAN_API void	DestroySharingManager( oms::omsContext* aContext);
}

#endif