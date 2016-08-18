#pragma once

#include "imessagehandler.h"
#include "iclientsession.h"
#include "serverInfo.h"
#include "../ConnectionListener.h"
#include "../messagetypes.h"
#include "../AsyncConnection.h"

class CRecordServerConnectionBase : cs::isessionlistener
{
public:
	CRecordServerConnectionBase (oms::omsContext* aContext, voip::voipServerSessionType aSessionType, cs::imessagehandler* apHandler, ivoiceconnectionlistender* apConnectionListener);
	~CRecordServerConnectionBase ();

	//! ���������� isessionlistener
public:
	virtual void OnConnectLost() {};
	virtual void OnConnectRestored() {};

public:
	//! ������������ � �������
	voipError	Connect( LPCTSTR alpcServerIP, unsigned int auiServerPort);

	//! ����������� �� �������
	voipError	Disconnect();

	//! ���������� true - ���� ������� ���������� � ��������
	bool IsConnected();

protected:
	//! ���������� ����� ���������� � ��������
	virtual void OnAfterConnected(){ }

protected:
	CAsyncConnection serverConnection;

	//! ��������, ��� ��� ��� ������� :)
	oms::omsContext* context;

	//! ��������� ������ �����
	ivoiceconnectionlistender* connectionListener;
};