#include "stdafx.h"
#include "RecordEditorSeekIn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace SyncManProtocol;

CRecordEditorSeekIn::CRecordEditorSeekIn( BYTE *apData, int apDataSize) :
	CCommonPacketIn( apData, apDataSize)
{
}

bool CRecordEditorSeekIn::Analyse()
{
	if (!CCommonPacketIn::Analyse())
		return false;

	int idx = 0;
	if(!memparse_object(pData, idx, iErrorCode))
		return false;

	return EndOfData( idx);
}

const int CRecordEditorSeekIn::GetErrorCode() const
{
	return iErrorCode;
}

