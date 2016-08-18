
#if !defined(AFX_NRMIMAGEFRAME_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_)
#define AFX_NRMIMAGEFRAME_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_

#include <string>
#include "Paper.h"
#include "Color4.h"
#include "nrmanager.h"
#include "nrmObject.h"
#include "nrmMOIVisibleStub.h"
#include "ToolPalette.h"
#include "ToolsParameters.h"
#include "IChangesListener.h"

#include "Sprite.h"

class nrmImageFrame :	public nrmObject,
						public nrmMOIVisibleStub,
					public moIButton,
					public moIImageFrame,
					public IChangesListener
{
public:
	nrmImageFrame(mlMedia* apMLMedia);
	virtual ~nrmImageFrame();

// ���������� moMedia
public:
	moILoadable* GetILoadable(){ return this; }
	moIVisible* GetIVisible(){ return this; }
	moIButton* GetIButton(){ return this; }
	moIImageFrame* GetIImageFrame(){ return this; }
	
public:
	void OnChanged(int eventID);

// ����������  moILoadable
public:
	bool SrcChanged(); 
	bool Reload(){return false;} // ���������� ������� �� �������� src
	void LoadingPriorityChanged( ELoadingPriority aPriority);

// ����������  moIVisible
public:
	// ��������
	void AbsXYChanged(); // ���������� ���������� ��������� 
	void SizeChanged();
	void AbsDepthChanged(); // ���������� ���������� �������
	void AbsVisibleChanged(); // ���������� ���������� ���������
	void AbsOpacityChanged(); // ���������� ���������� ���������
	mlSize GetMediaSize(); 
	void Crop(short awX, short awY, MLWORD awWidth, MLWORD awHeight);
	mlRect GetCropRect();
	void SetMask(int aiX, int aiY, int aiW, int aiH);
	int IsPointIn(const mlPoint aPnt);

// ����������  moIButton
public:
	void onKeyDown(){}
	void onKeyUp(){}

	void onSetFocus(){}
	void onKillFocus(){}
	void AbsEnabledChanged();

// ����������  moIImageFrame
public:
	void MarginsChanged();
	void TileChanged();

// nrmObject
public:
	CPaper*		m_paper;

protected:
	void UpdateMask( nrmImage* apMask);

private:
	void SetLoadingPriority(unsigned char aPriority);

	bool	m_isFirstLoaded;

	unsigned char m_loadingPriority;

	MP_WSTRING m_src;
};

#endif // !defined(AFX_NRMIMAGEFRAME_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_)
