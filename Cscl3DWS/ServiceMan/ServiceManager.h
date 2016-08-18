#pragma once
#include "serviceman.h"
#include "serviceManagerBase.h"
#include "imessagehandler.h"
#include "iclientsession.h"
#include <map>
#include "Mutex.h"
#include "IPadManager.h"


using namespace service;
class CServiceManager :
	public CServiceManagerBase,
	//public service::IBaseServiceManager,
	public IServiceManager,
	public cs::imessagehandler,
	public cs::isessionlistener
{
public:
	CServiceManager( omsContext* aContext);
	virtual ~CServiceManager();

// ----------------------------------------------------	
// ���������� �-� ���������� IBaseServiceManager
// ----------------------------------------------------	
	// ���������� � ������� ��������
	virtual void Connect();
	// ��������� �� ������� ��������
	virtual void Disconnect();
	// ������� �������� �������������
	virtual void Release();
	// �������� �������� 
	virtual omsresult GetJSONService(unsigned int aRmmlID, const wchar_t* apwcServiceName);
	// ���������� ���������, ����������� �������� ������ �� SyncManager-�
	virtual bool SetCallbacks(IServiceManCallbacks* apCallbacks);
	// ������� �����
	virtual omsresult ExecMethod(int requestID, unsigned int rmmlID, const wchar_t* apwcJSONMethodDescription);
	// ����������� ����������� ������� �� �������
	virtual omsresult RegisterService(int pendingID, unsigned int rmmlID);
	/*// �������� �� ������, �� ���������� ��������� ��������
	virtual void SendAllNotSendingServiceMessage();*/
	// ������� ������ ����
	virtual void Destroy();

	// ----------------------------------------------------	
	// ���������� �-� ���������� isessionlistener
	// ----------------------------------------------------	
	virtual void OnConnectLost();
	virtual void OnConnectRestored();

// ----------------------------------------------------	
// ���������� ����������� �-� ���������� IServiceManager
// ----------------------------------------------------	
	virtual omsresult SendPreviewServerRequest( unsigned int aiRecordID);
	// �������� ����� Voip �������
	virtual omsresult SendVoipServerRequest( const char* alpcRoomName);
	// �������� ����� Sharing �������
	virtual omsresult SendSharingServerRequest( const char* alpcRoomName);
	// ��������������� ������ ���������� �������
	virtual omsresult SendVoipRecordRegisterRequest( const char* alpcServerIP, const char* alpcRoomName, unsigned int aiRecordID);
	// �������� ��������� ���������� �������, �� ������� ���� ������ aiRecordID � 
	// ��������� ���������� �������, �� ������� ����� ������������� ��� ������
	virtual omsresult SendVoipGetPlayServerRequest( unsigned int aiPlayRealityID,
													unsigned int aiPlayingRecordID);
	// ���������� splashSrc
	virtual void SetSplashSrc( unsigned int aiRecordID, const char* alpcFileName);

	virtual void SendErrorMessageToAdmin(  const wchar_t *apErrorText);

	virtual void SendIPadChangeToken( unsigned int aToken);
	virtual void SendIPadObjectFullState( unsigned int aPacketID, unsigned int aToken, BYTE* aBinState, int aBinStateSize);
	virtual void SendIPadSlideSrcChangedOut( unsigned int aPacketID, unsigned int aToken, float aProportion, float aScaleX, float aScaleY, const wchar_t *aSrc);
	virtual void SendIPadLogoutOut( int aCode);

	//virtual omsresult SendPreviewServersRequest( );
	// �������� ����� Voip �������
	//virtual omsresult SendVoipServersRequest( );

	BEGIN_IN_MESSAGE_MAP()
		IN_MESSAGE2( RT_PreviewServerRequest, OnPreviewServerRequest)
		IN_MESSAGE2( RT_VoipServerRequest, OnVoipServerRequest)
		IN_MESSAGE2( RT_VoipServerForRecordRequest, OnVoipServerForRecordRequest)
		
		IN_MESSAGE2( RT_GetService, OnGetService)
		IN_MESSAGE2( RT_ExecMethodResults, OnExecMethodResults)
		IN_MESSAGE2( RT_GetServiceByMethod, OnGetServiceByMethod)
		IN_MESSAGE2( RT_Notification, OnServiceSendNotify)
		IN_MESSAGE2( RT_ServiceLoadingFailed, OnServiceLoadingFailed)
		IN_MESSAGE2( RT_SharingServerRequest, OnSharingServerRequest)
		/*IN_MESSAGE2( RT_IPadSessionStarted, OnIPadSessionStarted)
		IN_MESSAGE2( RT_IPadSessionStoped, OnIPadSessionStoped)
		IN_MESSAGE2( RT_IPadSessionStatus, OnIPadSessionStatus)*/
		IN_MESSAGE_ONE_OF_RANGE( RT_IPadSessionStarted, OnIPadMessage)
		IN_MESSAGE_ONE_OF_RANGE( RT_IPadSessionStoped, OnIPadMessage)
		IN_MESSAGE_ONE_OF_RANGE( RT_IPadSessionStatus, OnIPadMessage)
		IN_MESSAGE_ONE_OF_RANGE( RT_IPadSessionBinState, OnIPadMessage)
		IN_MESSAGE_ONE_OF_RANGE( RT_IPadSessionChangeSlide, OnIPadMessage)

		//IN_MESSAGE2( RT_PreviewServersRequest, OnPreviewServersRequest)
		//IN_MESSAGE2( RT_VoipServersRequest, OnVoipServersRequest)
	END_IN_MESSAGE_MAP()

protected:

	void OnPreviewServerRequest( BYTE* aData, int aDataSize);
	void OnVoipServerRequest( BYTE* aData, int aDataSize);
	void OnVoipServerForRecordRequest( BYTE* aData, int aDataSize);
	void OnSharingServerRequest( BYTE* aData, int aDataSize);
	void OnIPadMessage( BYTE aType, BYTE* aData, int aDataSize, DWORD aCheckSum);

	CIPadManager *pIPadManager;
};


