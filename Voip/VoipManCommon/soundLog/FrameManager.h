#pragma once

#include <vector>

class CFrame;
//PLIST(CFrameList, CFrame);

// �������� ������ - ������������ �������������� ������
class CFrameManager
{
public:
	CFrameManager( int mode = NONE, BOOL autoDeleteObject = TRUE);
	virtual ~CFrameManager( void);

	enum { NONE, EQUAL_BY_SIZE};	

private: 	
	// �������� ����
	virtual BOOL addFrame( void *data, int &size, BOOL bUseBlock, int dataOffset = 0);
	// �������� ����
	virtual BOOL updateFrame( int index, void *data, int size, BOOL bUseBlock = FALSE, int destOffset = 0, int sourceOffset = 0);

public:
	// �������� ����
	virtual BOOL updateFrames( CFrame *frame);
	// �������� ����
	virtual BOOL updateFrames( void *data, int length, BOOL bUseBlock = FALSE);
	// �������� ����
	virtual CFrame *getFrame( int index);
	// ������� ���� ��� ������� index
	virtual CFrame *removeFrameAt( int index);
	// ������� ��� �����
	virtual void removeAllFrames();
	virtual void clear();
	// ���������� TRUE, ���� ������ ������
	BOOL isEmpty();
	// �������� ����� ������
	int getFrameCount();
	// �������� ����� ������ ���� ������
	long getTotalSize();
	// �������� ������ ���������
	int getHeadSize();
	// ���������� ������ ������� �����
	void setFrameSize( int frameSize);
	// �������� ������ ������� �����
	int getFrameSize();
	// �������� ������ ��� ��������� (������� �����)
	virtual CFrame *createPacket();
	// ������������� ������
	virtual void parser( void *data, int size);
	// ������������� ������
	virtual void parser( CFrame *frame);

public:
	// ���� �����������, ���� true, �� ��, ����� ���
	virtual BOOL isBlockReady();
	//
	virtual int getBlockFrameCount();
	//
	virtual int getMode();
	//
	CFrame *getData();
	//
	CFrame *getDubleData();

private:
	// ���-�� ������
	int frameCount;
	// ����� �������� �����
	//int currentFrame;
	// ������ ���������� ������
	int lastFrameSize;
	// ����� ������ ������
	long totalSize;
	// ���-�� ������ � ����� ����� �����
	int blockFrameCount;
	// ������ ������� ����� (��������� ������������ ����� �������)
	int frameSize;
	// ������ ���������
	int headSize;
	// ����� ������ ���������� ������
	int mode;
	// ����� �������� ��������
	BOOL autoDeleteObject;

	void add( CFrame* obj)
	{ objects.push_back( obj);};

	CFrame* get( int index)
	{ return objects[ index];};	

	std::vector< CFrame*> &get_list()
	{ return objects;};

	BOOL is_empty()
	{ return objects.empty();};

	int get_size()
	{ return objects.size();};

	//	std::vector< Object>::iterator begin()
	//	{ return objects.begin();};
	//	std::vector< Object>::iterator end()
	//	{ return objects.end();};	

protected:
	MP_VECTOR<CFrame*> objects;
};
