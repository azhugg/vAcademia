#pragma once

/*
��������� ��������� ������� ���������� �������� 3� �������
*/

struct IAnimationEventsHandler
{
	// �������� ���������
	virtual void OnAnimationComplete() = 0;
	// �������� �����������
	virtual void OnAnimationSet() = 0;
};