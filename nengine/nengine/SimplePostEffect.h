
#pragma once

#include "BasePostEffect.h"
#include "FrameBuffer.h"

class CSimplePostEffect : public CBasePostEffect
{
public:
	CSimplePostEffect();
	~CSimplePostEffect();

	void OnSceneDrawStart(CViewport* viewport);
	void OnSceneDrawEnd(CViewport* viewport, CRenderTarget* renderTarget);
	void OnLoadFromXML(std::wstring& fileName);
	void SaveScreen(std::wstring fileName, SCropRect* const rect);

	void PrepareForReading();
	void EndReading();

	unsigned char GetMultisamplePower()const;

private:
	void GetTextureSize(int* textureWidth, int* textureHeight);

	// ��������� ��������� ���������� ���� � ��� ������� ��������� ����������� ����� �������� �������� ��������
	bool IsTextureSizeChanged(); 

	// ������������� �������
	void InitBuffers(); 

	CFrameBuffer* m_buffer;

	int m_bufferTexID;

	int m_textureWidth;
	int m_textureHeight;
};