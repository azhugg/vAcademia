// nrmVideo.h: interface for the nrmVideo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_nrmVideo_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_)
#define AFX_nrmVideo_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "nrmanager.h"
#include "nrmObject.h"
#include "nrmMOIVisibleStub.h"
#include "Sprite.h"
#include "IVideo.h"

class nrmObject;

class nrmVideo :	public nrmObject,
					public nrmMOIVisibleStub,
					public moIButton,
					public moIContinuous,
					public moIVideo
{
public:
	nrmVideo(mlMedia* apMLMedia);
	virtual ~nrmVideo();

// ���������� moMedia
public:
	moIContinuous* GetIContinuous(){ return this; }
	moILoadable* GetILoadable(){ return this; }
	moIVisible* GetIVisible(){ return this; }
	moIButton* GetIButton(){ return this; }
	void FrozenChanged();

// ����������  moILoadable
public:
	bool SrcChanged(); // �������� ������ �������
	bool Reload(){return false;} // ���������� ������� �� �������� src
// ����������  moIVisible
public:
	// ��������
	void AbsXYChanged(); // ���������� ���������� ��������� 
	void SizeChanged();
	void AbsDepthChanged(); // ���������� ���������� �������
	void AbsVisibleChanged(); // ���������� ���������� ���������
	void AbsOpacityChanged(); // ���������� ���������� ���������
	mlSize GetMediaSize(); 
	int IsPointIn(const mlPoint /*aPnt*/){ return -1; }

// ����������  moIButton
public:
	void onKeyDown(){}
	void onKeyUp(){}

	void onSetFocus(){}
	void onKillFocus(){}
	void AbsEnabledChanged(){}

// ����������  moIContinuous
public:
	// ��������
	void PlayingChanged();	// ������/��������� ������������
	void FrameChanged();	// ����������� �� ����� �����
	void PosChanged();	// ����������� �� ����� ����� (ms)
	unsigned long GetCurPos();	// ����������� ������� �������� (����������������� � ms)
	long GetDuration();	// ����������� �������� _duration (����������������� � ms)
	//
	void setCheckPoints(long* /*apfCheckPoints*/, int /*aiLength*/){}
	long getPassedCheckPoint(){ return 0; }

// ����������  moIVideo
public:
	void	OnLoaded();
	void	WindowedChanged();
	void	OnSetRenderManager();

	IVideo*				m_video;
};

#endif // !defined(AFX_nrmVideo_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_)
