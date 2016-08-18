#pragma once

#include <map>
#include <vector>
#include "Mutex.h"
#include "RecordServer/RecordServer.h"
#include "RecordServer/PlayServer.h"
#include "RecordServer/SelfRecordServer.h"
#include "messagetypes.h"
#include "ConnectionListener.h"
#include "../DeviceList/WTMicContoller.h"
#include "EvoSettings.h"
#include "..\ComMan\ProxySettings.h"

class CMP3Encoder;
class CVoip3DSoundCalcManager;
class CDataBuffer2;

#define	CALLBACK_NO_PARAMS(x)	\
	{	\
		csVecCallbacks.Lock();								\
		VecCallbacks::iterator it = vecCallbacks.begin(),	\
		end = vecCallbacks.end();							\
		for (; it!=end; ++it)								\
		(*it)->##x();										\
		csVecCallbacks.Unlock();							\
	}

#define	CALLBACK_ONE_PARAM(x,p)	\
	{	\
		csVecCallbacks.Lock();								\
		VecCallbacks::iterator it = vecCallbacks.begin(),	\
		end = vecCallbacks.end();							\
		for (; it!=end; ++it)								\
		(*it)->##x(p);										\
		csVecCallbacks.Unlock();							\
	}

#define	CALLBACK_TWO_PARAMS(x,p,m)	\
	{	\
		csVecCallbacks.Lock();								\
		VecCallbacks::iterator it = vecCallbacks.begin(),	\
		end = vecCallbacks.end();							\
		for (; it!=end; ++it)								\
		(*it)->##x(p,m);									\
		csVecCallbacks.Unlock();							\
	}

#define	CALLBACK_THREE_PARAMS(x,p,m,k)	\
	{	\
		csVecCallbacks.Lock();								\
		VecCallbacks::iterator it = vecCallbacks.begin(),	\
		end = vecCallbacks.end();							\
		for (; it!=end; ++it)								\
		(*it)->##x(p,m,k);									\
		csVecCallbacks.Unlock();							\
	}

#define MAX_STABLE_AUDIO_LEVEL_VOICE	3000
#define MAX_STABLE_AUDIO_LEVEL_SHARING	15000
#define SKIP_REALITY_ID 0xFFFFFFFF


struct IVoipManWndEventHandler
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
	//! ���������� ��� ������ ������������ �� ������
	virtual LRESULT OnLoggedOut() = 0;
	//! ���������� ��� ������ ������� � ������������ ������������
	virtual LRESULT OnKicked() = 0;
	//! ���������� ��� ���������� ������ � �������
	virtual LRESULT OnServerUpdate() = 0;
	//! ���������� 
	virtual LRESULT OnCommandError(WPARAM wParam) = 0;
	//! ���������� ��� ��������� ������ ������������ �� �������
	virtual LRESULT OnAddUser(WPARAM wParam, LPARAM lParam) = 0;
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
	virtual LRESULT OnChannelJoined(WPARAM wParam, LPARAM lParam) = 0;
	//! ���������� ��� �������� ���������� �� ������ �� �������
	virtual LRESULT OnChannelLeft(WPARAM wParam) = 0;
	virtual LRESULT OnAddFile(LPARAM lParam) = 0;
	virtual LRESULT OnRemoveFile(LPARAM lParam) = 0;
	virtual LRESULT OnFileTransferBegin(WPARAM wParam) = 0;
	virtual LRESULT OnFileTransferCompleted(WPARAM wParam) = 0;
	virtual LRESULT OnFileTransferFailed(LPARAM lParam) = 0;
	virtual LRESULT OnUserMessage(WPARAM wParam, LPARAM lParam) = 0;
	virtual LRESULT OnChannelMessage(WPARAM wParam, LPARAM lParam) = 0;
	virtual LRESULT OnUserTalking(WPARAM wParam, LPARAM lParam) = 0;
	virtual LRESULT OnUserStoppedTalking(WPARAM wParam, LPARAM lParam) = 0;
	virtual LRESULT OnInitVoipSystem(WPARAM wParam, LPARAM lParam) = 0;
};


