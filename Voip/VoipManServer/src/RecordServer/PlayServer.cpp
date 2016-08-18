// VoipMan.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "serverInfo.h"
#include "../../include/RecordServer/PlayServer.h"
#include "../../include/RecordServer/Protocol/StartPlayingQueryOut.h"
#include "../../include/RecordServer/Protocol/PlayAudioFileQueryOut.h"
#include "../../include/RecordServer/Protocol/StartRecordingQueryOut.h"
#include "../../include/RecordServer/Protocol/RemoveUserAudioQueryOut.h"
#include <MMSystem.h>

#define INCORRECT_RECORD_ID	0xFFFFFFFF
/************************************************************************/
// ������� ������ �����  
/************************************************************************/
CPlayServerConnection::CPlayServerConnection(oms::omsContext* aContext, cs::imessagehandler* apHandler, ivoiceconnectionlistender* apConnectionListener)
		: serverConnection( VSST_PLAYING/*play mode*/, aContext, apHandler, this)
		, context( aContext)
{
	realityID = INCORRECT_RECORD_ID;
	recordID = INCORRECT_RECORD_ID;
	playingInProgress = false;
	connectionListener = apConnectionListener;
}

CPlayServerConnection::~CPlayServerConnection()
{
	
}

//! ���������� ��������� ��������� ������
bool CPlayServerConnection::StartPlayingRecord(unsigned int aRecordID, unsigned int aRealityID, const wchar_t* alpcRoomName)
{
	if( playingInProgress)
	{
		// ��������� ��������� � StartPlayingRecord ��� StopPlayingRecord
		ATLASSERT( FALSE);
		return false;
	}
	if( recordID != aRecordID)
	{
		mapCurrentAudioPlayings.clear();
	}

	realityID = aRealityID;
	recordID = aRecordID;
	roomName = alpcRoomName;

	playingInProgress = true;
	return true;
}

//! ������������� ���������������
bool CPlayServerConnection::StopPlayingRecord()
{
	if( !playingInProgress)
	{
		// ������ �������������
		ATLASSERT( FALSE);
		return false;
	}

	//! ������������ ����� CStartPlayingQueryOut - ��� ��� �� ��������
	CStartPlayingQueryOut	queryOut( recordID, realityID);
	serverConnection.Send( ST_PlaySessionStop, queryOut.GetData(), queryOut.GetDataSize());
	
	playingInProgress = false;
	return true;
}

//! ������������ � ���������������� �������
voipError CPlayServerConnection::Connect( LPCTSTR alpcServerIP, unsigned int auiServerPort)
{
	bool savedPlayingInProgress = playingInProgress;

	ATLASSERT( context);
	ATLASSERT( context->mpComMan);

	if( !serverConnection.IsAlreadyConnectedTo(alpcServerIP, auiServerPort))
		serverConnection.AsyncConnect( alpcServerIP, auiServerPort);

	CStartPlayingQueryOut	queryOut( recordID, realityID);
	serverConnection.Send( ST_PlaySessionStart, queryOut.GetData(), queryOut.GetDataSize());

	//! ���� ���������������� � ������� � ���������� ���� �������, �� ��������������� ���
	if( !playingInProgress && savedPlayingInProgress)
	{
		StartPlayingRecord( recordID, realityID, roomName.c_str());
		SetEvoVOIPServer( voipServerIP);
	}

	return ERROR_NOERROR;
}

//! ����������� �� �������
voipError CPlayServerConnection::Disconnect()
{
	if( playingInProgress)
		StopPlayingRecord();

	serverConnection.Disconnect();
	return ERROR_NOERROR;
}

//! ���������� ��������������� � ������ ���������� �������
bool CPlayServerConnection::SetEvoVOIPServer(LPCSTR alpcVoipServerAddress)
{
	voipServerIP = alpcVoipServerAddress;

	if( voipServerIP.IsEmpty())
		return false;

	if( roomName.empty())
		return false;

	USES_CONVERSION;

	CComString sLocationName;
	sLocationName.Format( "%S%u", roomName.c_str(), realityID);
	
	CChangePlayServerQueryOut	queryOut( alpcVoipServerAddress, A2W(sLocationName.GetBuffer()), recordID, realityID);
	serverConnection.Send( ST_ChangePlayServer, queryOut.GetData(), queryOut.GetDataSize());
	return true;
}

//! ��������� ���� �� ���������������
bool CPlayServerConnection::PlayAudioFile(LPCSTR alpcUserName, LPCSTR alpcAudioFile, unsigned int aiStartPosition)
{
	CPlayAudioFileQueryOut	queryOut( alpcUserName, alpcAudioFile, aiStartPosition, recordID, realityID);
	serverConnection.Send( ST_PlayUserAudioFile, queryOut.GetData(), queryOut.GetDataSize());

	RegisterUserPlayEvent( alpcUserName, alpcAudioFile, timeGetTime());

	return true;
}

void CPlayServerConnection::UnregisterUserPlayEvent(LPCSTR alpcUserName)
{
	CMapCurrentAudioPlayingsIt it = mapCurrentAudioPlayings.find( alpcUserName);
	if( it != mapCurrentAudioPlayings.end())
		mapCurrentAudioPlayings.erase( it);
}

