#pragma once
#include "CommonPacketOut.h"

/************************************************************************/
/* ����������� ������ �������� ��� ������������ �������
/************************************************************************/

using namespace Protocol;

class CPreviewServerRequestQueryOut : public CCommonPacketOut
{
public:
	CPreviewServerRequestQueryOut( unsigned int aiRecordID);
};