class CVoipManager : public IVoipManager
					, public cs::imessagehandler
					, public ivoiceconnectionlistender
{
public:
	CVoipManager(oms::omsContext* aContext);
	~CVoipManager(void);

	//! ��������� ���������
public:
	BEGIN_IN_MESSAGE_MAP()
		IN_MESSAGE2( RT_VoipFileCreated, OnFileCreated)
		IN_MESSAGE2( RT_RecordStartResult, OnRecordStartResult)
		IN_MESSAGE2( RT_RecordFinished, OnRecordFinished)
		IN_MESSAGE2( RT_AudioPlayed, OnAudioPlayed)
		IN_MESSAGE2( RT_MixedAudioCreated, OnMixedAudioCreated)
		IN_MESSAGE2( RT_PlayFinished, OnPlayFinished)
		IN_MESSAGE2( RT_PlaySessionConnected, OnPlaySessionConnected)
		IN_MESSAGE2( RT_AudioFilesInfo, OnAudioFilesInfo)
		IN_MESSAGE2( RT_EvoClientWorkStateChanged, OnEvoClientWorkStateChanged)
		IN_MESSAGE2( RT_Notified, OnNotified)
	END_IN_MESSAGE_MAP()

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

	//! ������������� ��������� ����������� � ������� ��� ��������������� ������������ ������
	virtual bool	SetVoipPlayServerAddress(unsigned int aiRecordID
											, LPCSTR alpcVoipRecordServerAddress, unsigned int aiRecordPort
											, LPCSTR alpcVoipPlayServerAddress, unsigned int aiPlayPort);

	//! ������������� ��������� ��������� ����������� � ������������ �������
	virtual bool	SetVoipRecordServerAddress(LPCSTR alpcVoipServerAddress, unsigned int aiVoipPort, LPCSTR alpcRecordServerAddress
											, unsigned int aiRecordPort, LPCSTR alpcRoomName);

	//! ������������ � ������ ������� ��������� �����
	virtual voipError	Connect( LPCSTR alpcLocationName, unsigned int auRealityID, LPCSTR alpcUserName, LPCSTR alpcPassword, long long alAvatarID);

	//! ��������������� ����������� � �������� ������� ��������� �����
	voipError	RestoreConnection();

	//! ����������� �� �������� ������� ��������� �����
	voipError	Disconnect();

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

	//! ��������� ���������� ��������������� �������
	virtual bool AddCallback( voipIVoipManCallbacks* apCallback);

	//! ������� ���������� ��������������� �������
	virtual bool RemoveCallback( voipIVoipManCallbacks* apCallback);

	//! ���������� ��������� ����� ������� ������
	virtual bool OnBeforeStartRecording();

	//! �������� ������ ������� ��� ������� ������
	virtual bool StartRecording( unsigned int aRecordID, LPCSTR alpcLocationName, unsigned int auRealityID);

	//! ����������� ������ ������� ��� ������� ������
	virtual bool StopRecording(unsigned int aRecordID);

	//! �������� �������� ������� ������ � ��������� ������ �����
	virtual bool StartListenRecording( unsigned int aRecordID);

	//! ����������� �������� ������� ������ � ��������� ������ �����
	virtual bool StopListenRecording( unsigned int aRecordID);

	// ������� ������ (�������� ������ �� ������)
	virtual bool DeleteRecord(unsigned int aRecordID);

	// ������� ������ (�������� ������ �� ������)
	virtual bool DeleteRecords( unsigned int *pRecIDs, unsigned int count);

	//! �������� ��������������� ��������� ������ ��� ��������� ����������
	virtual bool StartPlayingRecord(unsigned int aRecordID, unsigned int aRealityID, const wchar_t* apwcLocationID);

	//! ����������� ��������������� ������������ ������
	virtual bool StopPlayingRecord();

	//! �������� ������� ������� �� ��������������� ����������� �����
	virtual bool StartUserAudioPlayback( LPCSTR alpUserName, LPCSTR alpFileName, LPCSTR alpcCommunicationAreaName, unsigned int aiStartPosition = 0, unsigned int auSyncVersion = 0);

	//! �������� ������� ������� �� ��������� ����������� �����
	virtual bool StopUserAudioPlayback( LPCSTR alpUserName);

	//! �������� ������� ������� �� ������������ ��������������� ���� ������������
	virtual bool PauseUserAudio( LPCSTR alpUserName);

	//! �������� ������� ������� �� ����������� ��������������� ���� ������������
	virtual bool ResumeUserAudio( LPCSTR alpUserName);

	//! ������������� ������� ��������� �� ������������
	virtual bool SetUserVolume( const wchar_t* alpwcUserLogin, double adUserVolume);

	// ����� ��������� ��������� ����� �������������
	virtual bool SetIncomingVoiceVolume( double adUserVolume);
	virtual bool SetEchoCancellation(bool anEnabled);
	virtual bool SetMixerEchoCancellation(bool anEnabled);

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
	virtual float GetCurrentInputLevelNormalized(){ return GetMicrophoneEnabled() ? GetCurrentVoiceInputLevelNormalized() : GetCurrentMicInputLevelNormalized();}
	// ���������� ��������� ������ ������ � ������ ���������� �������� � ������
	virtual float GetCurrentVoiceInputLevelNormalized(){ return 0;}	
	// ���������� ������� ��������� ������������� ����� � ��������� (waveIn)
	virtual float GetCurrentMicInputLevelNormalized();
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

	virtual bool StartMicRecording();
	virtual bool StopMicRecording( LPWSTR *fileName, unsigned int aSize);

	//virtual bool GetInputDevices( wchar_t* alpcDeviceNames, int aiBufferLength) = 0;
	virtual bool GetInputDevices( bool abForce) = 0;
	virtual bool SetInputDeviceMode( const wchar_t* alpcDeviceName, const wchar_t* alpcDeviceLine, int aiToIncludeSoundDevice) = 0;
	virtual bool SetOutputDeviceMode( const wchar_t* alpcDeviceName, const wchar_t* alpcDeviceLine, int aiToIncludeSoundDevice) = 0;
	virtual bool SetInputMixerMode( const wchar_t* alpcDeviceName, const wchar_t* alpcDeviceLine, int aiToIncludeSoundDevice) = 0;
	//virtual bool SetInputVirtualAudioCableMode( int aiToIncludeSoundDevice) = 0;

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
	virtual int StartSelfRec();
	//! ���������� ������ ������ ������
	virtual int StopSelfRec();
	//! ������������� ���������� �����
	virtual int PlaySelfRec();
	//! ����� �� ������ ������������� ������ ������
	virtual int ExitSelfRecordingMode();

	virtual void CloseVoipManServer(){};
	virtual void OnCommunicationAreaChanged(){};
	virtual bool IsMixerSet(){ return false;};
	virtual bool StartFindRealMicrophon(){ return false;}
	virtual bool EndFindRealMicrophon(){ return false;}

protected:

	virtual void SendConnectRequest( LPCSTR alpcVoipServerAddress, unsigned int aiVoipPort, LPCSTR alpcRecordServerAddress
								, unsigned int aiRecordPort, LPCSTR alpcRoomName)
	{
		ATLASSERT( FALSE);
	}
	virtual void HandleOnConnectRequest(){}

	//! ������������ � ������������� ������� ��������� �����
	voipError	ConnectToVoipServer( LPCSTR alpcServer, unsigned int auiVoipServerPort, LPCSTR alpcGroupName
		, LPCSTR alpcUserName, LPCSTR alpcPassword, long long alAvatarID
		, LPCSTR alpcRecordServer, unsigned int auiRecordServerPort);

	void	OnConnectToEVoipServer();

protected:
	//! �������� ����� ������������� ������ ������
	int AbortSelfRecordingMode();
	//! ���������� ��� ��������� � �����
	void OnChannelJoined(const char* alpcChannelName);
	//! ������������ � ������ ����� ������ �����
	voipError ConnectToSelfVoiceChannel();
	//! ��������� �� ������� ���������� ������� ��� ������ �����
	bool PrepareSelfRecording( );
	//! �������� ���������� ������ ������������ ������
	bool ResumeSelfRecording( );
	//! ������������� ������ ������������ ������
	bool StopSelfRecording( );
	//! �������� ���������� ������ � ��������������� aRecordID �� ������� �������
	bool StartRecordingImpl( unsigned int aRecordID, LPCSTR alpcLocationName, unsigned int auRealityID);
	//! ������������� ������ ��������������� aRecordID �� ������� �������
	bool StopRecordingImpl(unsigned int aRecordID );
	//! ���������� ��� ����������� ������, ������� � ������� ������ ������� ��������������� �������
	void CopyCurrentPlayingFiles();
	//! ���������� ����� ��� �� �� ���������� �������� �������� � �� ��������� ������� ��������� ����
	void OnFileCreated( BYTE* aData, int aDataSize);
	//! ����������, ����� ������ �������� ������� ������ ����
	void OnRecordStartResult( BYTE* aData, int aDataSize);
	//! ����������, ����� ������� ������ �� ������� �����������
	void OnRecordFinished( BYTE* aData, int aDataSize);
	//! ����������, ����� �� ������� ������������� ������������� �����
	void OnAudioPlayed( BYTE* aData, int aDataSize);
	//! ���������������� ��������������� ������������ ������
	virtual void PauseServerAudio();
	//! ���������� ��������������� ������������ ������
	virtual void ResumeServerAudio();
	//! ������������ ��������������� ������������ ������
	virtual void RewindServerAudio();
	//! ����� ������������ ��������� ������������� ����
	virtual void OnMixedAudioCreated( BYTE* aData, int aDataSize);
	//! ����������, ����� �� ������������ � ������ ���������������
	void OnPlaySessionConnected( BYTE* aData, int aDataSize);
	//! ����������, ����� ��������������� ������ �� ������� �����������
	void OnPlayFinished( BYTE* aData, int aDataSize);
	//! ����������, ����� �� �������� ������ ������ �� ������
	void OnAudioFilesInfo( BYTE* aData, int aDataSize);
	//! ����������, ����� ��������� ������ ���������� EvoVOIP �������
	void OnEvoClientWorkStateChanged( BYTE* aData, int aDataSize);
	//! � �������� ���������� ���� �������� �������������
	virtual bool IsVoipSystemInitialized(){ return false;};
	//! �������� ����������� �� �������
	void OnNotified( BYTE* aData, int aDataSize);

protected:
	//! ������� ����������� ����������� �����������
	virtual voipError	ConnectImpl( bool aCheckConnecting){ return ERROR_NOERROR;}
	virtual voipError	DisconnectImpl(){ return ERROR_NOERROR;}
	virtual voipError	ChangeLocationImpl( LPCSTR alpcLocation){ return ERROR_NOERROR;}
	
	//! 
	virtual void HandleVoiceData( int nUserID, int nSampleRate, const short* pRawAudio, int nSamples);
	virtual bool SetUserPosition( int nUserID, float x, float y, float z);
	void SetEvoVoipClientWorkState(voip::voipEvoClientSessionType aSessionType, voip::voipEvoClientWorkState aWorkState, unsigned int auError);

	//! ���������� ����� ��������� �������
	virtual	void OnDirectSoundUsedChanged() = 0;
	virtual	void OnGainLevelChanged() = 0;
	virtual	void OnActivationLevelChanged() = 0;
	virtual	void OnVoiceActivatedChanged() = 0;
	virtual	void OnVoiceQualityChanged() = 0;
	//! ��������� ��������� �������������� ����������� ���������
	virtual void OnAutoVolumeChanged() = 0;
	//
	void WriteLogLine(LPCSTR alpString);

	//int GetUserIDByName( LPCTSTR alpcName);
	//LPCTSTR GetUserNameByID( int aiAvatarID);
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
	CRecordServerConnection*		recordServerConnection;
	CPlayServerConnection*		playServerConnection;
	CSelfRecordServerConnection*	selfRecordingServerConnection;
	//MP_WSTRING	currentInDeviceName;
	//MP_WSTRING	currentOutDeviceName;
	bool				autoVolume;
	voipAudioSource		audioSource;
	voipAudioSource		wantAudioSource;

	//! ������������ ��������������� RMML, ����� � VOICE
	//typedef	std::map<int/* voipID*/, CComString /* userName*/> MapVoipIDName;
	//typedef	std::map<int/* voipID*/, CMP3Encoder* /* mp3Encoder*/> MapMP3LogIDName;
	//typedef MP_MAP<int,CComString > MapVoipIDName;
	//typedef MP_MAP<int,CMP3Encoder* > MapMP3LogIDName;

#if VOIPMAN_BUILD_L
	//typedef	std::map<int/* voipID*/, CVoip3DSoundCalcManager* /* mp3Encoder*/> MapVoip3DSoundCalcManagerID;	
	typedef MP_MAP<int,CVoip3DSoundCalcManager* > MapVoip3DSoundCalcManagerID;
#endif

	void SetSRModeState( voip::voipSRModeStatusChanged	aState);
	void RegisterVoipAvatar( int aiAvatarID, const CComString& asName);
	void UnregisterVoipAvatar( int aiAvatarID);
	void Destroy();
	CComString	EvaluateVoiceChannelName( LPCSTR alpcLocationName, unsigned int auRealityID);

	//MapVoipIDName	mapVoipIDName;

#if VOIPMAN_BUILD_L
	MapVoip3DSoundCalcManagerID mapVoip3DSoundCalcManagerID;
#endif
	CMutex			mutexLog;
	BOOL			bTerminating;

	//! Callbacks
	//typedef	std::vector<voipIVoipManCallbacks*>	VecCallbacks;
	typedef MP_VECTOR<voipIVoipManCallbacks*> VecCallbacks;
	VecCallbacks	vecCallbacks;

	int					myUserID;
	CComString			serverIP;
	unsigned int		voipServerPort;
	CComString			roomName;
	CComString			userName;
	CComString			userPassword;

	CMutex				changeProxyListMutex;
	MP_VECTOR<IProxySettings*> httpsProxySettingsVec;
	MP_VECTOR<IProxySettings*> socksProxySettingsVec;


	//! ��������� ��������������� ����������
	CComString			requestedRoomName;
	CComString			requestedRealityID;
	CComString			requestedUserName;
	CComString			requestedUserPassword;
	long long			requestedAvatarID;

	//! ��������� �������, � ������� ������ ���� ���������������
	bool				recordIsOnGoing;
	CDataBuffer2*		m_pSndData2lex;

	//! ����� ������ ������ ������
	voip::voipSRModeStatusChanged		selfRecordingIsOnGoing;
	bool								selfRecordingChannelConnected;

	//! ��������� �������������� ���������� �� ��� ��� ������ � ������ SR
	CComString			inSRModeRequestedRoomName;
	CComString			inSRModeRequestedUserName;
	CComString			inSRModeRequestedUserPassword;
	long long			inSRModeRequestedAvatarID;


#if VOIPMAN_BUILD_L
	IVoip3DSoundCalculator	*m_pUser3DPositioning;
#endif

	CComString sRecordServer;
	unsigned int uiRecordServerPort;

private:
	void SetRecordState(unsigned int aRecordID, bool aRecording, bool anAuthor);

	unsigned int m_currentRecordID;
protected:
	CCriticalSection csVecCallbacks;
};