#pragma once

#include "CommonRenderManagerHeader.h"

// ������� ����� ��� �������, �������� ������ �������
// ������ ������ �� ���� ��� �� ��������� �������
class CLinkCollector
{
public:
	CLinkCollector();
	~CLinkCollector();

	void AddLink(void* pLink);
	void DeleteLink(void* pLink);
	std::vector<void*>& GetLinks();

private:
	MP_VECTOR<void*> m_links;
};