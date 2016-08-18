#pragma once

#include <map>
#include <vector>
#include "Mutex.h"
#include "EvoSettings.h"
#include "../../Comman/ProxySettings.h"
#include "DeviceList/WTMicContoller.h"

class CMP3Encoder;

class CVoip3DSoundCalcManager;
class CDataBuffer2;

#define MAX_STABLE_AUDIO_LEVEL_VOICE	3000
#define MAX_STABLE_AUDIO_LEVEL_SHARING	15000

struct ProxyInfo;

struct IVoipManWndEventHandlerEVO
{
	virtual void HandleOnConnectRequest() = 0;

	//! ���������� ��� ������������� ���������� � ��������
	virtual LRESULT OnConnectFailed() = 0;
	//! ���������� ��� �������� ���������� � ��������
	virtual LRESULT OnConnectSuccess() = 0;
	//! ���������� ��� ������ ����� � ��������
	virtual LRESULT OnConnectionLost() = 0;
	//! ���������� ��� �������� ���������
	virtual LRESULT OnAccepted() = 0;
	//! ���������� ��� �������� ���������
	virtual LRESULT OnAcceptedFromVoip() = 0;
	//! ���������� ��� ������ ������������ �� ������
	virtual LRESULT OnLoggedOut() = 0;
	//! ���������� ��� ������ ������� � ������������ ������������
	virtual LRESULT OnKicked() = 0;
	//! ���������� ��� ���������� ������ � �������
	virtual LRESULT OnServerUpdate() = 0;
	//! ���������� 
	virtual LRESULT OnCommandError(WPARAM wParam) = 0;
	//! ���������� ��� ��������� ������ ������������ �� �������
	virtual LRESULT OnAddUser(WPARAM wParam) = 0;
	//! ���������� ��� ���������� ������ �� ����������� ������������
	virtual LRESULT OnUpdateUser(WPARAM wParam) = 0;
	//! ���������� ��� �������� ������������ �� �������
	virtual LRESULT OnRemoveUser(WPARAM wParam) = 0;
	//! ���������� ��� ��������� �� ������� ������ ������
	virtual LRESULT OnAddChannel(WPARAM wParam) = 0;
	//! ���������� ��� ��������� ������ ������
	virtual LRESULT OnUpdateChannel(WPARAM wParam) = 0;
	//! ���������� ��� �������� ������ �� �������
	virtual LRESULT OnRemoveChannel(WPARAM wParam) = 0;
	//! ���������� ��� �������� ����������� � ������ �� �������
	virtual LRESULT OnChannelJoined(WPARAM wParam) = 0;
	//! ���������� ��� �������� ���������� �� ������ �� �������
	virtual LRESULT OnChannelLeft(WPARAM wParam) = 0;
	virtual LRESULT OnAddFile(LPARAM lParam) = 0;
	virtual LRESULT OnRemoveFile(LPARAM lParam) = 0;
	virtual LRESULT OnFileTransferBegin(WPARAM wParam) = 0;
	virtual LRESULT OnFileTransferCompleted(WPARAM wParam) = 0;
	virtual LRESULT OnFileTransferFailed(LPARAM lParam) = 0;
	virtual LRESULT OnUserMessage(WPARAM wParam, LPARAM lParam) = 0;
	virtual LRESULT OnChannelMessage(WPARAM wParam, LPARAM lParam) = 0;
	virtual LRESULT OnUserTalking(WPARAM wParam) = 0;
	virtual LRESULT OnUserStoppedTalking(WPARAM wParam) = 0;
	virtual LRESULT OnInvalideChannelName(WPARAM wParam) = 0;
	virtual LRESULT OnMyUserIDRecieved(WPARAM wParam) = 0;
	virtual LRESULT OnLogMumbleErrorCode(WPARAM wParam, LPARAM lParam) = 0;
	//virtual LRESULT ProcessCommandById(WPARAM wParam, LPARAM lParam) = 0;
};

struct UserVolume
{
	std::wstring name;
	long volume;
};

