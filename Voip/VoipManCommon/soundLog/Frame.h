#pragma once

class CFrame
{	
public:
	CFrame();
	CFrame( CFrame *frame);
	CFrame( void* data, int size);
	~CFrame();

public:
	// �������� ������
	void *getData();
	// �������� ������
	void *getData( int pos);
	// �������� ������ ������ ����
	int getIntData();	
	// �������� ������ ������ ����
	int getIntData( int size, int pos = 0);	
	// �������� ������ ���� � ��������� ������
	float getFloatData();
	// �������� ������ ���� � ��������� ������
	float getFloatData( int size, int pos = 0);
	// �������� ������
	void *getCopyData();
	// �������� ������
	void *getCopyData( int size, int pos = 0);
	// �������� ������
	void getCopyData( void *data, int size, int destOffset = 0, int sourceOffset = 0);	
	// �������� ������ 8*bytes-������ ������
	int getSize( int bytes = 1);

	// ������� ����� ������
	void createData( int size);

protected:
	// ������� ����� ������
	void createData( void* data, int size, BOOL bUseBlock = TRUE);
	// ������� ����� ������
	void createData( CFrame *frame);

public:
	// ��������� ����� ������ size ������
	void createSilence( int size);
	// ��������� ����� ������ ������
	void createSilence();
	// �������� ������
	void updateData( void *data, int size, BOOL bUseBlock = TRUE);
	// �������� ������
	void updateData( void *data, int size, int destOffset, int sourceOffset = 0, BOOL bUseBlock = TRUE);
	// �������� ������
	void updateData( CFrame *frame, int destOffset = 0, int sourceOffset = 0, BOOL bUseBlock = TRUE);
	// ���������� ������
	int copyData( const void *data, int size, int destOffset = 0, int sourceOffset = 0);
	// ���������� ������
	int copyBytes( const BYTE *data, int size, int destOffset = 0, int sourceOffset = 0);
	// ���������� ������
	int copyData( CFrame *frame, int destOffset = 0, int sourceOffset = 0);
	// ���������� ������ ������ ����
	int copyIntData( int data, int size, int destOffset = 0, int sourceOffset = 0);
	// ���������� ������ ���� � ��������� ������
	int copyFloatData( float data, int size, int destOffset  = 0, int sourceOffset = 0);
	// ������� ������
	void deleteData();
	// ���������� ��� �����
	BOOL getBlockType();
	// ���������� ��� �����
	void setBlockType( BOOL bUseBlock);
	// ���������� ������ ������: �� ���� data ����� ���������� ������. ������ ������ ������� ������ ����.
	void setEmptyData();	

private:
	char *convertIntToChar( int iData, int size);


private:
	void *data;
	int size;
	BOOL bUseBlock;
};
