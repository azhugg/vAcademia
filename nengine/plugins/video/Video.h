
#pragma once

#include <windows.h>
#pragma warning(push)
#pragma warning(disable : 4201)
	#include <vfw.h>
#pragma warning(pop)
#include "IVideo.h"
#include "IDynamicTexture.h"

#define STATUS_NOT_OPEN		0
#define STATUS_OPENED		1
#define STATUS_PLAYING		2
#define STATUS_STOPPED		3
#define STATUS_PAUSED		4

class CVideo : public IVideo, public IDynamicTexture
{
public:
	CVideo();
	~CVideo();

public:
	// ������ ���������������
	void Start();
	// ���������� ���������������
	void Stop();
	// �����
	void Pause();
	// ���������� ������ ����� �����
	void Play();

	// ���������� ����������������� �������� � �������������
	unsigned int GetDuration();
	// ����������/����� ����������� ���������������
	// �� ��������� ������������� ���
	void SetLoop(bool isLooped);

	// ������� ������� ������� � �������������
	int GetPosition();
	// ���������� ������� ������� � �������������
	void SetPosition(unsigned int pos);

	// ������� ������� ����� �����
	int GetFrameNum();
	// ���������� ������� ����� �����
	void SetFrameNum(unsigned int num);

	// ���������� true, ���� ������������� ��������
	bool IsLooped();
	// ���������� true, ���� ���� ������������
	bool IsPlayed();
	// ���������� true, ���� ���������� �����
	bool IsPaused();

	// ���������� ������������ ��������
	IDynamicTexture* GetDynamicTexture();

		// ������� ���� � ���������� ���������� ��������
	bool Open(char* fileName);
	// ���������� �����
	void Close();

public:
	// �������� ��������� �������
	void Update();
	// �������� ��������
	void UpdateFrame();

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

	bool IsNeedUpdate();

	void Destroy();

	void* GetUserData(unsigned int userDataID);
	void SetUserData(unsigned int userDataID, void* data);

	void SetFreezState(bool /*isFreezed*/){};
	bool IsFreezed(){return false;};
	bool IsFirstFrameSetted(){return false;};

private:
	void UpdateTime();

	unsigned char m_currentStatus;
	unsigned char m_colorChannelCount;

	unsigned short m_width;
	unsigned short m_height;
	unsigned short m_textureWidth;
	unsigned short m_textureHeight;

	bool m_isLooped;
	bool m_isDirectChannelOrder;

	unsigned int m_duration;
	unsigned int m_position;
	unsigned int m_lastUpdateTime;
	unsigned int m_frameCount;

	unsigned int m_updateWidth;
	unsigned int m_updateHeight;
	unsigned int m_updateTextureWidth;
	unsigned int m_updateTextureHeight;
	unsigned int m_updateFrameNum;
	HDC m_updateHDC;
	HBITMAP m_updateBitmap;
	PGETFRAME m_updateGetFrame;

	int m_lastFrameNum;

	unsigned char* m_frameData;

	AVISTREAMINFO m_aviStreamInfo;
	PAVISTREAM m_aviStream;
	PGETFRAME m_getFrame; 
	HBITMAP m_bitmap;
	HDRAWDIB m_hdd;
	HDC m_hdc;

	void* m_userData[4];
};