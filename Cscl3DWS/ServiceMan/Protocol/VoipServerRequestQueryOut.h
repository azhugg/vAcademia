#pragma once
#include "CommonVoipPacketOut.h"

/************************************************************************/
/* ����������� ������ �������� ��� ������������ �������
/************************************************************************/
class CVoipServerRequestQueryOut : public CCommonVoipPacketOut
{
public:
	CVoipServerRequestQueryOut( LPCSTR alpcRoomName);
};