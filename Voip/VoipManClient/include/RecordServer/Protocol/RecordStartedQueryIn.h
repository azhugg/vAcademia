#pragma once
#include "CommonPacketIn.h"

/************************************************************************/
/* ����� �� ������� ������ ��������� �����. ������:
1. RecordUD
/************************************************************************/
class CRecordStartedQueryIn : public Protocol::CCommonPacketIn
{
public:
	CRecordStartedQueryIn( BYTE *aData, int aDataSize);
	virtual bool Analyse();

public:
	unsigned int	GetRecordID();

protected:
	unsigned int	recordID;
};
