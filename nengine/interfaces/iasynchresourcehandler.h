#pragma once

/*
��������� ��������� ������� �������� ������������ �������
*/

#include "iasynchresource.h" 

struct IAsynchResourceHandler
{
	// �������� �������� �������� �������
	virtual void OnAsynchResourceLoadedPersent(IAsynchResource* asynch, unsigned char percent) = 0;
	// ������ ��������
	virtual void OnAsynchResourceLoaded(IAsynchResource* asynch) = 0;
	// ������ ��������
	virtual void OnAsynchResourceError(IAsynchResource* asynch) = 0;
};