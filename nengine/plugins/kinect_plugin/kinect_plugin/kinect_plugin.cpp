#include "stdafx.h"
#include "SkeletalViewer.h"
#include "kinect_plugin.h"



/*****************************************************
	������ ���������� ������ �� ���������
	����� ������� �����  ������������������ ������
	��������� ���������� �������
	� ������������������� ������ �  ������� ������
******************************************************/

IKinect* CreateKinectObject()
{
	CSkeletalViewerApp*	pSkeletalViewerApp = new CSkeletalViewerApp();
	return pSkeletalViewerApp;
}

