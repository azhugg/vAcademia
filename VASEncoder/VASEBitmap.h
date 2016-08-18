#pragma once

class VASE_API VASEBitmap
{
public:
	VASEBitmap(void);
	~VASEBitmap(void);

public:
	//! ���������� ������
	CSize GetSize();
	//! ���������� �������
	CPoint	GetPosition();
	//! ������������� �������
	void	SetPosition(CPoint aPosition);
	//! ������������� ������
	void	SetSize(CSize aSize);
	//! ���������� ������ ��������
	unsigned int GetBuffer( const void** appData, unsigned int* apDataSize);
	//! �������� ������ ��� ��������
	byte* GetBufferSetLength( unsigned int auBufferSize);
	//! ������������� ������ ����� ������
	void SetEncodedBufferSize( unsigned int auEncodedBufferSize);
private:
	CSize	m_size;
	CPoint	m_position;
	byte*	m_encodedData;
	unsigned int m_allocatedDataSize;
	unsigned int m_encodedDataSize;
};
