#pragma once
#include "CommonPacketIn.h"

/************************************************************************/
/* ����� �� ������� ������ ��������� �����. ������:
1. RecordUD
2. MixedFileName
/************************************************************************/
class CMixedAudioCreatedQueryIn : public Protocol::CCommonPacketIn
{
public:
	CMixedAudioCreatedQueryIn( BYTE *aData, int aDataSize);
	virtual bool Analyse();

	unsigned int	GetRecordID();
	LPCSTR			GetMixedAudioName();

private:
	int			recordID;
	CComString	mixedAudioName;
};
