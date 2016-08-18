#pragma once
#include "CommonPacketIn.h"

/************************************************************************/
/* ����� �� ������� ������ ��������� �����. ������:
1. RecordUD
/************************************************************************/
class CRecordFinishedIn: public Protocol::CCommonPacketIn
{
public:
	CRecordFinishedIn( BYTE *aData, int aDataSize);
	virtual bool Analyse();

public:
	unsigned int	GetRecordID();
	unsigned int	GetErrorCode();

protected:
	unsigned int	recordID;
	unsigned int	errorCode;
};