class CVoipManagerEVO /*: public IVoipManager
	, public cs::imessagehandler
	, public ivoiceconnectionlistender*/
{
public:
	CVoipManagerEVO(oms::omsContext* aContext);
	~CVoipManagerEVO(void);

	//! ��������� ���������
public:
	/*BEGIN_IN_MESSAGE_MAP()		
		IN_MESSAGE2( RT_RecordStartResult, OnRecordStartResult)
		IN_MESSAGE2( RT_RecordFinished, OnRecordFinished)
		IN_MESSAGE2( RT_AudioPlayed, OnAudioPlayed)
		IN_MESSAGE2( RT_MixedAudioCreated, OnMixedAudioCreated)
		IN_MESSAGE2( RT_PlayFinished, OnPlayFinished)
		IN_MESSAGE2( RT_PlaySessionConnected, OnPlaySessionConnected)
		IN_MESSAGE2( RT_AudioFilesInfo, OnAudioFilesInfo)
		IN_MESSAGE2( RT_EvoClientWorkStateChanged, OnEvoClientWorkStateChanged)
		IN_MESSAGE2( RT_Notified, OnNotified)
	END_IN_MESSAGE_MAP()*/

	// ���������� voip::IVoipManager
public:
	void OnRecordConnectionLost(unsigned int auErrorCode);
	void OnRecordConnectionRestored();

	void OnPlayConnectionLost();
	void OnPlayConnectionRestored();

	void OnSelfRecordingConnectionLost();
	void OnSelfRecordingConnectionRestored();

	// ���������� voip::IVoipManager
public:

	//! ������������ � ������ ������� ��������� �����
	virtual voipError	Connect( LPCSTR alpcGroupName, LPCSTR alpcUserName, LPCSTR alpcPassword, long long alAvatarID);

	//! ��������������� ����������� � �������� ������� ��������� �����
	voipError	RestoreConnection();

	//! ����������� �� �������� ������� ��������� �����
	//voipError	Disconnect();

	//! ������� ���������� ���� � �������
	voipError	CreateServerTalkingBot( unsigned int aClientSideObjectID);

	//! ������� ���������� ���� � �������
	voipError	DestroyServerTalkingBot( unsigned int aClientSideObjectID);

	//! ������� �������� ��������� �����
	void Release();

	//! ������������ �� DirectSound ��� ��������������� �����
	//! �������� �� ���������: 1
	void SetDirectSoundUsed( bool val);

	//! ������� �������� ������� � ���������. �������� �������� 100-4000. 1000 - �������� ���. 4000 - x4, 100-x0.1
	//! �������� �� ���������: 1000
	void SetMicGainLevel(float val);

	//! ������� ������ �������� �������. �������� �������� 0-20. 
	//! 0 - ������ ���������� ���������, 10 - ������ ���������� ������ ���� �� ������� ��� 50% ���������, 
	//! 20 - ������ ������������ �� �����
	//! �������� �� ���������: 5
	void SetMicActivationLevel(float val);

	//! ������� ������������� ��� ������. ��������: 8000, 16000, 32000
	//! �������� �� ���������: 8000
	void SetRecorderFreq(unsigned int val);

	//! �������� �����. �������� �������� 1-10. 1 - ����������� ��������. 10 - ������������ ��������
	//! �������� �� ���������: 4
	void SetVoiceQuality(float val);

	//! ��������� ���������� ������ hands-free
	virtual void SetVoiceActivated(bool enabled);

	//! �������������� ������ �������� ������
	virtual void StartTransmitting(){}

	//! �������������� ��������� �������� ������
	virtual void StopTransmitting(){}

	//! ���������� ��������� ����� ������� ������
	virtual bool OnBeforeStartRecording();

	//! ������������� ������� ��������� �� ������������
	virtual bool SetUserVolume( const wchar_t* alpwcUserLogin, double adUserVolume);

	//! ������������� ������� �������� ��������� �� ������������
	virtual bool SetUserGainLevel( const wchar_t* alwpcUserLogin, double adUserGainLevel);

	//! ��������� ���������� �����
	virtual bool IsDirectSoundUsed();
	virtual float GetMicGainLevel();
	virtual float GetMicActivationLevel();
	virtual float GetVoiceQuality();
	virtual unsigned int GetRecorderFreq();
	virtual float GetCurrentInputLevel(){ return 0;}
	// ���������� ������� ��������� ������������� ����� � ���������
	//virtual float GetCurrentInputLevelNormalized(){ return GetMicrophoneEnabled() ? GetCurrentVoiceInputLevelNormalized() : GetCurrentMicInputLevelNormalized();}
	// ���������� ��������� ������ ������ � ������ ���������� �������� � ������
	virtual float GetCurrentVoiceInputLevelNormalized(){ return 0;}	
	// ���������� ������� ��������� ������������� ����� � ��������� (waveIn)
	//virtual float GetCurrentMicInputLevelNormalized();
	//GetMicrophoneEnabled
	virtual int GetLogMode(){ return m_iLogMode;};
	virtual bool GetVoiceActivated(){ return voiceActivated;}
	virtual bool GetMicrophoneEnabled(){ return false;}

	//! ���������� ��� ��������� ����� - ����� ��� ���� � �����
	voipAudioSource GetAudioSource();
	//! ������������� ��� ��������� �����
	bool SetAudioSource(voipAudioSource aeAudioSource);
	//! ���������� ����� ��������� ���������. �������������� ��� ������
	bool GetAutoVolume();
	//! ������������� ����� ��������� ���������. �������������� ��� ������
	void SetAutoVolume(bool abAuto);
	
	//virtual bool SetInputVirtualAudioCableMode() = 0;

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
	bool RebuildIfEmptyDevicesList(){return false;};

	//! ���� � ����� ������������� ������ ������
	/*
	���������� ��������� ������:
	SR_NOT_AVAILABLE_DURING_RECORDING
	NOTINITIALIZED
	NOERROR
	*/
	virtual int EnterSelfRecordingMode();
	//! ������ ������ ������ ������
	/*
	NOERROR
	*/
	/*virtual int StartSelfRec();
	//! ���������� ������ ������ ������
	virtual int StopSelfRec();
	//! ������������� ���������� �����
	virtual int PlaySelfRec();
	//! ����� �� ������ ������������� ������ ������
	virtual int ExitSelfRecordingMode();*/

	//! � �������� ���������� ���� �������� �������������
	virtual bool IsVoipSystemInitialized(){ return false;}


protected:	

	/*virtual void SendConnectRequest( LPCSTR alpcVoipServerAddress, LPCSTR alpcRecordServerAddress
		, unsigned int aiRecordPort, LPCSTR alpcRoomName)
	{
		ATLASSERT( FALSE);
	}*/
	virtual void HandleOnConnectRequest(){}

protected:
	//! �������� ����� ������������� ������ ������
	//int AbortSelfRecordingMode();
	//! ���������� ��� ��������� � �����
	void OnChannelJoined(const char* alpcChannelName);
	//! ������������ � ������ ����� ������ �����
	voipError ConnectToSelfVoiceChannel();

protected:
	//! ������� ����������� ����������� �����������
	virtual voipError	ConnectImpl(){ return ERROR_NOERROR;}
	virtual voipError	DisconnectImpl(){ return ERROR_NOERROR;}
	//virtual voipError	ChangeLocationImpl( LPCSTR alpcLocation){ return ERROR_NOERROR;}

	//! 
	virtual void HandleVoiceData( int nUserID, int nSampleRate, const short* pRawAudio, int nSamples);
	virtual bool SetUserPosition( int nUserID, float x, float y, float z);
	//void SetEvoVoipClientWorkState(voip::voipEvoClientSessionType aSessionType, voip::voipEvoClientWorkState aWorkState, unsigned int auError);

	//! ���������� ����� ��������� �������
	virtual	void OnDirectSoundUsedChanged() = 0;
	virtual	void OnGainLevelChanged() = 0;
	virtual	void OnActivationLevelChanged() = 0;
	virtual	void OnVoiceActivatedChanged() = 0;
	virtual	void OnVoiceQualityChanged() = 0;
	//! ��������� ��������� �������������� ����������� ���������
	virtual void OnAutoVolumeChanged() = 0;

	int GetUserIDByName( LPCWSTR apwsName);
	LPCTSTR GetUserNameByID( int aiAvatarID);
	long GetUserVolume( int aiAvatarID);
	bool SaveUserVolume( int aiAvatarID, long adUserVolume);
#if VOIPMAN_BUILD_L
	CVoip3DSoundCalcManager *GetVoip3DSoundManagerID( int aiAvatarID);
#endif
	//!
	oms::omsContext* context;

	//! ��������� ����������
	bool  mUseDS;
	float micGainLevel;
	float micActivationLevel;
	float voiceQuality;
	int   m_iLogMode;
	bool  voiceActivated;
	unsigned int recFreq;
	CONNECTION_STATUS		connectionStatus;
	//MP_WSTRING	currentInDeviceGuid;
	//MP_WSTRING	currentOutDeviceGuid;
	bool				autoVolume;
	voipAudioSource		audioSource;

	typedef MP_MAP<int, UserVolume> MapVoipUsers;

#if VOIPMAN_BUILD_L
	//typedef	std::map<int/* voipID*/, CVoip3DSoundCalcManager* /* mp3Encoder*/> MapVoip3DSoundCalcManagerID;	
	typedef MP_MAP<int,CVoip3DSoundCalcManager*> MapVoip3DSoundCalcManagerID;
#endif

	void SetSRModeState( voip::voipSRModeStatusChanged	aState);
	void Destroy();

	MapVoipUsers	mapVoipUsers;
	CMutex			mutexAvatars;
#if VOIPMAN_BUILD_L
	MapVoip3DSoundCalcManagerID mapVoip3DSoundCalcManagerID;
#endif
	//CMutex			mutexLog;
	BOOL			bTerminating;

	//! Callbacks
	/*typedef	std::vector<voipIVoipManCallbacks*>	VecCallbacks;
	VecCallbacks	vecCallbacks;*/

	int					myUserID;
	CComString			serverIP;
	//CComString			roomName;
	CComString			userName;
	CComString			userPassword;
	std::vector<ProxyInfo> proxySettingsList;	

	//! ��������� ��������������� ����������
	/*CComString			requestedRoomName;
	CComString			requestedUserName;
	CComString			requestedUserPassword;
	long long			requestedAvatarID;*/

	//! ��������� �������, � ������� ������ ���� ���������������
	//bool				recordIsOnGoing;
	//CDataBuffer2*		m_pSndData2lex;

	//! ����� ������ ������ ������
	//voip::voipSRModeStatusChanged		selfRecordingIsOnGoing;
	//bool								selfRecordingChannelConnected;

	//! ��������� �������������� ���������� �� ��� ��� ������ � ������ SR
	CComString			inSRModeRequestedRoomName;
	CComString			inSRModeRequestedUserName;
	CComString			inSRModeRequestedUserPassword;
	long long			inSRModeRequestedAvatarID;


#if VOIPMAN_BUILD_L
	IVoip3DSoundCalculator	*m_pUser3DPositioning;
#endif
public:
};