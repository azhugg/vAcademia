#pragma once
#include "CommonPacketIn.h"

/************************************************************************/
/* ����� �� ������� ��������� �����. ��������� ������ ���������� EvoVOIP �������
������:
auRealityID - 
auRecordID - 
auState - ��. public enum EvoClientWorkState � EvoVoipClientConnectStatusChangedOut.cs
auErrorCode - 
}
/************************************************************************/
class CEvoClientWorkStateChangedQueryIn : public Protocol::CCommonPacketIn
{
public:
	CEvoClientWorkStateChangedQueryIn( BYTE *aData, int aDataSize);
	virtual bool Analyse();

public:
	unsigned int	GetRealityID();
	unsigned int	GetRecordID();
	voip::voipEvoClientWorkState	GetWorkState();
	unsigned int	GetErrorCode();
	voip::voipEvoClientSessionType	GetSessionType();

protected:
	unsigned int			realityId;
	unsigned int			recordId;
	unsigned int			errorCode;
	voip::voipEvoClientWorkState	workState;
	voip::voipEvoClientSessionType	sessionType;
};
