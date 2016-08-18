#ifndef __SERVICEMAN_H_
#define __SERVICEMAN_H_

#ifdef SERVICEMAN_EXPORTS
#define SERVICEMAN_API __declspec(dllexport)
#else
#define SERVICEMAN_API __declspec(dllimport)
#endif

#ifdef WIN32
#define SERVICEMAN_NO_VTABLE __declspec(novtable)
#else
#define SERVICEMAN_NO_VTABLE
#endif

#include "oms_context.h"
#include "rmml.h"

// ---------------------
// ����������� ���������
// ---------------------

#define RT_GetService					1  // �������� JSON �������� �������
#define RT_ExecMethodResults  			2  // ��������� ���������� ������
#define RT_GetServiceByMethod			3  // �������� JSON �������� ������� �� ������
#define RT_Notification					4  // ����������� � �������
#define RT_ServiceLoadingFailed			7 // ������ �������� �������
#define RT_PreviewServerRequest			5 // ��������� ����� preview �������
#define RT_VoipServerRequest			6 // ��������� ����� voip �������
#define RT_VoipServerForRecordRequest	8 // ��������� ����� voip ������� �� ������ ������
#define RT_SharingServerRequest			9 // ��������� ����� sharing �������

#define RT_IPadSessionStarted			10 // ������ ������ �� IPad
#define RT_IPadSessionStoped			11 // ������� ������ �� IPad
#define RT_IPadSessionStatus			12 // ������ ������ �� IPad
#define RT_IPadSessionBinState			13 // �������� ��������� �������
#define RT_IPadSessionChangeSlide		14 // �������� �����

//#define RT_PreviewServersRequest		10 // ��������� ���� preview ��������
//#define RT_VoipServersRequest			11 // ��������� ���� voip ��������

// -------------------------------------
// Send Type - ���� ���������� ���������
// -------------------------------------
#define ST_GetService					1  // �������� � ������� JSON ��������� �������
#define ST_ExecMethod					2  // ��������� �����
#define ST_RegisterService				3  // ����������� ����������� ������� �� �������
#define ST_PreviewServerRequest			4 // ��������� ����� Preview ������ 
#define ST_VoipServerRequest			5 // ��������� ����� Voip ������ 
#define ST_RegisterRecord				6 // ���������������� ������ �� �������
// �������� #define ST_UnregisterRecord				7 // ������� ������ � ������ 
#define ST_VoipServerForRecordRequest	8 // �������� ���� � ������� �� ������� ����� ������������� ����� ������
#define ST_SetSplashSrc					9 // ���������� splashSrc
#define ST_SharingServerRequest			11 // ��������� ����� Sharing ������ 
#define ST_SendErrorMessageToAdmin		12
#define ST_SendIPadChangeToken			13
#define ST_SendIPadObjectFullState		14
#define ST_SendIPadSlideSrcChangedOut	15
#define ST_SendIPadLogoutOut			16

//#define ST_PreviewServersRequest		12 // ��������� ����� Preview ������ 
//#define ST_VoipServersRequest			13 // ������� �������������� ��������� 

namespace service
{

	// Received Type - ���� ����������� ���������

	struct SERVICEMAN_NO_VTABLE IServiceManCallbacks
	{
		// �������� json ���������� � �������
		virtual void onGetService(unsigned int rmmlID, const wchar_t* apwcServiceName, const wchar_t* apwcServiceDescription, int* apiErrorCode=NULL ) = 0;
		
		// ������� ����� �� ������
		virtual void onMethodComplete(int aRequestID, int aRMMLID, const wchar_t* apwcResponse=NULL, bool isService=false, int aiErrorCode=0) = 0;
		virtual void onServiceNotify(int aiID, const wchar_t* apwcResponse, int* apiErrorCode) = 0;
		virtual void onConnectRestored() = 0;
	};

	struct SERVICEMAN_NO_VTABLE IBaseServiceManager
	{
		// ���������� � ������� ��������
		virtual void Connect() = 0;
		// ��������� �� ������� ��������
		virtual void Disconnect() = 0;
		// ������� �������� �������������
		virtual void Release() = 0;
		// �������� �������� 
		virtual omsresult GetJSONService(unsigned int aRmmlID, const wchar_t* apwcServiceName) = 0;
		// ������� �����
		virtual omsresult ExecMethod(int requestID, unsigned int rmmlID, const wchar_t* apwcJSONMethodDescription);
		// ���������� ���������, ����������� �������� ������ �� SyncManager-�
		virtual bool SetCallbacks(IServiceManCallbacks* apCallbacks)=0;
		// ����������� ����������� ������� �� �������
		virtual omsresult RegisterService(int pendingID, unsigned int rmmlID)=0;
		/*// �������� �� ������, �� ���������� ��������� ��������
		virtual void SendAllNotSendingServiceMessage()=0;*/
		// ������� ������ ����
		virtual void Destroy() = 0;
	};

	struct SERVICEMAN_NO_VTABLE IServiceManager : public IBaseServiceManager
	{
		// �������� ����� Preview �������
		virtual omsresult SendPreviewServerRequest( unsigned int aiRecordID)=0;
		// �������� ����� Voip �������
		virtual omsresult SendVoipServerRequest( const char* alpcRoomName)=0;
		// �������� ����� Sharing �������
		virtual omsresult SendSharingServerRequest( const char* alpcRoomName)=0;
		// ��������������� ������ ���������� �������
		virtual omsresult SendVoipRecordRegisterRequest( const char* alpcServerIP, const char* alpcRoomName, unsigned int aiRecordID)=0;
		// �������� ��������� ���������� �������, �� ������� ���� ������ aiRecordID � 
		// ��������� ���������� �������, �� ������� ����� ������������� ��� ������
		virtual omsresult SendVoipGetPlayServerRequest( unsigned int aiPlayRealityID,
														unsigned int aiPlayingRecordID)=0;
		// ���������� splashSrc
		virtual void SetSplashSrc( unsigned int aiRecordID, const char* alpcFileName)=0;

