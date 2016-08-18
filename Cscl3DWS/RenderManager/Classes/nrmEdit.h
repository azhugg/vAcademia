// nrmEdit.h: interface for the nrmEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NRMEDIT_H__FB2788A7_23A9_4F4D_95A4_E99BF48D9535__INCLUDED_)
#define AFX_NRMEDIT_H__FB2788A7_23A9_4F4D_95A4_E99BF48D9535__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////
#include "nrmanager.h"
#include "nrmObject.h"
#include "nrmMOIVisibleStub.h"

//////////////////////////////////////////////////////////////////////////
//using namespace natura3d;

//////////////////////////////////////////////////////////////////////////
class nrmEdit : public nrmObject,
				public nrmMOIVisibleStub,
				public moIButton,
				public moIInput
{
public:
	nrmEdit(mlMedia* apMLMedia);
	virtual ~nrmEdit();

// ���������� rmMedia
public:
    virtual moMedia* GetMO(){ return this; }

// ���������� moMedia
public:
    virtual moILoadable* GetILoadable() { return this; }
    virtual moIVisible*  GetIVisible()  { return this; }
    virtual moIButton*   GetIButton()   { return this; }
    virtual moIInput*    GetIInput()    { return this; }
	virtual void FrozenChanged();

	virtual void	OnSetRenderManager();

// ����������  moILoadable
public:
    virtual bool SrcChanged(); // �������� ������ �������
	virtual bool Reload(){return false;} // ���������� ������� �� �������� src

// ����������  moIVisible
public:
    virtual void AbsXYChanged();       // ���������� ���������� ��������� 
    virtual void SizeChanged();        // ��������� �������
    virtual void AbsDepthChanged();    // ���������� ���������� �������
    virtual void AbsVisibleChanged();  // ���������� ���������� ���������
    virtual void AbsOpacityChanged();  // ���������� ���������� ���������
	//////////////////////////////////////////////////////////////////////////
	virtual mlSize GetMediaSize();
    virtual mlRect GetCropRect();
    virtual int IsPointIn(const mlPoint aPnt);

// ����������  moIButton
public:
    virtual void AbsEnabledChanged();
    virtual void onSetFocus();
    virtual void onKillFocus();
    virtual void onKeyDown();
    virtual void onKeyUp();

// ����������  moIInput
public:
    virtual void ValueChanged();
    virtual void StylesChanged();
	virtual void PasswordChanged();
	virtual void MultilineChanged();
	virtual void TabGroupChanged();
	virtual void TabIndexChanged();
    virtual const wchar_t* GetValue() const;

	bool InsertText(const wchar_t* apwcText, int aiPos);

protected:
	// ���� ��� �������������� ������������ ��������
    bool				_in_ValueChanged;
};

#endif // !defined(AFX_NRMEDIT_H__FB2788A7_23A9_4F4D_95A4_E99BF48D9535__INCLUDED_)