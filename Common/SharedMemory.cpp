#include "mumble_pch.hpp" 
//#include "stdafx.h"
#include "SharedMemory.h"
#include "windows.h"
#include "winuser.h"
#include <string>

#ifndef HANDLE
typedef void* HANDLE;
#endif

using namespace std;

typedef unsigned int SMBSize; // ��� ��� ������� ����� ����������� ������

/*
 * ����� ��� ������ � Shared Memory
 */
class CSharedMemoryBlock: public ISharedMemoryBlock
{
	wstring m_sStringID; // ������������� shared memory � �������
	HANDLE m_hGlobalMemoryObject; // handle ����� shared memory 
	SharedMemoryBlockPtr m_pPointer;	// ��������� �� ����� ������ ����� ������
	SharedMemoryBlockPtr m_pDataPointer;	// ���������, ������������ ����� ��������� GetPointer
	SMBSize m_uSize;	// ������ ����� ����������� ������
	int m_iLastError;		// ������ ��������� ��������
public:
	// alpcStringID - ������������� shared memory
	CSharedMemoryBlock(const wchar_t* apwcStringID, unsigned int auiSize = 0);
	virtual ~CSharedMemoryBlock();

// ���������� ISharedMemoryBlock
public:
	// �������� ������ ����� ����������� ������
	virtual unsigned int GetSize(){ return m_uSize; }

	// �������� ��������� �� ���� ����������� ������
	virtual SharedMemoryBlockPtr GetPointer();

	// ���������� ��������� �� ���� ����������� ������
	virtual void FreePointer();

	// �������� ��� ������ ��������� ��������
	virtual int GetLastError();

	// �������� ��������� ������������� �����
	virtual const wchar_t* GetStringID(){ return m_sStringID.c_str(); }

	// ��������� ���������
	virtual void Release(); 

private:
	// ���������� ������ ����� ����������� ������
	void DefineSize();

	// ��������� ������ � ���� ����������� ������
	//void SaveSize();

	// ����������� ������ ����� ����������� ������ �� ��� �����
	// ���������� ��������� �� ���� ������
	//SharedMemoryBlockPtr LoadSize(SharedMemoryBlockPtr apPointer);

	// �������� ��������� �� ����� ������ ����� ������
	//SharedMemoryBlockPtr GetSM0Pointer();

	// �� ��������� �� ������ ���������� ��������� �� ����� ������ ����� ������
	//SharedMemoryBlockPtr UnshiftTo0(SharedMemoryBlockPtr apDataPointer);
};

// alpcStringID - ������������� shared memory
// auiSize - ������ ����� (���� == 0, �� ����������� ������������ ����)
CSharedMemoryBlock::CSharedMemoryBlock(const wchar_t* alpcStringID, unsigned int auiSize)
{
	m_sStringID = alpcStringID;
	m_hGlobalMemoryObject = NULL;
	m_uSize = 0;
	m_pPointer = NULL;
	m_pDataPointer = NULL;
	m_iLastError = ERROR_SUCCESS;
	if(auiSize > 0){
		m_hGlobalMemoryObject = ::CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, auiSize, m_sStringID.c_str());
		m_iLastError = ::GetLastError();
		// ���� ���� ������ � ����� ID ��� ����, 
		if(m_iLastError == ERROR_ALREADY_EXISTS){
			this->GetPointer();
			this->FreePointer();
			// �� ��� ������ ������
			if(m_uSize < auiSize){
				// �� �������, ��� ������� ���� �� ����������
				if (m_hGlobalMemoryObject != INVALID_HANDLE_VALUE && m_hGlobalMemoryObject != NULL)
					CloseHandle(m_hGlobalMemoryObject);
				m_hGlobalMemoryObject = INVALID_HANDLE_VALUE;
				m_iLastError = ERROR_INVALID_HANDLE;
				m_uSize = 0;
			}
		}else{
			this->GetPointer();
			if(m_pPointer != NULL){
				*((SMBSize*)m_pPointer) = auiSize;
				m_uSize = auiSize;
			}
			this->FreePointer();
		}
	}else{
		m_hGlobalMemoryObject = ::OpenFileMappingW(
			FILE_MAP_ALL_ACCESS,   // read/write access
			FALSE,                 // do not inherit the name
			m_sStringID.c_str());               // name of mapping object
		m_iLastError = ::GetLastError();
		// ������ ������ ����� ����������� ������
		this->DefineSize();
	}
}

// ���������� ������ ����� ����������� ������
void CSharedMemoryBlock::DefineSize(){
	if(m_hGlobalMemoryObject == INVALID_HANDLE_VALUE || m_hGlobalMemoryObject == NULL){
		m_uSize = 0;
		return;
	}
	// ������ ����������� �� ����� ������ � GetPointer()
	this->GetPointer();
	this->FreePointer();
}

CSharedMemoryBlock::~CSharedMemoryBlock(){
	FreePointer();
	if (m_hGlobalMemoryObject)
		CloseHandle(m_hGlobalMemoryObject);
}

// �������� ��������� �� ���� ����������� ������
SharedMemoryBlockPtr CSharedMemoryBlock::GetPointer()
{
	m_iLastError = ERROR_SUCCESS;
	if(m_pDataPointer != NULL)
		return m_pDataPointer;
	m_pPointer = NULL;
	m_pDataPointer = NULL;
	if(m_hGlobalMemoryObject == INVALID_HANDLE_VALUE || m_hGlobalMemoryObject == NULL){
		m_iLastError = ERROR_INVALID_HANDLE;
		return NULL;
	}
	m_pPointer = (SharedMemoryBlockPtr)::MapViewOfFile(m_hGlobalMemoryObject, FILE_MAP_ALL_ACCESS, 0, 0, m_uSize);
	if(m_pPointer == NULL){
		m_iLastError = ::GetLastError();
		return NULL;
	}
	m_uSize = *((SMBSize*)m_pPointer);
	m_pDataPointer = ((SMBSize*)m_pPointer)+1;
	return m_pDataPointer;
}

// ���������� ��������� �� ���� ����������� ������
void CSharedMemoryBlock::FreePointer()
{
	if(m_pPointer == NULL)
		return;

	::UnmapViewOfFile(m_pPointer); 
	m_iLastError = ::GetLastError();
	m_pPointer = NULL;
	m_pDataPointer = NULL;
}

// �������� ��� ������ ��������� ��������
int CSharedMemoryBlock::GetLastError()
{
	return m_iLastError;
}

// ��������� ���������
void CSharedMemoryBlock::Release()
{
	delete ((CSharedMemoryBlock*)this);
}

ISharedMemoryBlock* CSharedMemory::CreateBlock(const wchar_t* apwcStringID, unsigned int aiSize)
{
	return (ISharedMemoryBlock*)(new CSharedMemoryBlock(apwcStringID, aiSize));
}

ISharedMemoryBlock* CSharedMemory::GetBlock(const wchar_t* apwcStringID)
{
	return (ISharedMemoryBlock*)(new CSharedMemoryBlock(apwcStringID));
}

