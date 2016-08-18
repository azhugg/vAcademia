#pragma once

//#ifdef USE_CONAITO_SDK_LIB

#include "VoipWindowClient.h"
#include "../VoipManager.h"
#include "DefaultAudioDeviceManager.h"
#include "../VoipManagerPipe.h"

#include "iclientsession.h"
#include "../Comman/ProxySettings.h"

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

class CVoipManagerConaito : public IVoipManWndEventHandler
							, public CVoipManager
							, public ViskoeThread::CThreadImpl<CVoipManagerConaito>
							, public cm::ichangeconnection
{
	friend class CVoipWindowClient;
	friend class CVoipManagerPipe;

public:
	CVoipManagerConaito(oms::omsContext* aContext);
	~CVoipManagerConaito(void);

	// ���������� voip::IVoipManager
public:
	voipError	ConnectImpl( bool aCheckConnecting);	
	voipError	ChangeLocationImpl( LPCSTR alpcLocation);
	voipError	DisconnectImpl();

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
	//! ���������� ������ ���������, � ������� ����� ������������ ����
	//bool GetInputDevices( wchar_t* alpcDeviceGuids, int aiBufferLength);
	bool GetInputDevices( bool abForce);
	bool GetOutputDevices( bool abForce);
	bool CheckEnableCurrentDevice();
	//! ������������� ����� ��������� ����� � ������ ��������� ���������� - ����������
	bool SetInputDeviceMode( const wchar_t* alpcDeviceGuid, const wchar_t* alpcDeviceLine, int aiToIncludeSoundDevice);
	bool SetInputDeviceMode( const wchar_t* alpcDeviceGuid, const wchar_t* alpcDeviceLine, int aiVasDevice, int aiToIncludeSoundDevice);
	void OnSetInputDeviceMode( std::wstring &aDeviceGuid, std::wstring &aDeviceLine, int aiToIncludeSoundDevice, int aSource);
	//! ������������� ����� ��������� ����� � ����� � ������� ���������� ����� StereoMixer
	bool SetInputMixerMode( const wchar_t* alpcDeviceGuid, const wchar_t* alpcDeviceLine, int aiToIncludeSoundDevice);
	void OnSetInputMixerMode( std::wstring &aDeviceGuid, std::wstring &aMixerName, int aiToIncludeSoundDevice, int aSource);
	//! ������������� ����� ��������� ����� � ����� � ������� �������� ����������� �����-����� VAC4.10
	//void OnSetInputVirtualAudioCableMode( int aiToIncludeSoundDevice);
	//bool SetInputVirtualAudioCableMode( int aiToIncludeSoundDevice);

	bool SetOutputDeviceMode( const wchar_t* alpcDeviceGuid, const wchar_t* alpcDeviceLine, int aiToIncludeSoundDevice);
	void OnSetOutputDeviceMode( std::wstring &aOutDeviceName, std::wstring &aDeviceLine, int aiToIncludeSoundDevice);
	
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
	void CloseVoipManServer();
	void OnCommunicationAreaChanged();
	bool IsMixerSet();
	bool StartFindRealMicrophon();
	bool EndFindRealMicrophon();

// ichangeconnection
public:
	bool IsEqualProxySettings(std::vector<IProxySettings*>* aProxyInfoList1, std::vector<IProxySettings*>* aProxyInfoList2);
	virtual void OnChangeConnectionType(void* aProxyInfo);
	virtual void SubscribeToNotify(ichangeconnection* aSubscriber){};
	virtual void UnSubscribeFromNotify(ichangeconnection* aSubscriber){};

protected:
	virtual void SendConnectRequest( LPCSTR alpcVoipServerAddress, unsigned int aiVoipPort, LPCSTR alpcRecordServerAddress
						, unsigned int aiRecordPort, LPCSTR alpcRoomName);
	virtual void HandleOnConnectRequest();
	//! ������������� ����� �������� ����������
	bool InitializeVoipOnAudioDevice( const wchar_t* alpcDeviceGuid, int aiVasDevice, std::wstring &aDeviceLine, std::wstring &aMixDeviceLine, int aiToIncludeSoundDevice);

protected:
	//! ��������� ���� �������� �������������
	bool IsVoipSystemInitialized();
	//!
	unsigned int GetAudioSubsystemType();
	//! �������� ���� � ������������� ���������� EVOVOIP
	//voipError InitializeEvoSystem();
	//! ������������� ��������� ����� �� ����������	
	//! ��������������� ��������� ����� �� ����������
	void DestroyVoipSystem();
	//! �������� ���� � ��������������� ���������� EVOVOIP
	void DestroyEvoSystem();

	/*//! ��������� ��������������� �������� ���������
	int GetDefaultInputDeviceID();
	int GetDefaultOutputDeviceID();*/

	//int GetInputDeviceID( bool abDefault);
	//int GetOutputDeviceID( bool abDefault);

	//! ��������� ��������������� �������� ���������
	/*int GetCurrentInputDeviceID();
	int GetCurrentOutputDeviceID();*/

	/** ���������� EVO_VOIP ����� ���������� �� ��� �����. � ������, ���� ������ ���������� ��� � ������� - ���������� -1
	*/
	int GetEvoVoipDeviceIDByGuid( const wchar_t* alpcDeviceGuid);

	/** ���������� guid ���������� �� ��� EVO_VOIP ������. � ������, ���� ������ ���������� ��� � ������� - ���������� ""
	*/
	std::wstring GetDeviceGuidByEvoVoidID( int aiDeviceID);
	std::wstring GetDeviceNameByGuid( const wchar_t* alpcDeviceGuid);
	std::wstring GetDeviceGuidByName( const wchar_t* alpcDeviceName, std::vector<SoundDevice> &aInputDevicesInfo);

	//! ��������� ������ ��������� �������� �����
	virtual bool GetVoiceActivated();
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
	//virtual	int CalculateAverageLevel( int nSampleRate, const short* pRawAudio, int nSamples);
	//! ��������� ��������� �������������� ����������� ���������
	virtual void OnAutoVolumeChanged();
	//! ������������� ������� ��������� �� ������������
	virtual bool SetUserVolume( const wchar_t* alwpcUserLogin, double adUserVolume);

	// ����� ��������� ��������� ����� �������������
	virtual bool SetIncomingVoiceVolume( double adUserVolume);
	virtual bool SetEchoCancellation(bool anEnabled);
	virtual bool SetMixerEchoCancellation(bool anEnabled);

	//! ������������� ������� �������� ��������� �� ������������
	virtual bool SetUserGainLevel( const wchar_t* alwpcUserLogin, double adUserGainLevel);
	//! 
	virtual void HandleVoiceData( int nUserID, int nSampleRate, const short* pRawAudio, int nSamples);

	//! ���������� ��� ������������� ���������� � ��������
	LRESULT OnConnectFailed();
	//! ���������� ��� �������� ���������� � ��������
	LRESULT OnConnectSuccess();
	//! ���������� ��� ������ ����� � ��������
	LRESULT OnConnectionLost();
	//! ���������� ��� �������� ���������
	LRESULT OnAccepted(){ return 0;};
	//! ���������� ��� ������ ������������ �� ������
	LRESULT OnLoggedOut(){return 0;};;
	//! ���������� ��� ������ ������� � ������������ ������������
	LRESULT OnKicked();
	//! ���������� ��� ���������� ������ � �������
	LRESULT OnServerUpdate(){return 0;};
	//! ���������� 
	LRESULT OnCommandError(WPARAM wParam){return 0;};
	//! ���������� ��� ��������� ������ ������������ �� �������
	LRESULT OnAddUser(WPARAM wParam, LPARAM lParam);
	//! ���������� ��� ���������� ������ �� ����������� ������������
	LRESULT OnUpdateUser(WPARAM wParam){return 0;};
	//! ���������� ��� �������� ������������ �� �������
	LRESULT OnRemoveUser(WPARAM wParam);
	//! ���������� ��� ��������� �� ������� ������ ������
	LRESULT OnAddChannel(WPARAM wParam){return 0;};
	//! ���������� ��� ��������� ������ ������
	LRESULT OnUpdateChannel(WPARAM wParam){return 0;};
	//! ���������� ��� �������� ������ �� �������
	LRESULT OnRemoveChannel(WPARAM wParam){return 0;};
	//! ���������� ��� �������� ����������� � ������ �� �������
	LRESULT OnChannelJoined(WPARAM wParam, LPARAM lParam);
	//! ���������� ��� �������� ���������� �� ������ �� �������
	LRESULT OnChannelLeft(WPARAM wParam);
	LRESULT OnAddFile(LPARAM lParam){return 0;};
	LRESULT OnRemoveFile(LPARAM lParam){return 0;};
	LRESULT OnFileTransferBegin(WPARAM wParam){return 0;};
	LRESULT OnFileTransferCompleted(WPARAM wParam){return 0;};
	LRESULT OnFileTransferFailed(LPARAM lParam){return 0;};
	LRESULT OnUserMessage(WPARAM wParam, LPARAM lParam){return 0;};
	LRESULT OnChannelMessage(WPARAM wParam, LPARAM lParam){return 0;};
	LRESULT OnUserTalking(WPARAM wParam, LPARAM lParam);
	LRESULT OnUserStoppedTalking(WPARAM wParam, LPARAM lParam);

	//BOOL GetEvoUserImpl( INT32 nUserID, User* lpUser);
	//BOOL GetEvoUserImpl( int aMsgID, INT32 nUserID, User* lpUser);

	//void OnUserTalking( int aUserID, const char *apNickName);
	//void OnUserStoppedTalking( int aUserID, const char *apNickName);
	//BOOL GetEvoUserImpl( int aMsgID, INT32 nUserID, int aUserID, const char *apNickName);	
	//void OnAddUser( int aUserID, const char *apNickName);

	// c�������� ������ 
public:
	voipError InitializeVoipSystem( int aiVasDevice, int aiToIncludeSoundDevice, const wchar_t *alpcDeviceGuid, const wchar_t *alpcOutDeviceName);
	voipError ReInitializeVoipSystem();
	bool OnSendInitVoipSystem( int aErrorCode, bool abInitVoipSystem, int aiVasDevice, int aiToIncludeSoundDevice, std::wstring &aDeviceGuid, std::wstring &aOutDeviceName);
	bool OnGetInputDevices();
	bool OnGetOutputDevices();
	LRESULT OnInitVoipSystem( WPARAM wParam, LPARAM lParam);
	void OnGetMyUserID( int auserID);
	void OnDeviceErrorCode(int anErrorCode);
	void OnFindedMicName(const wchar_t * aDeviceGuid, const wchar_t * aDeviceName, const wchar_t * aDeviceLineName);

	bool IsGetInputDevices();
	voipError OnConnectImpl( unsigned int aErrorCode);

	/*void OnConnectFailed();
	void OnConnectSuccess();
	void OnConnectionLost();*/
	void OnConnectionStatus( int aConnectionStatus);
	//void OnKicked();
	void OnChannelJoined(wchar_t *szChannelPath);
	//void OnChannelLeft();
	//void OnRemoveUser(WPARAM wParam);
	void SetCurrentInputLevel( float aCurrentInputLevel);
private:
	MP_WSTRING				currentEvoVoipDeviceGuid;	
	MP_WSTRING				currentDeviceLine;
	MP_WSTRING				currentMixerGuid;
	MP_WSTRING				currentEvoVoipLineName;
	MP_WSTRING				currentEvoVoipMixLineName;

	MP_WSTRING				currentEvoVoipOutDeviceGuid;
	MP_WSTRING				currentEvoVoipOutDeviceLine;	

	// ��� ����������� ����������� � ������� ���������� ���������� c ������ ���������� �� ���������
	MP_WSTRING				currentMicrophoneDeviceGuid;
	MP_WSTRING				currentMixDeviceGuid;
	//

	CVoipWindowClient					voipWindowClient;
	//LPVOID						clientInstance;
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

	typedef std::vector<SoundDevice> vecDeviceInfo;
	typedef vecDeviceInfo::iterator vecDeviceInfoIt;
	MP_VECTOR<SoundDevice>		inputDevicesInfo;
	MP_VECTOR<SoundDevice>		outputDevicesInfo;

	//! ������� ���������
	double				m_currentInputLevel;
	// ����������� ���������� ��������� ��� ������ (����������� ������� �� ������ �������������)
	double				m_currentModeTopInputLevel;
	// ������������ ��������� �������� ������ (��� ������������). ��� ���������� m_currentModeTopInputLevel - �������� ���������� � ���
	double				m_currentModeCurrentTopInputLevel;

	//! ������� ���������
	double				m_currentOutputLevel;
	// ����������� ���������� ��������� ��� ������ (����������� ������� �� ������ �������������)
	double				m_currentModeTopOutputLevel;
	// ������������ ��������� �������� ������ (��� ������������). ��� ���������� m_currentModeTopInputLevel - �������� ���������� � ���
	double				m_currentModeCurrentTopOutputLevel;

	bool				m_skipCurrentLevelCalculation;
	CVoipManagerPipe	*m_voipManagerPipe;
	int m_userID;
	bool bVoipSystemInitialized;
	bool bVoipSettingCorrect;
	CMutex mutexCurrentInputLevel;
	MP_WSTRING oldCurrentDeviceName;
	MP_WSTRING oldDeviceLine;
	MP_WSTRING oldMixerName;

	MP_WSTRING oldCurrentOutDeviceName;
	MP_WSTRING oldCurrentEvoVoipOutDeviceLine;
	bool	   outputDeviceWasChanged;


public:
	void SetInputDevices( vecDeviceInfo *apInputDevicesInfo);
	void SetOutputDevices( vecDeviceInfo *apOutputDevicesInfo);
	void CorrectOldSettings();
	void SetVoipSystemInitialized( bool abVoipSystemInitialized);
	void SendMessageToWnd( unsigned int auMSG, unsigned int data1 = 0, const wchar_t *data2 = NULL);
	oms::omsContext *GetOMSContext();
	void UpdateVoipConnection();	

	void CheckAudioDevices();

private:
	bool LogSystemDrivers();
	std::string ReadRegKey(std::string path, std::string key);
	std::wstring TryGetCurrentInDeviceGuid( bool aMicrophone/*, SoundDevice &sndDevice*/);
	bool DeviceIsEqual( std::wstring aDevice1, std::wstring aDevice2);
	bool DeviceIsIntoArrayName( std::wstring aDevice1, CWTMicContoller::CDevicesNamesVector &aDeviceGuidArray);

	void StartCheckAudioDevices();

	bool isSubscribedToNotify;
	HANDLE m_hCheckAudioDevicesThread;
	DWORD m_checkAudioDevicesThreadId;
	volatile bool m_bCheckAudioDevices;
	HANDLE m_eventCheckAudioDevices;
	CDefaultAudioDeviceManager *m_pDefaultAudioDeviceManager;
	CMutex mutexAudioDevice;
};

//#endif USE_CONAITO_SDK_LIB