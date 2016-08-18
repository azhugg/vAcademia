
#if !defined(AFX_nrmAnim_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_)
#define AFX_nrmAnim_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_

#include "Sprite.h"
#include "nrmObject.h"
#include "nrmMOIVisibleStub.h"
#include "NRManager.h"
#include "IChangesListener.h"

class nrmAnim :		public nrmObject,
					public nrmMOIVisibleStub,
					public moIButton,
					public moIContinuous,
					public IChangesListener
{
public:
	nrmAnim(mlMedia* apMLMedia);
	virtual ~nrmAnim();

// ���������� moMedia
public:
	moIContinuous* GetIContinuous(){ return this; }
	moILoadable* GetILoadable(){ return this; }
	moIVisible* GetIVisible(){ return this; }
	moIButton* GetIButton(){ return this; }

	void OnChanged(int eventID);

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
	void SetMask(int /*aiX*/, int /*aiY*/, int /*aiW*/, int /*aiH*/);
	int IsPointIn(const mlPoint /*aPnt*/); // -1 - not handled, 0 - point is out, 1 - point is in

// ����������  moIButton
public:
	void onKeyDown(){}
	void onKeyUp(){}

	void onSetFocus(){}
	void onKillFocus(){}
	void AbsEnabledChanged();

// ����������  moIContinuous
public:
	// ��������
	void PlayingChanged();	// ������/��������� ������������ 
	void FrameChanged();	// ����������� �� ����� �����
	void PosChanged();	// ����������� �� ����� ����� (ms)
	void LengthChanged(); // ���������� ���������� ������ � ����������� ��������
	void DurationChanged(); // ���������� ����������������� ����������� �������� (� ms)
	void FPSChanged(); // ��������� FPS ����������� ��������
	unsigned long GetCurPos();	// ����������� ������� �������� (����������������� � ms)
	long GetDuration();	// ����������� �������� _duration (����������������� � ms)

	void setCheckPoints(long* /*apfCheckPoints*/, int /*aiLength*/){}
	long getPassedCheckPoint(){ return 0; }

	CSprite* m_sprite;

private:
	bool m_isFirstLoaded;
};

#endif // !defined(AFX_nrmAnim_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_)
