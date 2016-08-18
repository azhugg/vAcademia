#pragma once

#include <string>

using namespace std;

typedef unsigned char UCHAR;

class CDataBuffer2
{	
public:
	CDataBuffer2();
	//CDataBuffer2( CDataBuffer2 *frame);
	CDataBuffer2( const BYTE* data, int size, int destOffset = 0, int sourceOffset = 0);	
	CDataBuffer2( void* data, int size, int destOffset = 0, int sourceOffset = 0);	
	CDataBuffer2( CDataBuffer2 *frame, int destOffset = 0, int sourceOffset = 0);
	CDataBuffer2( UCHAR v, int size);
	~CDataBuffer2();

	void init();
	inline operator LPVOID() { return data;};
	void createBuffer( int aSize);
	void setDataLength( int aSize);

public:
	// �������� ������
	void *getData();
	// �������� ������
	UCHAR *getBytes();
	// �������� ������
	void *getCopyData();
	// �������� ������
	void *getCopyData( int aSize, int pos = 0);
	// �������� ������
	void getCopyData( void *data, int aSize, int destOffset = 0, int sourceOffset = 0);	
	// �������� ������
	void getCopyNewData( UCHAR **data, int aSize, int destOffset = 0, int sourceOffset = 0);	
	// �������� ������ 8*bytes-������ ������
	int getSize( int bytes = 1);	
	// ������� ����� ������
	void createData( int aSize);
	// ������� ����� ������
	void createData( void* data, int aSize, int destOffset = 0, int sourceOffset = 0, BOOL bUseBlock = TRUE);
	// ������� ����� ������
	void createData( const BYTE* data, int aSize, int destOffset = 0, int sourceOffset = 0, BOOL bUseBlock = TRUE);
	// ������� ����� ������
	void createData( CDataBuffer2 *frame, int destOffset = 0, int sourceOffset = 0);
	// �������� ������
	void updateData( void *data, int aSize, BOOL bUseBlock = TRUE);
	// �������� ������
	void updateData( void *data, int aSize, int destOffset, int sourceOffset = 0, BOOL bUseBlock = TRUE);
	// ���������� ������
	int setData( const void *data, int aSize, int destOffset = 0, int sourceOffset = 0);
	// ���������� ������
	int setData( CDataBuffer2 *DataBuffer2, int destOffset = 0, int sourceOffset = 0);	
	// ������� ������
	void deleteData();
	// ���������� ��� �����
	BOOL getBlockType();
	// ���������� ��� �����
	void setBlockType( BOOL bUseBlock);	
	// ���������� ������ ����� ���������
	void setDataByValue( UCHAR v, int aSize, int destOffset = 0);

public:
	void attachData( UCHAR *aData, int aSize, int destOffset = 0);

	// �������� ������
	template <class T>
	void add( T data)
	{
		setData( data, size);		
	}
	// �������� ������
	int addData( const void *data, int aSize, int sourceOffset = 0);
	// �������� ������
	int addData( CDataBuffer2 *DataBuffer2, int sourceOffset = 0);	
	void addArray( UCHAR *data, WORD aSize);
	void addString( const char* str);
	void addString( const string& str);
	void addString( const wstring& str);
	void addString( const wchar_t* str);
	void addLargeArray( UCHAR *data, DWORD aSize);
	void addLargeString( const char* str);
	void addLargeString( const string& str);

public:
	void reset();	
	void seek( int aPos);	
	int getPosition();	
	bool endOfData();

	bool mayBeReadAt( int aSize);

	template <class T>
	BOOL read( T &data)
	{
		if ( !mayBeReadAt( sizeof( T)))
			return false;
		getData( data, pos);
		pos += sizeof( T);
		return TRUE;
	}

	BOOL readData( UCHAR **data, DWORD aSize, BOOL bCreate = FALSE);	
	BOOL readArray( UCHAR **data, WORD &aSize, BOOL bCreate = FALSE);
	BOOL readLargeArray( UCHAR **data, DWORD &aSize, BOOL bCreate = FALSE);
	
	BOOL readData( CDataBuffer2 *DataBuffer2, DWORD aSize = 0, BOOL bCreate = FALSE);
	BOOL readArray( CDataBuffer2 *DataBuffer2, BOOL bCreate = FALSE);
	BOOL readLargeArray( CDataBuffer2 *DataBuffer2, BOOL bCreate = FALSE);
	BOOL readString( string &str);
	BOOL readString( wstring &str);
	BOOL readLargeString( string str);

	BOOL readDataOffset( UCHAR **data, DWORD aSize, DWORD destOffset);
	
public:	
	int shiftl( unsigned int aPos);
	int shiftr( unsigned int aPos);

public:	
	int getMemorySize();

public:
	// ���������� ������ ������������� ����
	template <class T>
	void setData( T data, int destOffset = 0)
	{		
		setData( (UCHAR*) &data, sizeof( T), destOffset);		
	};

	// �������� ������ ������������� ����
	template <class T>
	void getData( T &data, int pos = 0)
	{		
		data = *(( T*)(( UCHAR *)this->data + pos));
	};

	
	// �������� ������ ������������� ����
	template <class T>
	T* getDataArray( T *data, int aSize, int pos = 0)
	{
		int typeSize = sizeof( T);
		int count = size / typeSize;
		if ( data == NULL)
			data = MP_NEW_ARR( T, count);
		UCHAR *bytes = MP_NEW_ARR( UCHAR, typeSize);
		for ( int i = 0; i < count; i++)
		{
			rtl_memcpy( bytes, typeSize, (UCHAR *) this->data + pos + i * typeSize, typeSize);
			data[ i] = *(( T*)bytes);
		}
		MP_DELETE_ARR( bytes);
		return data;
	};

protected:
	void _createData( int aSize);
	void setSize( int aSize);
	void grow( int aSize);
	BOOL isValidParams( const void *data, int aSize, int destOffset = 0, int sourceOffset = 0);	

private:
	void *data;
	int size;
	BOOL bUseBlock;
	int begin;
	int pos;
	int memorySize;	
	BOOL bAttached;
};
