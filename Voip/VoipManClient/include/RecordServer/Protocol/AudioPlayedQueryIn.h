#pragma once
#include "FileCreatedQueryIn.h"

/************************************************************************/
/* ����� �� ������� ������ ��������� �����. ������:
1. RecordUD
/************************************************************************/
class CAudioPlayedQueryIn : public CFileCreatedQueryIn
{
public:
	CAudioPlayedQueryIn( BYTE *aData, int aDataSize) : CFileCreatedQueryIn( aData, aDataSize) {}
};
