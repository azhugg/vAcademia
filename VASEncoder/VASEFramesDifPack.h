#pragma once

class VASEBitmap;

/************************************************************************/
/* ������ ������� ����� ������� � ���� ������ ���������� ��������
/************************************************************************/
class VASE_API VASEFramesDifPack
{
public:
	VASEFramesDifPack(void);
	~VASEFramesDifPack(void);

public:
	//! ������� �������� � ���������� �� ��� ���������
	VASEBitmap*	AddBitmap();
	//! ������� �������� � ���������� �� ��� ���������
	bool	DeleteBitmap(VASEBitmap* apBitmap);
	//! ���������� ���������� ��������
	unsigned int	GetBitmapsCount();
	//! ���������� �������� �� �������
	VASEBitmap*		GetBitmap( unsigned int auIndex);
	//! ������������� ���� ���� ��� �������� �� ��������� ����� wavelet-��������������
	void SetEasyPacked();
	//! 
	bool IsEasyPacked();

private:
	MP_VECTOR<VASEBitmap*> bitmaps;
	CRITICAL_SECTION	csFactory;
	bool easyPacked;
};
