#pragma once

#include "imessagehandler.h"
#include "iclientsession.h"
#include "serverInfo.h"
#include "../../../Common/ViskoeThread.h"
#include "messagesStorage.h"

class CAsyncConnection : public ViskoeThread::CThreadImpl<CAsyncConnection>
{
public:
	CAsyncConnection(	voip::voipServerSessionType aSessionType
						, oms::omsContext* aContext
						, cs::imessagehandler* apHandler
						, cs::isessionlistener* apSessionListener);
	~CAsyncConnection();

	//! ���������� CThreadImpl
public:
	DWORD Run();

public:
	//! ������������ � �������
	bool	AsyncConnect( LPCTSTR alpcServerIP, unsigned int auiServerPort);

	//! ����������� �� �������
	voipError	Disconnect();

	//! �������� �� ������������� ���������� � ��������
	bool	IsAlreadyConnectedTo( LPCTSTR alpcServerIP, unsigned int auiServerPort);

	//! ��������� ��� ���
	bool	IsConnected();

	//! �������� ��������� �� ������
	void Send(unsigned char aType, const unsigned char FAR *aData, int aDataSize);

protected:
	void SaveAsyncMessage(unsigned char aType, const unsigned char FAR *aData, int aDataSize);
	void ClearAsyncMessages();

	//! ����� ������� ������
	//! ���� ������� ������
	cs::CServerInfo serverInfo;
	bool			connectionRequestIsActive;
	bool			connectionIsDestroyed;

	//! �������� ���������� � �������� ������
	cs::iclientsession* clientSession;
	//! ���������� ��������� �� �������
	cs::imessagehandler* messageHandler;
	//! ���������� ������� ������ � �������������� �����
	cs::isessionlistener* sessionListener;
	//! ��������, ��� ��� ��� ������� :)
	oms::omsContext* context;
	//! ���������� ��� ��������������� ��� ��� ������
	voipServerSessionType		sessionType;

	//! ����������� ������ �� ������
	CCriticalSection			csSession;
	CEvent						evConnect;
	MP_VECTOR<VoipMessage*>	asyncMessages;
};