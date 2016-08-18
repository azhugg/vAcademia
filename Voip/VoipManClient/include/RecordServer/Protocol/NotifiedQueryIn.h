#pragma once
#include "CommonPacketIn.h"

/************************************************************************/
/* ����� �� ������� ��������� �����. ��������� ������ ���������� EvoVOIP �������
/************************************************************************/
class CNotifiedQueryIn : public Protocol::CCommonPacketIn
{
public:
	CNotifiedQueryIn( BYTE *aData, int aDataSize);
	virtual bool Analyse();

public:
	//! ���������� ��� �����������
	byte			GetNotificationCode();

	//! ���������� ����� ��� ������ ������
	unsigned int GetSessionToken();


protected:
	//! ��� �����������
	byte	notificationCode;
	//! ����� ������
	unsigned int	sessionToken;
};
