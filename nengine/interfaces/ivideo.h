
#pragma once

class IDynamicTexture;

class IVideo
{
public:
	// ������ ���������������
	virtual void Start() = 0;
	// ���������� ���������������
	virtual void Stop() = 0;
	// �����
	virtual void Pause() = 0;
	// ���������� ������ ����� �����
	virtual void Play() = 0;

	// ���������� ����������������� �������� � �������������
	virtual unsigned int GetDuration() = 0;
	// ����������/����� ����������� ���������������
	// �� ��������� ������������� ���
	virtual void SetLoop(bool isLooped) = 0;

	// ������� ������� ������� � �������������
	virtual int GetPosition() = 0;
	// ���������� ������� ������� � �������������
	virtual void SetPosition(unsigned int pos) = 0;

	// ������� ������� ����� �����
	virtual int GetFrameNum() = 0;
	// ���������� ������� ����� �����
	virtual void SetFrameNum(unsigned int num) = 0;

	// ���������� true, ���� ������������� ��������
	virtual bool IsLooped() = 0;
	// ���������� true, ���� ���� ������������
	virtual bool IsPlayed() = 0;
	// ���������� true, ���� ���������� �����
	virtual bool IsPaused() = 0;

	// ���������� ������������ ��������
	virtual IDynamicTexture* GetDynamicTexture() = 0;

	// ������� ���� � ���������� ���������� ��������
	virtual bool Open(char* fileName) = 0;
	// ���������� �����
	virtual void Close() = 0;
};