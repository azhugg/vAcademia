// nrmFlash.h: interface for the nrmFlash class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_nrmFlash_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_)
#define AFX_nrmFlash_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "nrmanager.h"
#include "nrmMOIVisibleStub.h"
#include "nrmActiveX.h"
#include "Sprite.h"
#include "iflash.h"

#include "Sprite.h"

//////////////////////////////////////////////////////////////////////////
//using namespace natura3d;

//////////////////////////////////////////////////////////////////////////
class nrmFlash :	public nrmObject,
					public nrmMOIVisibleStub,
					public moIButton,
					public moIFlash,
					public IChangesListener,
					public IFlashCallBack

{
	wstring DefCLSIDBySrc(const wchar_t* apwcSrc);
public:
	nrmFlash(mlMedia* apMLMedia);
	virtual ~nrmFlash();

	LPCRITICAL_SECTION GetCSForChildWindows();

	// ���������� moMedia
public:
	moILoadable* GetILoadable(){ return this; }
	moIVisible* GetIVisible(){ return this; }
	moIButton* GetIButton(){ return this; }
	moIFlash* GetIFlash(){ return this; }

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
	mlSize GetMediaSize(); 
	int IsPointIn(const mlPoint /*aPnt*/){ return -1; }

	// ����������  moIButton
public:
	// ��������
	void AbsEnabledChanged();
	// �������
	virtual void onSetFocus();
	virtual void onKillFocus();
	virtual void onKeyDown();
	virtual void onKeyUp();

// ���������� moIFlash
public:
	virtual void  setVariable(const wchar_t *var, const wchar_t *value);

	virtual int getScaleMode();
	virtual void setScaleMode(int);

	virtual int getWMode();
	virtual void setWMode(int);

	// ���������� ����� ������
	virtual long getVariable(const wchar_t *var, mlOutString &sRetVal);
	// ���������� ����� ����������
	virtual long callFunction(const wchar_t *request, mlOutString &sRetVal);

// ���������� IChangeListener
public:
	virtual void OnChanged(int /*eventID*/){}

// ���������� IFlashCallBack
public:
	virtual void OnAdjustSize(int& aiWidth, int& aiHeight); // �������� ������ ������� ������, �������� ������� ��������, �� ������� ��� ����� ��������������
	virtual void onFSCommand(const wchar_t *command, const wchar_t *args);

public:
	IDynamicTexture* GetDynamicTexture();

private:
	bool recreate_sprite(bool create = true);

	IFlash*				m_flash;

	CSprite* m_sprite;

};

#endif // !defined(AFX_nrmFlash_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_)
