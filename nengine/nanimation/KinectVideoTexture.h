
#pragma once

#include "IDynamicTexture.h"
#include "IKinect.h"

class CKinectVideoTexture : public IDynamicTexture
{
public:
	CKinectVideoTexture(IKinect* kinect);	
	~CKinectVideoTexture();

	// �������� ��������� ������� (�� ��������)
	void Update();
	// �������� ��������
	virtual void UpdateFrame();

	// ������� ������ �� ������ � ���������� ���������� �����	
	void* GetFrameData();

	// ������� ������ �������� �������� � ��������
	unsigned short GetSourceWidth();
	// ������� ������ �������� �������� � ��������
	unsigned short GetSourceHeight();
	// ������� ������ �������������� �������� � ��������
	unsigned short GetTextureWidth();
	// ������� ������ �������������� �������� � ��������
	unsigned short GetTextureHeight();
	// ���������� ������ �������������� ��������
	void SetTextureSize(int width, int height);
	// ������� ���������� �������� �������, 3 ��� RGB, 4 ��� RGBA
	unsigned char GetColorChannelCount();
	// ������� �������� ������� ������ (RGB) ��� �������� (BGR)
	bool IsDirectChannelOrder();

	// ����� �� ��������� �������� (��� ������ � ���������� �� ����������?)
	bool IsNeedUpdate();

	// �������� ����
	// (��������� ��� ����, ����� nengine ��� ��� ������ ����� ��� ��������� �������)
	void Destroy();

	// ��� ����� ������� ���������� IDynamicTexture ������ 
	// ������������ userDataID ���� �� �� 0 �� 3 ������������
	// �������� ���������������� (����������� ��� ���� ������������ ��������) ������
	void* GetUserData(unsigned int userDataID);
	// ���������� ���������������� (����������� ��� ���� ������������ ��������) ������
	void SetUserData(unsigned int userDataID, void* data) ;

	void SetFreezState(bool isFreezed);
	bool IsFreezed();
	bool IsFirstFrameSetted();

	void EnableAnalyzeMode(bool isEnabled);
	bool IsAnalyzeEnabled()const;

	unsigned int GetPixelsWithGivenChannel(char channelID);

private:
	void AllocBufferIfNeeded(unsigned int size);

	IKinect* m_kinect;
	unsigned char* m_data;
	unsigned int m_size;

	void* m_userData[4];

	bool m_isAnalyzeEnabled;
};