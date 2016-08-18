#pragma once

#include "imessagehandler.h"
#include "iclientsession.h"
#include "../ConnectionListener.h"
#include "serverInfo.h"
#include "../messagetypes.h"
#include "../AsyncConnection.h"
#include "Protocol/PlaySessionConnectedQueryIn.h"
#include "Protocol/AudioFilesInfoQueryIn.h"
#include <map>

class CPlayServerConnection : cs::isessionlistener
{
public:
	CPlayServerConnection(oms::omsContext* aContext, cs::imessagehandler* apHandler, ivoiceconnectionlistender* apConnectionListener);
	~CPlayServerConnection();

	//! ���������� isessionlistener
public:
	void OnConnectLost();
	void OnConnectRestored();

public:
	//! ���������� ��������� ��������� ������
	virtual bool StartPlayingRecord(unsigned int aRecordID, unsigned int aRealityID, const wchar_t* alpcRoomName);

	//! ������������� ���������������
	virtual bool StopPlayingRecord();

	//! ������������ � �������
	voipError	Connect( LPCTSTR alpcServerIP, unsigned int auiServerPort);

	//! ����������� �� �������
	voipError	Disconnect();

	//! ���������� ��������������� � ������ ���������� �������
	bool SetEvoVOIPServer(LPCSTR alpcVoipServerAddress);

	//! ��������� ���� �� ���������������
	bool PlayAudioFile(LPCSTR alpcUserName, LPCSTR alpcAudioFile, unsigned int aiStartPosition, unsigned int auSyncVersion, LPCSTR alpcCommunicationAreaName);

	//! ��������� ��� ����� ��� ���������� ������������
	bool RemoveUserAudio(LPCSTR alpcUserName);

	//! �������� ������� ������� �� ������������ ��������������� ���� ������������
	virtual bool PauseUserAudio( LPCSTR alpUserName);

	//! �������� ������� ������� �� ����������� ��������������� ���� ������������
	virtual bool ResumeUserAudio( LPCSTR alpUserName);

	//! ���������������� ��������������� ������������ ������
	void PauseServerAudio();

	//! ���������� ��������������� ������������ ������
	void ResumeServerAudio();

	//! ������������ ��������������� ������������ ������
	void RewindServerAudio();

	//! ����������� ������ ������ ��� ���
	bool IsPlayingInProgress()
	{
		return playingInProgress;
	}

	//! ������������� ����� ��������������� ������: �� ������� ��� �� �������
	void SetPlayingMode( EPLAYSESIONMODE aePlayMode);

	//! ������������� ������ ������ ��� ���������������
	void SetAudioFilesInfo( CAudioFilesInfoQueryIn::VecAudioFrames& aAudioFrames, unsigned int auRecordID);

	//! ����� ������, ������� �������������
	unsigned int GetRecordID()
	{
		return recordID;
	}
	
	//! ����� ����������, � ������� �������������
	unsigned int GetRealityID()
	{
		return realityID;
	}

	//! ���������� ������ ������, ������� ���� �������� �� ����� ��� CHECKTIME_TOOLONG_DURATION ������ �����
	std::vector<std::string>	GetPlayingFiles();

protected:
	void UnregisterUserPlayEvent(LPCSTR alpcUserName);
	void RegisterUserPlayEvent(LPCSTR alpcUserName, LPCSTR alpcFileName, unsigned long auTime);

protected:
	CAsyncConnection* serverConnection;

	//! ����� �������� ���������� ������� ���������������
	CComString		voipServerIP;
	//! ��� �������, � ������� �������������
	MP_WSTRING	roomName;
	//! ����� ������, ������� �������������
	unsigned int	recordID;
	//! ����� ����������, � ������� �������������
	unsigned int	realityID;
	//! ���� ���������
	bool	playingInProgress;
	//! ���� ���������������
	//typedef std::map<std::string/*user name*/, std::string/*file name*/>	CMapCurrentAudioPlayings;
	typedef MP_MAP<MP_STRING,MP_STRING> CMapCurrentAudioPlayings;
	typedef CMapCurrentAudioPlayings::iterator			CMapCurrentAudioPlayingsIt;
	CMapCurrentAudioPlayings	mapCurrentAudioPlayings;

	//! ��������, ��� ��� ��� ������� :)
	oms::omsContext* context;

	//! ��������� ������ �����
	ivoiceconnectionlistender* connectionListener;

	//! ������������� ����� ��������������� ������: �� ������� ��� �� �������
	EPLAYSESIONMODE playMode;

	//typedef std::map<unsigned int, CAudioFilesInfoQueryIn::VecAudioFrames>	MapAudioFrames;
	typedef MP_MAP<int,CAudioFilesInfoQueryIn::VecAudioFrames> MapAudioFrames;
	typedef MapAudioFrames::iterator MapAudioFramesIt;
	MapAudioFrames	mapAudioFrames;
};