void CPlayServerConnection::RegisterUserPlayEvent(LPCSTR alpcUserName, LPCSTR alpcFileName, unsigned long auTime)
{
	UnregisterUserPlayEvent( alpcUserName);

	mapCurrentAudioPlayings.insert( CMapCurrentAudioPlayingsIt::value_type( alpcUserName, alpcFileName));
}

//! ���������� ������ ������, ������� ���� �������� �� ����� ��� CHECKTIME_TOOLONG_DURATION ������ �����
std::vector<std::string>	CPlayServerConnection::GetPlayingFiles()
{
	std::vector<std::string>	playingFiles;

	CMapCurrentAudioPlayingsIt it = mapCurrentAudioPlayings.begin();
	for( ; it != mapCurrentAudioPlayings.end(); it++)
	{
		voip::IVoipCharacter* mpIVoipCharacter = context->mpMapMan->GetIVoipAvatarByName( it->first.c_str());
		if( mpIVoipCharacter != NULL)
		{
			if( mpIVoipCharacter->isSayingAudioFile(it->second.c_str()))
			{
				playingFiles.push_back(it->second);
			}
		}
		it++;
	}

	return playingFiles;
}

//! ��������� ��� ����� ��� ���������� ������������
bool CPlayServerConnection::RemoveUserAudio(LPCSTR alpcUserName)
{
	CRemoveUserAudioQueryOut queryOut( alpcUserName, recordID, realityID);
	serverConnection.Send( ST_RemoveUserAudio, queryOut.GetData(), queryOut.GetDataSize());

	UnregisterUserPlayEvent( alpcUserName);

	return true;
}

//! �������� ������� ������� �� ������������ ��������������� ���� ������������
bool CPlayServerConnection::PauseUserAudio( LPCSTR alpUserName)
{
	CPauseResumeUserAudioQueryOut	queryOut( alpUserName, realityID);
	serverConnection.Send( ST_PauseUserAudio, queryOut.GetData(), queryOut.GetDataSize());
	return true;
}

//! �������� ������� ������� �� ����������� ��������������� ���� ������������
bool CPlayServerConnection::ResumeUserAudio( LPCSTR alpUserName)
{
	CPauseResumeUserAudioQueryOut	queryOut( alpUserName, realityID);
	serverConnection.Send( ST_ResumeUserAudio, queryOut.GetData(), queryOut.GetDataSize());
	return true;
}

//! ���������������� ��������������� ������������ ������
void CPlayServerConnection::PauseServerAudio()
{
	CServerAudioCommandQueryOut	queryOut( recordID, realityID, CServerAudioCommandQueryOut::SAS_PAUSE);
	serverConnection.Send( ST_ServerAudioCommand, queryOut.GetData(), queryOut.GetDataSize());
}

//! ���������� ��������������� ������������ ������
void CPlayServerConnection::ResumeServerAudio()
{
	CServerAudioCommandQueryOut	queryOut( recordID, realityID, CServerAudioCommandQueryOut::SAS_RESUME);
	serverConnection.Send( ST_ServerAudioCommand, queryOut.GetData(), queryOut.GetDataSize());
}

//! ������������ ��������������� ������������ ������
void CPlayServerConnection::RewindServerAudio()
{
	CServerAudioCommandQueryOut	queryOut( recordID, realityID, CServerAudioCommandQueryOut::SAS_REWIND);
	serverConnection.Send( ST_ServerAudioCommand, queryOut.GetData(), queryOut.GetDataSize());
}

void CPlayServerConnection::OnConnectLost()
{
	if( IsPlayingInProgress())
	{
		if( connectionListener)
			connectionListener->OnPlayConnectionLost();
	}
}

void CPlayServerConnection::OnConnectRestored()
{
	if( IsPlayingInProgress())
	{
		CStartPlayingQueryOut	queryOut( recordID, realityID);
		serverConnection.Send( ST_PlaySessionStart, queryOut.GetData(), queryOut.GetDataSize());

		// ������� ����, ����� �� ������������ ������� ���������������
		playingInProgress = false;
		StartPlayingRecord( recordID, realityID, roomName.c_str());
		SetEvoVOIPServer( voipServerIP);
	}
}

void CPlayServerConnection::SetPlayingMode( EPLAYSESIONMODE aePlayMode)
{
	playMode = aePlayMode;
}

//! ������������� ������ ������ ��� ���������������
void CPlayServerConnection::SetAudioFilesInfo( CAudioFilesInfoQueryIn::VecAudioFrames& aAudioFrames, unsigned int auRecordID)
{
	MapAudioFramesIt it = mapAudioFrames.find( auRecordID);
	if( it == mapAudioFrames.end())
	{
		mapAudioFrames.insert( MapAudioFrames::value_type(auRecordID, aAudioFrames));
	}
	if( playingInProgress && playMode == EPM_ONCLIENT && recordID == auRecordID)
	{
		// downloaderAudio.StartDownload( aAudioFrames, auRecordID);
	}
}