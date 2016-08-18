//////////////////////////////////////////////////////////////////////

#pragma once
//�� ����� ����� ��� ���������� - 4251 - warning � ���, ��� � ���������� � ��� ��������� �� ������ ������ (std, stl) 
//�� ��� ������� ������ ��� 2010 �� ��������, ����� ������� ��� ���������� ������ ���������(������ � ��� std::string �� char* � ��)
#pragma warning(disable: 4251) 

#define _HAS_ITERATOR_DEBUGGING 0
#define _ITERATOR_DEBUG_LEVEL 0
#define _SECURE_SCL		0
#define THREAD_STACK_SIZE 64000

#ifndef _WIN32_WINNT
#if _MSC_VER >= 1600
#define _WIN32_WINNT 0x0502
#else
#define _WIN32_WINNT 0x0500
#endif
#endif

