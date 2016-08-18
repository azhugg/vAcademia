#pragma once


#include "VoipWindow.h"
#include "../VoipManager.h"
//#include "AudioCaptureManager.h"
#include "../VoipManagerPipeEVO.h"
#include "../../../Common/ViskoeThread.h"
#include "SimpleThreadStarter.h"
#include "proxyInfo.h"


#ifndef USE_CONAITO_SDK_LIB
	using namespace api;
	
#endif USE_CONAITO_SDK_LIB

/* ������� �����������:
1. VOIP_InitVoIP
2. VOIP_LaunchSoundSystem
3. VOIP_Connect -> OnConnectionSuccess / OnConnectFailed
4. VOIP_DoLogin -> OnAccepted -> OnAddChannel
4. VOIP_DoJoinChannel -> OnAddChannel -> OnAddUser
*/

/*
��������� �������� �����������:
1. ServerName
2. UserName
3. Password
4. RoomName
5. 
*/

class CAudioCapture;

class CVoipManagerConaitoServer : public IVoipManWndEventHandlerEVO
	, public CVoipManagerEVO
	, public ViskoeThread::CThreadImpl<CVoipManagerConaitoServer>
{
	friend class CVoipWindow;
	friend class CVoipManagerPipeEVO;
	//friend void UserTalking(int nUserID, int nSampleRate, const short* pRawAudio, int nSamples, LPVOID pUserData);

public:
	CVoipManagerConaitoServer(oms::omsContext* aContext);
	~CVoipManagerConaitoServer(void);

	LPVOID GetClientInstance();
	CComString GetChannelPath();

public:
	DWORD	Run();

	// ���������� voip::IVoipManager
public:
	voipError	ConnectImpl( bool aCheckConnecting);	

	//! ������� �������� ��������� �����
	void Release();
	//! �������������� ������ �������� ������
	virtual void StartTransmitting();
	//! �������������� ��������� �������� ������
	virtual void StopTransmitting();
	//! ���������� ��������� ������ ������
	virtual float GetCurrentInputLevel();	
	//! ���������� ��������� ������ ������ � ������ ���������� �������� � ������
	virtual float GetCurrentVoiceInputLevelNormalized();
	//! ������������� ����� ��������� ����� � ����� � ������� �������� ����������� �����-����� VAC4.10
	//bool SetInputVirtualAudioCableMode();
	//! �������������� ���������� ����������
	virtual bool IsAutoGaneControlEnabled();
	virtual bool SetAutoGaneControlEnabled(bool abEnabled);
	//! �������������� ���������� �����
	virtual bool IsDenoisingEnabled();
	virtual bool SetDenoisingEnabled(bool abEnabled);
	// ��� ��������� ��������. ����� ������� ���:
	/*
	VoipMan ������ ������������ ��������� ������ ������������ ���������. � ������ ���� �� ���������
	�������� � ������ �� ����. ��� ���� ���� ������� ���������� ������� - ���� �� ���� �������.
	���� ������ ���� ������ - � ��� � ������������, ���� ������� ���������� �������� �������.
	*/
	bool RebuildIfEmptyDevicesList();

	bool SetUserPosition( int nUserID, float x, float y, float z);

protected:
	/*virtual void SendConnectRequest( LPCSTR alpcVoipServerAddress, LPCSTR alpcRecordServerAddress
		, unsigned int aiRecordPort, LPCSTR alpcRoomName);*/
	virtual void HandleOnConnectRequest();
	//! ������������� ����� �������� ����������
	bool InitializeVoipOnAudioDevice( const wchar_t* alpcDeviceName);

	// ������� ������
	//public:
	//void HandleOnConnectRequestImpl();
	//private:
	//static int HandleOnConnectRequestThread( CVoipManagerConaito *aThis);

	// ����� �����	
public:
	//! ������������� ��������� ����� �� ����������
	voipError InitializeVoipSystem( const wchar_t* alpcDeviceGuid, const wchar_t* alpcOutDeviceGuid);
	void SetVoiceGainLevel( float aVal);
	void SetVoiceActivationLevel( float aVal);
	void SetServerParams( std::string &aServerIP, int aTcpPort, int anUdpPort, std::vector<ProxyInfo> aProxyInfoList);
	void JoinChannel( std::string &aRoomName);

	//! ��������� ���� �������� �������������
	bool IsVoipSystemInitialized();

	//! ��������� ��������������� �������� ���������
	int GetCurrentInputDeviceID( EVO_WRAPPER_SoundSystemType soundSystem);
	int GetCurrentOutputDeviceID( EVO_WRAPPER_SoundSystemType soundSystem);
	virtual bool GetVoiceActivated();
	const wchar_t *GetCurrentEvoVoipDeviceGuid();
	const wchar_t *GetCurrentEvoVoipOutDeviceGuid();
protected:
	//!
	//unsigned int GetAudioSubsystemType();
	//! �������� ���� � ������������� ���������� EVOVOIP
	voipError InitializeEvoSystem();	
	//! ��������������� ��������� ����� �� ����������
	void DestroyVoipSystem(bool aNeedDisconnect=true);
	//! �������� ���� � ��������������� ���������� EVOVOIP
	void DestroyEvoSystem();

	//! ���������� �� �������� ������
	void DisconnectCurrentChannel();

	//! ��������� ��������������� �������� ���������
	int GetDefaultInputDeviceID( EVO_WRAPPER_SoundSystemType soundSystem);
	int GetDefaultOutputDeviceID( EVO_WRAPPER_SoundSystemType soundSystem);

	int GetInputDeviceID( EVO_WRAPPER_SoundSystemType soundSystem, bool abDefault);
	int GetOutputDeviceID( EVO_WRAPPER_SoundSystemType soundSystem, bool abDefault);

	/** ���������� EVO_VOIP ����� ���������� �� ��� �����. � ������, ���� ������ ���������� ��� � ������� - ���������� -1
	*/
	int GetEvoVoipDeviceIDByGuid( const wchar_t* alpcDeviceGuid);
	int GetEvoVoipOutDeviceIDByGuid( const wchar_t* alpcDeviceGuid);

	/** ���������� ��� ���������� �� ��� EVO_VOIP ������. � ������, ���� ������ ���������� ��� � ������� - ���������� ""
	*/
	std::wstring GetDeviceGuidByEvoVoidID( int aiDeviceID);

	//! ��������� ������ ��������� �������� �����	
	virtual bool GetMicrophoneEnabled();

	// ��������� �������
protected:
	void HandleTransmissionStateChanged();
	//! ���������� ����� ��������� �������
	virtual	void OnDirectSoundUsedChanged();
	virtual	void OnGainLevelChanged();
	virtual	void OnActivationLevelChanged();
	virtual	void OnVoiceActivatedChanged();
	virtual	void OnVoiceQualityChanged();
	virtual	int CalculateAverageLevel( int nSampleRate, const short* pRawAudio, int nSamples);
	bool GrabDevicesInfoImpl(EVO_WRAPPER_SoundSystemType soundSystem, bool abForce);	
	bool GrabInputDevicesInfo(EVO_WRAPPER_SoundSystemType soundSystem, bool abForce);
	bool GrabOutputDevicesInfo(EVO_WRAPPER_SoundSystemType soundSystem, bool abForce);
	//! ��������� ��������� �������������� ����������� ���������
	virtual void OnAutoVolumeChanged();
	//! ������������� ������� ��������� �� ������������
	virtual bool SetUserVolume( const wchar_t* alwpcUserLogin, double adUserVolume);

	void SetMasterVolume(float aVolume);
	void SetEchoCancellation(bool anEnabled);
	void SetMixerEchoCancellation(bool anEnabled);
	void AppStartFindMicDevice();
	void AppEndFindMicDevice();

	//! ������������� ������� �������� ��������� �� ������������
	//virtual bool SetUserGainLevel( const wchar_t* alwpcUserLogin, double adUserGainLevel);	
	//! 
public:
	virtual void HandleVoiceData( int nUserID, int nSampleRate, const short* pRawAudio, int nSamples);
	bool GrabDevicesInfo(bool abForce);
	bool GrabInputDevicesInfoForce( bool abForce);
	bool GrabOutputDevicesInfoForce( bool abForce);
protected:

	//! ���������� ��� ������������� ���������� � ��������
	LRESULT OnConnectFailed();
	//! ���������� ��� �������� ���������� � ��������
	LRESULT OnConnectSuccess();
	//! ���������� ��� ������ ����� � ��������
	LRESULT OnConnectionLost();
	//! ���������� ��� �������� ���������
	LRESULT OnAccepted();
	//! ���������� ��� �������� ��������� �� e�� ��� �����
	LRESULT OnAcceptedFromVoip();
	//! ���������� ��� ������ ������������ �� ������
	LRESULT OnLoggedOut();
	//! ���������� ��� ������ ������� � ������������ ������������
	LRESULT OnKicked();
	//! ���������� ��� ���������� ������ � �������
	LRESULT OnServerUpdate();
	//! ���������� 
	LRESULT OnCommandError(WPARAM wParam);
	//! ���������� ��� ��������� �� ������ ������������
	LRESULT OnMyUserIDRecieved(WPARAM wParam);
	//! ���������� ��� ��������� ������ ������������ �� �������
	LRESULT OnAddUser(WPARAM wParam);
	//! ���������� ��� ���������� ������ �� ����������� ������������
	LRESULT OnUpdateUser(WPARAM wParam);
	//! ���������� ��� �������� ������������ �� �������
	LRESULT OnRemoveUser(WPARAM wParam);
	//! ���������� ��� ��������� �� ������� ������ ������
	LRESULT OnAddChannel(WPARAM wParam);
	//! ���������� ��� ��������� ������ ������
	LRESULT OnUpdateChannel(WPARAM wParam);
	//! ���������� ��� �������� ������ �� �������
	LRESULT OnRemoveChannel(WPARAM wParam);
	//! ���������� ��� �������� ����������� � ������ �� �������
	LRESULT OnChannelJoined(WPARAM wParam);
	//! ���������� ��� �������� ���������� �� ������ �� �������
	LRESULT OnChannelLeft(WPARAM wParam);
	LRESULT OnAddFile(LPARAM lParam);
	LRESULT OnRemoveFile(LPARAM lParam);
	LRESULT OnFileTransferBegin(WPARAM wParam);
	LRESULT OnFileTransferCompleted(WPARAM wParam);
	LRESULT OnFileTransferFailed(LPARAM lParam);
	LRESULT OnUserMessage(WPARAM wParam, LPARAM lParam);
	LRESULT OnChannelMessage(WPARAM wParam, LPARAM lParam);
	LRESULT OnUserTalking(WPARAM wParam);
	LRESULT OnUserStoppedTalking(WPARAM wParam);
	LRESULT OnInvalideChannelName(WPARAM wParam);
	LRESULT OnLogMumbleErrorCode(WPARAM wParam, LPARAM lParam);
	//BOOL GetEvoUserImpl(INT32 nUserID, User* lpUser);

	// ��������� � ������� � �� ���������
public:
	void OnStartTransmitting();
	void OnStopTransmitting();
	bool OnHandleTransmissionStateChanged( bool aMicrophoneEnabled, bool aVoiceActivated);
	int GetMyUserID();

	//! ������������� ������� �������� ��������� �� ������������
	virtual bool SetUserGainLevel(  const wchar_t* alwpcUserLogin, double adUserGainLevel);
	voipError	DisconnectImpl();

	BOOL GetEvoUserImpl(INT32 nUserID, User* lpUser);

	//! ���������� ��� �������� ���������� � ��������
	void DoLogin( const wchar_t *aUserName, const wchar_t *aUserPassword, const wchar_t *aRoomName);

private:
	MP_WSTRING				currentEvoVoipDeviceGuid;
	//std::wstring				currentEvoVoipLineName;	
	MP_WSTRING				currentEvoVoipOutDeviceGuid;
	
	CVoipWindow					voipWindow;
	LPVOID						clientInstance;
	CONNECTION_STATUS			channelConnectionStatus;
	bool						microphoneEnabled;
	ConnectRequestParams		connectRequestParams;
	CEvoSettings				settings;
	bool						isLoggedIn;

	CEvent						initializeCompleteEvent;
	voipError					initializeErrorCode;

	CMutex						mutexConnect;
	int							audioCapDeviceID;
	bool						agcEnabled;
	bool						denoisingEnabled;

	MP_VECTOR<SoundDevice>		inputDevicesInfo;
	MP_VECTOR<SoundDevice>		outputDevicesInfo;

	//! ������� ���������
	double				m_currentInputLevel;
	// ����������� ���������� ��������� ��� ������ (����������� ������� �� ������ �������������)
	double				m_currentModeTopInputLevel;
	// ������������ ��������� �������� ������ (��� ������������). ��� ���������� m_currentModeTopInputLevel - �������� ���������� � ���
	double				m_currentModeCurrentTopInputLevel;
	bool				m_skipCurrentLevelCalculation;

public:
	void GetInputDevicesInfo( vecDeviceInfo **aInputDevicesInfo);
	void GetOutputDevicesInfo( vecDeviceInfo **aOutputDevicesInfo);	
	void SetManagerPipe( CVoipManagerPipeEVO *apVoipManagerPipe);
	void Initialize( bool abTwoApplication);
	int GetConnectionStatus();
	void WriteLogLine(const char* alpString);
	void WriteLogLine(const wchar_t* alpwString);
	void VoipLog();
	void ChannelThread();

protected:
	int tcpPort; 
	int udpPort;
	CComString roomName;
	CVoipManagerPipeEVO *m_voipManagerPipe;
	CComString channelPath;
	bool bNeedLogin;
	bool bLaunchedSoundSystem;
	bool bTwoApplication;
	CMutex mutex;	
	
	void WriteLogLineImpl(const wchar_t* alpwString);

private: 
	void ResetDirectConnectFlags();
	bool ConnectToNextInProxyList();
	void StartVoipLog();
	void StartChannelThread();

	EVO_WRAPPER_SoundSystemType m_mainSoundSystem;

	bool m_directConnectFailed;
	bool m_directConnectSuccess;
	bool m_directConnectNow;
	unsigned int m_currentProxySettingsListIndex;

	CMutex m_proxySettingsMutex;

	HANDLE m_hVoipLogThread;
	DWORD m_voipLogThreadId;
	volatile bool m_bVoipLog;
	HANDLE m_eventVoipLog;

	HANDLE m_hChannelThread;
	DWORD m_channelThreadId;
	volatile bool m_bChannel;
	HANDLE m_eventChannel;
	int tempID;
	//int wParamIDChannel;
	ProxyInfo httpsProxyInfo;
};

