#pragma once
#include "CommonPacketIn.h"

/************************************************************************/
/* ����� �� ������� ������ ��������� �����. ������:
1. RecordUD
/************************************************************************/
class CRecordPlayFinishedIn: public Protocol::CCommonPacketIn
{
public:
	CRecordPlayFinishedIn( BYTE *aData, int aDataSize);
	virtual bool Analyse();

public:
	unsigned int	GetRecordID();

protected:
	unsigned int	recordID;
};
