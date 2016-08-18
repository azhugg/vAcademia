#pragma once
#include "CommonPacketIn.h"

typedef enum	EPLAYSESIONMODE	
{ 
	EPM_ONSERVER = 0,
	EPM_ONCLIENT = 1,
	EPM_LAST
};

/************************************************************************/
/* ����� �� ������� ������ ��������� �����. ������:
1. PlayMode - ����� ���������������
/************************************************************************/
class CPlaySessionConnectedQueryIn : public Protocol::CCommonPacketIn
{
public:
	CPlaySessionConnectedQueryIn( BYTE *aData, int aDataSize);
	virtual bool Analyse();

public:
	EPLAYSESIONMODE	GetPlaySessionMode();
	unsigned int	GetRealityID();
	unsigned int	GetRecordID();

protected:
	EPLAYSESIONMODE	playSessionMode;
	unsigned int	realityId;
	unsigned int	recordId;
};
