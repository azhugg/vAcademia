#pragma once
#include "..\RecordServer\Protocol\CommonVoipPacketOut.h"

/************************************************************************/
/* ����������� ������ �������� ��� ������������ �������
/************************************************************************/
class CVoipServerRequestQueryOut : public CCommonVoipPacketOut
{
public:
	CVoipServerRequestQueryOut( LPCSTR alpcRoomName);
};