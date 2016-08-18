#include "StdAfx.h"
#include ".\VASEFramesDifPack.h"
#include ".\VASEBitmap.h"
#include <algorithm>

#include ".\MemLeaks.h"

VASEFramesDifPack::VASEFramesDifPack(void): MP_VECTOR_INIT(bitmaps)
{
	InitializeCriticalSection(&csFactory);
	easyPacked = false;
}

VASEFramesDifPack::~VASEFramesDifPack(void)
{
	std::vector<VASEBitmap*>::iterator it = bitmaps.begin(), end = bitmaps.end();
	for (; it!= end; it++)
	{
		SAFE_DELETE( (*it));
	}
	bitmaps.clear();
	DeleteCriticalSection(&csFactory);
}

//! ���������� ���������� ��������
unsigned int	VASEFramesDifPack::GetBitmapsCount()
{
	return bitmaps.size();
}

//! ���������� �������� �� �������
VASEBitmap*	VASEFramesDifPack::GetBitmap( unsigned int auIndex)
{
	if( auIndex >= bitmaps.size())
		return NULL;

	return bitmaps[ auIndex];
}

//! ������� �������� � ���������� �� ��� ���������
VASEBitmap*	VASEFramesDifPack::AddBitmap()
{
	VASEBitmap*	bitmap = MP_NEW( VASEBitmap);
	EnterCriticalSection(&csFactory);
	bitmaps.push_back( bitmap);
	LeaveCriticalSection(&csFactory);
	return bitmap;
}

//! ������� �������� � ���������� �� ��� ���������
bool	VASEFramesDifPack::DeleteBitmap(VASEBitmap* apBitmap)
{
	std::vector<VASEBitmap*>::iterator it = find( bitmaps.begin(), bitmaps.end(), apBitmap);
	if( it != bitmaps.end())
	{
		SAFE_DELETE( (*it));
		EnterCriticalSection(&csFactory);
		bitmaps.erase( it);
		LeaveCriticalSection(&csFactory);
		return true;
	}
	return false;
}

//! ������������� ���� ���� ��� �������� �� ��������� ����� wavelet-��������������
void VASEFramesDifPack::SetEasyPacked()
{
	easyPacked = true;
}
//! 
bool VASEFramesDifPack::IsEasyPacked()
{
	return easyPacked;
}