		virtual void SendErrorMessageToAdmin( const wchar_t *apErrorText)=0;

		virtual void SendIPadChangeToken( unsigned int aToken)=0;
		virtual void SendIPadObjectFullState(  unsigned int aPacketID, unsigned int aToken, BYTE* aBinState, int aBinStateSize)=0;
		virtual void SendIPadSlideSrcChangedOut( unsigned int aPacketID, unsigned int aToken, float aProportion, float aScaleX, float aScaleY, const wchar_t *aSrc)=0;
		virtual void SendIPadLogoutOut( int aCode)=0;

		// �������� ����� Preview �������
		//virtual omsresult SendPreviewServersRequest()=0;
		// �������� ����� Voip �������
		//virtual omsresult SendVoipServersRequest()=0;
	};

	struct SERVICEMAN_NO_VTABLE IIPadManager
	{
		virtual void StartSession(  wchar_t *aName, unsigned int aObjectID, unsigned int aBornRealityID, unsigned int aRealityID, void *aImgObject) = 0;
		virtual void DestroySession() = 0;
		virtual void OnIPadMessage( BYTE aType, BYTE* aData, int aDataSize) = 0;
		virtual bool IsIPadObject( wchar_t *aName, unsigned int aObjectID, unsigned int aBornRealityID, unsigned int aRealityID);		
		virtual void SendForceFullState() = 0;
		virtual void SetIPadImageParam( const wchar_t *apSrc, float aProportion, float aScaleX, float aScaleY) = 0;
		virtual void OnConnectRestored() = 0;
		virtual void Logout() = 0;
		virtual void UpdateIPadObjects() = 0;
	};

	enum E_SESSION_STATE_TYPES
	{ 
		CSS_CLIENT_WORK_STATE = 0,
		CSS_DEADLOCKS_COUNT,
		CSS_CRASH_WINDOW_SHOWN,
		CSS_EXIT_CODE
	};

	enum E_DUMP_SENDING_STATE
	{
		DUMP_SENDING_STARTING = 0,
		DUMP_SENDED,
		DUMP_NO_SENDED,
		DUMP_ERROR,
	};

#define COPY_DATA_CODE_PASSWORD	3
#define COPY_DATA_CODE_LOGIN	4
#define COPY_DATA_CODE_COMMENT	5
#define COPY_DATA_CODE_VERSION	6
#define COPY_DATA_VJS			7
#define COPY_DATA_EXIT			8

	struct SERVICEMAN_NO_VTABLE IServiceWorldManager: public IBaseServiceManager 
	{
		virtual int Login( const wchar_t* apwcLogin, const wchar_t* apwcPassword, bool isEncodingPassword, const wchar_t* apwcEncodingPassword, const wchar_t* apwcComputerJeyCode, bool isAnonymous, int requestID, int source, const wchar_t* apwcAuthKey, const wchar_t* apwcUpdaterKey, const wchar_t* apwcBuildType) = 0;
		virtual int Logout( int iCodeError) = 0;
		virtual int LoginAsync( int requestID, const wchar_t* apwcLogin, const wchar_t* apwcPassword, bool isAnonymous, int source = -1, bool isEncodingPassword = false, const wchar_t* apwcAuthKey = NULL, const wchar_t* apwcUpdaterKey = NULL) = 0;
		virtual const wchar_t* GetClientStringID()=0;
		virtual bool IsExitCodeReceived()=0;
		virtual bool IsDebuggerPresent()=0;

		/** �������� ������� ������ ���������� ������ ������������ �� ������ 
		\param unsigned int auState - ������������� ���������
		CSS_ - client session state
		CSS_CLIENT_WORK_STATE = 0 - ���� ��������
		CSS_DEADLOCKS_COUNT = 1 - ���������� ����������
		CSS_CRASH_WINDOW_SHOWN = 2 - ���������� ���� �����
		CSS_CLOSE_CODE = 3 - ���������� �����

		\param unsigned int auValue - �������� ���������
		1 ���� - ����� ����� ��������
		2 ���� - ���������� ����������
		3 ���� - ���� ����� / ���������� ����
		4 ���� - ���� ����������� �������� ������
		\return void
		\details
		*/
		virtual void SetSessionStateValue( E_SESSION_STATE_TYPES auState, unsigned short auValue, const wchar_t* apwcComment = NULL)=0;
		virtual void SendConnectionRestoredStatus()=0;
		virtual int SendMDumpParams( BYTE* aData, int aDataSize) = 0;
		virtual int SendMDumpFile( BYTE* aData, int aDataSize, int aDataTotalSize) = 0;
		virtual int IsDumpSendedCode() = 0;
		virtual void SetWndHandle(HWND ahWnd) = 0;
	};


	SERVICEMAN_API omsresult CreateServiceManager( oms::omsContext* aContext);
	SERVICEMAN_API void DestroyServiceManager( oms::omsContext* aContext);
	SERVICEMAN_API omsresult CreateServiceWorldManager( oms::omsContext* aContext);
	SERVICEMAN_API void DestroyServiceWorldManager( oms::omsContext* aContext);
}
#endif
