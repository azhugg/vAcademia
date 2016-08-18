#pragma once

#include "RecordServerBase.h"

class CSelfRecordServerConnection : public CRecordServerConnectionBase
{
public:
	CSelfRecordServerConnection (oms::omsContext* aContext, cs::imessagehandler* apHandler, ivoiceconnectionlistender* apConnectionListener);
	~CSelfRecordServerConnection ();

	//! ���������� isessionlistener
public:
	void OnConnectLost();
	void OnConnectRestored();

public:
	//! ��������� �� ������� ���������� ������� ��� ������ �����
	bool PrepareSelfRecording( LPCSTR alpcVoipServerAddress, LPCSTR alpcRoomName, LPCSTR alpcUserName);

	//! �������� ������ of self voice
	bool ResumeSelfRecording( );

	//! ������������� ������ of self voice
	bool PauseSelfRecording( );

	//! ������������� ������ of self voice
	bool PlaySelfRecording( );

	//! ����������� ������ ������� ��� ������� ������
	bool StopSelfRecording();

	//! ���������� true - ���� ������ ������� ������ ������ ������ �����
	bool IsRecordingInProgress();

	//! ���������� ����� ��� ������ ������
	unsigned int GetSessionToken()
	{
		return sessionToken;
	}

private:
	//! 
	bool StartRecordingBase( LPCSTR alpcVoipServerAddress, LPCSTR alpcRoomName, LPCSTR alpcUserName, unsigned int aRecordID);

	//!
	unsigned int GenerateNextToken();

protected:
	//! ����� ���������� ������
	CComString		voipServerIP;
	//! ��� �������, ������� �����
	CComString		roomName;
	//! ����� ������, ������� �����
	unsigned int	recordID;
	//! ��� ����� ������������
	CComString		userName;
	//! ����� ������
	unsigned int	sessionToken;

	//! ���� ���������
	bool	recordingInProgress;
};