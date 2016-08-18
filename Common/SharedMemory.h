/*
	��������� � ��������� ������ � ����������� (����� ����������) �������

	������ �������������:
		�������-"������": 
			// ������� ���� ����������� ������ � ������ "SharedMemBlockName1" � �������� - 256 ����
			ISharedMemoryBlock* sharedMemBlock = CSharedMemory::CreateBlock(L"SharedMemBlockName1", 256);
			... (�� ���� �������������:)
			// �������� ��������� �� ���� ����������� ������
			int* pMemory = (int*)sharedMemBlock->GetPointer();
			// ��������� ���� ������-�� ����������
			int iMemSize = sharedMemBlock->GetSize();
			for(int i = 0; i < iMemSize; i++)
				pMemory[i] = i;
			// ��������� ������� �������� �������� � ���� ������ ������
			sharedMemBlock->FreePointer();
			...
			// ������ �� ����� �������� � ���� ������ ������
			sharedMemBlock->Release();

		�������-"������": 
			// ������� ���� ����������� ������ � ������ "SharedMemBlockName1"
			ISharedMemoryBlock* sharedMemBlock = CSharedMemory::GetBlock(L"SharedMemBlockName1");
			...
			// �������� ��������� �� ���� ����������� ������
			int* pMemory = (int*)sharedMemBlock->GetPointer();
			// ��������� �������� �� �����
			int iMemSize = sharedMemBlock->GetSize();
			int* values = new int[iMemSize];
			for(int i = 0; i < iMemSize; i++)
				values[i] = pMemory[i];
			delete[] values;
			// ��������� ������� �������� �������� � ���� ������ ������
			sharedMemBlock->FreePointer();
			...
			// ������ �� ����� �������� � ���� ������ ������
			sharedMemBlock->Release();
*/
#include "stdafx.h"

typedef void* SharedMemoryBlockPtr;

// ��������, ������������ ISharedMemoryBlock::GetLastError() (������������� ���������)
#ifndef ERROR_SUCCESS
#define ERROR_SUCCESS 0
#define ERROR_INVALID_HANDLE 6
#endif

/*
 * ��������� ������� � ����� ����������� ������
 */
struct ISharedMemoryBlock
{
	// �������� ������ ����� ����������� ������
	virtual unsigned int GetSize() = 0;

	// �������� ��������� �� ���� ����������� ������
	virtual SharedMemoryBlockPtr GetPointer() = 0;

	// ���������� ��������� �� ���� ����������� ������
	virtual void FreePointer() = 0;

	// �������� ��� ������ ��������� ��������
	virtual int GetLastError() = 0;

	// �������� ��������� ������������� �����
	virtual const wchar_t* GetStringID() = 0;

	// ��������� ���������
	virtual void Release() = 0; 
};

class CSharedMemory
{
public:
	/*
	 * �������� ��������� � ����� ���������� ������������ ����� ����������� ������
	 */
	static ISharedMemoryBlock* CreateBlock(const wchar_t* apwcStringID, unsigned int aiSize);

	/*
	 * �������� ��������� � ������������ ����� ����������� ������
	 */
	static ISharedMemoryBlock* GetBlock(const wchar_t* apwcStringID);
};

