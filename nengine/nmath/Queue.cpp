#include "StdAfx.h"
#include "Queue.h"

bool CQueue::QWrite(SQElement& point)//������ � ����� �������
{
	if  ( (m_tail + 1) == QUEUE_SIZE)
	{
		return false;//������� �����
	}

	m_queue[m_tail] = point;

	m_tail++;

	return true;
}

SQElement* CQueue::QRead()//������� �� ������
{
	if (m_head == m_tail) // ������� �����
	{
		return 0; 
	}

	SQElement* ptr = (&m_queue[0]) + m_head;

	m_head++;

	return ptr;
}

int CQueue::GetTailPosition()
{
	return m_tail;
}

SQElement* CQueue::GetElement(int index)
{
	return &m_queue[index];
}