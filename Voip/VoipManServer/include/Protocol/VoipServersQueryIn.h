#pragma once
#include "CommonPacketIn.h"

/************************************************************************/
/* ����� �� �������� ������� ��������� �����. ������:
1. VoipServer - [4 bytes - ����� ������� ��������� �����] | [UnicodeString]
2. TcpPort - 2 bytes - ����
3. UdpPort - 2 bytes - ����
4. VoipRecordServer - [4 bytes - ����� ������������� ������� ��������� �����] | [UnicodeString]
5. RecordPort - 2 bytes - ����
6. ��� ������� UnicodeString
/************************************************************************/
class CVoipServersQueryIn : public Protocol::CCommonPacketIn
{
public:
	CVoipServersQueryIn( BYTE *aData, int aDataSize);
	virtual bool Analyse();

public:
	CComString		GetRoomName();
	CComString		GetVoipHostName();
	CComString		GetRecordHostName();

	WORD			GetTcpPort();
	WORD			GetUdpPort();
	WORD			GetRecordPort();

protected:
	CComString		roomName;
	CComString		voipServerName;
	CComString		voipRecordServerName;

	WORD			tcpPort;
	WORD			udpPort;
	WORD			recordPort;
};
