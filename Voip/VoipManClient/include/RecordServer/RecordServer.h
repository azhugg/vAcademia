#pragma once

#include "RecordServerBase.h"

class CRecordServerConnection : public CRecordServerConnectionBase
{
public:
	CRecordServerConnection(oms::omsContext* aContext, cs::imessagehandler* apHandler, ivoiceconnectionlistender* apConnectionListener);
	~CRecordServerConnection();

	//! ���������� isessionlistener
public:
	void OnConnectLost();
	void OnConnectRestored();

public:
	//! �������� ���� � ������ alpcFileName �� ����� ������ auSourceRecordID � ����� ������� ������
	virtual bool CopyAudioFile( LPCTSTR alpcFileName, unsigned int auSourceRecordID);

	//! ������� �� ������ ���� (����)
	bool IsRecordingInProgress();

	//! ������� �� ������ ���� (�� �� ����)
	bool IsRecordListeningInProgress();

	//! �������� ������ ������� ��� ������� ������
	bool StartRecording( LPCSTR alpcVoipServerAddress, LPCSTR alpcRoomName, LPCSTR alpcUserName, unsigned int aRecordID);

	//! ����������� ������ ������� ��� ������� ������
	bool StopRecording(unsigned int aRecordID);

	//! �������� �������� ������� ������ � ��������� ������ �����
	bool StartListenRecording( unsigned int aRecordID);

	//! ����������� �������� ������� ������ � ��������� ������ �����
	bool StopListenRecording( unsigned int aRecordID);

	//! ������� ������ (�������� ������ �� ������)
	bool DeleteRecord(unsigned int aRecordID);

	//! ������� ������ (�������� ������ �� ������)
	bool DeleteRecords( unsigned int *pRecIDs, unsigned int count);

	//! ��������� ������ � ����� �����. ��� ���� ������� ������ �����������, ��
	//! ����� � ������������ �������� �� �����������. ��� ����������� ������
	//! ������ ����� ������� �� ������� ������������ �������.
	bool PauseRecording();

	//! ��������� ������ �������� � ������� ������ �� ������� ������������ �������.
	//! ��� ���� ������ ������ ������������ � ������ ���������� �������
	bool ResumeRecording(LPCSTR alpcVoipServerAddress, LPCSTR alpcRoomName);

	//! ���������� ����� ������, ������� �� ������ �����
	unsigned int	GetRecordID(){ return recordID;}

	//! ���������� ����� ������, ������� �� ������ �������
	unsigned int	GetListenRecordID(){ return listenRecordID;}

	//! �������� ������� �������������
	bool ForceSynchronization( int uiUserID);

protected:
	//! ���������� ����� ���������� � ��������
	virtual void OnAfterConnected();

private:
	//! 
	bool StartRecordingBase( LPCSTR alpcVoipServerAddress, LPCSTR alpcRoomName, LPCSTR alpcUserName, unsigned int aRecordID);

protected:
	//! ����� ���������� ������
	CComString		voipServerIP;
	//! ��� �������, ������� �����
	CComString		roomName;
	//! ����� ������, ������� �����
	unsigned int	recordID;
	//! ��� ����� ������������
	CComString		userName;

	//! ����� ������, ������� ����� ���-��, � �� �������� ������ �������
	unsigned int	listenRecordID;

	//! ���� ���������
	bool	recordingInProgress;
	bool	recordListeningInProgress;
};