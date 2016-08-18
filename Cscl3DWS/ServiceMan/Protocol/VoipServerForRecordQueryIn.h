#pragma once
#include "CommonPacketIn.h"

/************************************************************************/
/* ����� �� �������� ������� ��������� �����. ������:
1. ����� ������
2. VoipServer - [4 bytes - ����� ������� ��������� �����] | [UnicodeString]
/************************************************************************/
class CVoipServerForRecordQueryIn : public Protocol::CCommonPacketIn
{
public:
	CVoipServerForRecordQueryIn( BYTE *aData, int aDataSize);
	virtual bool Analyse();

public:
	CComString		GetRecordServerIP();
	unsigned int	GetRecordServerPort();
	CComString		GetPlayServerIP();
	unsigned int	GetPlayServerPort();
	unsigned int	GetRecordID();

protected:
	CComString		recordServerIP;
	unsigned int	recordServerPort;
	CComString		playServerIP;
	unsigned int	playServerPort;
	unsigned int	recordID;
};