#pragma once

#include "../../nengine/interfaces/idynamictexture.h"
#include "TextureLoader.h"
#include "SharingContext.h"
#include "../include/view/RemoteDesktopWindow.h"

class VASEFramesDifPack;

typedef struct _VASE_FRAME
{
	VASEFramesDifPack*	pack;
	bool				isKeyframe;
} VASE_FRAME;

//////////////////////////////////////////////////////////////////////////
class DESKTOPMAN_API CDynamicTextureImpl : public IDynamicTexture
{
public:
	CDynamicTextureImpl(SHARING_CONTEXT* apContext);
	virtual ~CDynamicTextureImpl();

public:
	// ���������� ����, ��� ��� �� �������
	bool	IsLive();
	// ��������� ����� ����
	void	AddVASEPack(VASEFramesDifPack* aPack, bool abKeyFrame);
	// ������������� ����� ������ � ����������
	void	EnableNormalMode();

	
	void UseNewCodec()
	{
		useNewCodec = true;
	}
	void NotUseNewCodec()
	{
		useNewCodec = false;
	}
	void SetViewWindow(CRemoteDesktopWindow* window);
	void DesktopManDestroy();
	void SetTarget( LPCSTR alpcName);
	std::string GetTextureInfo();
	void ReturnRealSize();
	void SetTextureSizeBeforeResize(int width, int height);
	void ShowLastFrame();
public:
	// �������� ��������� ������� (�� ��������);
	virtual void Update();
	// �������� ��������
	virtual void UpdateFrame();

	// ������� ������ �� ������ � ���������� ���������� �����	
	virtual void* GetFrameData();

	// ������� ������ �������� �������� � ��������
	virtual unsigned short GetSourceWidth();
	// ������� ������ �������� �������� � ��������
	virtual unsigned short GetSourceHeight();
	// ������� ������ �������������� �������� � ��������
	virtual unsigned short GetTextureWidth();
	// ������� ������ �������������� �������� � ��������
	virtual unsigned short GetTextureHeight();
	// ���������� ������ �������������� ��������
	virtual void SetTextureSize(int width, int height);
	// ������� ���������� �������� �������, 3 ��� RGB, 4 ��� RGBA
	virtual unsigned char GetColorChannelCount();
	// ������� �������� ������� ������ (RGB); ��� �������� (BGR);
	virtual bool IsDirectChannelOrder();

	// ����� �� ��������� �������� (��� ������ � ���������� �� ����������?);
	virtual bool IsNeedUpdate();

	// ������� ����
	// (��������� ��� ����, ����� nengine ��� ��� ������ ����� ��� ��������� �������);
	virtual void Destroy();

	void* 	GetUserData(unsigned int userDataID);
	void 	SetUserData(unsigned int userDataID, void* data);

	void SetFreezState(bool isFreezed);
	bool IsFreezed();
	bool IsFirstFrameSetted();

private:
	void	MakeWhiteBackBufferBitmap();
	void	OnNewKeyframeReceived(VASEFramesDifPack* aPack);
	void	SetKeyFrameSize(CSize aSize);
	void	CleanUpFrames();
	HBITMAP CreateDIBSection( unsigned int auWidth, unsigned int auHeight
								, unsigned int auBPP, void** lppBits
								, BITMAPINFO &bmpInfo, bool abReverseHMode = true);
	void	CopyFrameToBackbuffer( unsigned int auWidth, unsigned int auHeight
									, void* lpData, unsigned int auSize
									, CPoint position, bool abReverseHMode);
	void	DrawStatistics( CDC& cdc);
	bool	DestroyFromDesktopMan();
	bool	DestroyFromNengine();
	bool	DestroyFromUnpacing();
	bool	CanDelete();
	void	Release();
	void	Clear();

private:
	SHARING_CONTEXT* context;
	CRemoteDesktopWindow* viewWindow;
	bool useNewCodec;
	typedef MP_VECTOR<VASE_FRAME>	VecFrames;
	typedef VecFrames::iterator VecFramesIt;
	VecFrames	frames;
	bool	isChanged;
	bool	isDestroyed;
	bool	isOneKeyReceived;
	bool	isDestroyedInDesktopMan;
	bool	isDestroyedInEngine;
	bool	m_isFreezed;

	// last keyframe parameters
	CSize			keyFrameSize;
	unsigned int	keyFrameBPP;

	// Texture Bitmap params
	CDC				backBufferDC;
	CBitmap			backBufferBitmap;
	CSize			backBufferSize;
	CSize			textureRequestSize;
	void*			backBufferBits;
	unsigned int	backBufferStride;
	// �������� ����������� � �������� ��������
	CDC				backBackBufferDC;
	CBitmap			backBackBitmap;
	void*			backBackBits;

	// Statistics
		// ����� ���������� ���������� ���� �� ������
	unsigned int	totalBytesReceived;
		// ���������� ���������� �� ��������� ����� ����
	unsigned int	lastFrameBytesReceived;
		// fps ���������� ��������
	float			updateFPS;
		// fps ��������� ������� �������
	float			receivedFPS;

	CTextureLoader	textureLoader;
	CCriticalSection csFrames;
	CCriticalSection csDestroy;

	bool			normalModeEnabled;
	// isUnpackingNow - true - ����� ���������� ����� ������, �������� ������ ����� ��� ��������� � UpdateFrame
	bool			isUnpackingNow;
	bool			m_deleteResultGiven;
	bool			m_lostDestination;

	void*			m_userData[4];
	CComString		destination;

	long			m_countRecvFrames;
	long			m_countDrawFrames;
	long			m_timeFromLastRedraw;
	long			m_timeFromLastRecvFrame;


	int				m_textureWidthBeforeResize;
	int				m_textureHeigthBeforeResize;

	bool			m_isNeedToSetKeyFrameSize;

	long			m_totalSizeFrame;

};