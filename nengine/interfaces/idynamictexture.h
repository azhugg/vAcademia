#pragma once

class IDynamicTexture
{
public:
	// �������� ��������� ������� (�� ��������)
	virtual void Update() = 0;
	// �������� ��������
	virtual void UpdateFrame() = 0;

	// ������� ������ �� ������ � ���������� ���������� �����	
	virtual void* GetFrameData() = 0;

	// ������� ������ �������� �������� � ��������
	virtual unsigned short GetSourceWidth() = 0;
	// ������� ������ �������� �������� � ��������
	virtual unsigned short GetSourceHeight() = 0;
	// ������� ������ �������������� �������� � ��������
	virtual unsigned short GetTextureWidth() = 0;
	// ������� ������ �������������� �������� � ��������
	virtual unsigned short GetTextureHeight() = 0;
	// ���������� ������ �������������� ��������
	virtual void SetTextureSize(int width, int height) = 0;
	// ������� ���������� �������� �������, 3 ��� RGB, 4 ��� RGBA
	virtual unsigned char GetColorChannelCount() = 0;
	// ������� �������� ������� ������ (RGB) ��� �������� (BGR)
	virtual bool IsDirectChannelOrder() = 0;

	// ����� �� ��������� �������� (��� ������ � ���������� �� ����������?)
	virtual bool IsNeedUpdate() = 0;

	// �������� ����
	// (��������� ��� ����, ����� nengine ��� ��� ������ ����� ��� ��������� �������)
	virtual void Destroy() = 0;

	// ��� ����� ������� ���������� IDynamicTexture ������ 
	// ������������ userDataID ���� �� �� 0 �� 3 ������������
	// �������� ���������������� (����������� ��� ���� ������������ ��������) ������
	virtual void* GetUserData(unsigned int userDataID) = 0;
	// ���������� ���������������� (����������� ��� ���� ������������ ��������) ������
	virtual void SetUserData(unsigned int userDataID, void* data) = 0;

	virtual void SetFreezState(bool isFreezed)= 0;
	virtual bool IsFreezed()= 0;
	virtual bool IsFirstFrameSetted()= 0;
};