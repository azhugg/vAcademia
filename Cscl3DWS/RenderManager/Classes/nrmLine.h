// nrmLine.h: interface for the nrmLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_nrmLine_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_)
#define AFX_nrmLine_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "nrmanager.h"
#include "nrmObject.h"
#include "nrmMOIVisibleStub.h"
#include "Sprite.h"
#include "IVideo.h"

class nrmObject;

class nrmLine :	public nrmObject,
					public nrmMOIVisibleStub,
					public moIButton,
					public moILine
{
public:
	nrmLine(mlMedia* apMLMedia);
	virtual ~nrmLine();

// ���������� moMedia
public:
	moIVisible* GetIVisible(){ return this; }
	moIButton* GetIButton(){ return this; }
	void FrozenChanged(){}

// ����������  moIVisible
public:
	// ��������
	void AbsXYChanged(); // ���������� ���������� ��������� 
	void SizeChanged();
	void AbsDepthChanged(); // ���������� ���������� �������
	void AbsVisibleChanged(); // ���������� ���������� ���������
	void AbsOpacityChanged(); // ���������� ���������� ���������
	mlSize GetMediaSize(){ mlSize sz; ML_INIT_SIZE(sz); return sz; } 
	int IsPointIn(const mlPoint aPnt); // -1 - not handled, 0 - point is out, 1 - point is in

// ����������  moIButton
public:
	void onKeyDown(){}
	void onKeyUp(){}

	void onSetFocus(){}
	void onKillFocus(){}
	void AbsEnabledChanged(){}

// ����������  moILine
public:
	virtual void XY2Changed();
	void ColorChanged();
	virtual void ThicknessChanged();
	virtual void SmoothChanged(){}

	moILine* GetILine(){ return this; }

	CLine* m_line;
};

#endif // !defined(AFX_nrmLine_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_)
