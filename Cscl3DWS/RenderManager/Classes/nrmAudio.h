// nrmAudio.h: interface for the nrmAudio class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_nrmAudio_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_)
#define AFX_nrmAudio_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../res/res.h"
#include "nrmanager.h"
#include "nrmObject.h"
#include "iasynchresourcehandler.h"
#include "Mutex.h"
#include "../sm/sm.h"

#define SND_BUILD_H_STREAMING 1

class nrm3DObject;
class CBSpline;
class CSplinePoints;

class nrmAudio :	
					public nrmObject,
					public moIContinuous,
					public moISpeech,
					public IAsynchResourceHandler,
					public sm::smISoundNotify,
					public moIAudible
{
	friend class nrmCharacter;
	friend class nrm3DObject;
public:
	nrmAudio(mlMedia* apMLMedia);
	virtual ~nrmAudio();

// ���������� moMedia
public:
	moILoadable* GetILoadable(){ return this; };
	moIContinuous* GetIContinuous(){ return this; };
	moISpeech* GetISpeech(){ return this; };
	moIAudible* GetIAudible(){ return this; };
	void PropIsSet();

// ����������  moILoadable
public:
	bool SrcChanged(); // �������� ������ �������
	bool Reload(){return false;} // ���������� ������� �� �������� src

// ����������  moIAudible
public:
	bool EmitterTypeChanged(); // ��������� ���� �����
	bool SplineSrcChanged(); // �������� ������ ������� �������
	bool VolumeChanged(); // 
	bool LocationChanged(); // ��������� ������� ��� �����

// ����������  moIContinuous
public:
	void update(DWORD delta);
	// ��������
	void PlayingChanged();	// ������/��������� ������������ �����
	void FrameChanged();	// ����������� ���� �� ����� �����
	void PosChanged();	// ����������� ���� �� ����� ����� (ms)
	long GetDuration();	// ����������� �������� _duration (����������������� � ms)
	//
	void setCheckPoints(long* /*apfCheckPoints*/, int /*aiLength*/){}
	long getPassedCheckPoint(){ return 0; }
	unsigned long GetCurPos();

// ����������  moISpeech
public:
	long GetCurrentPos(){ return (long) GetCurPos();};

// ����������  sm::smISoundNotify
public:
	void OnLoaded( int iError);
	void OnStopped( int iReason);

// ����������  res::IAsynchResourceHandler
public:
	void OnAsynchResourceLoaded(IAsynchResource* asynch);
	void OnAsynchResourceError(IAsynchResource* asynch);
	void OnAsynchResourceLoadedPersent(IAsynchResource* asynch, unsigned char percent);

protected:
	BOOL OpenSound( const wchar_t* pwcSrc);
	void onOpened();
	void PauseLoading( IAsynchResource* asynch);

public:
	BOOL IsLoading();
	BOOL IsLoaded();

public:
	enum{ ON_LOADED = 101, ON_STOPPED, ON_DESTROY};

	void set_p3dobj( nrm3DObject *ap3DObj, ml3DPosition aPos3D);
	nrm3DObject *get_p3dobj();
	double GetFactorDist();
	BOOL IsFactorDist();
	double GetMinDist();
	BOOL IsMinDist();
	void SetAudioMode( int iMode);
	void SetAudioVolume( float fVolume);
	void UpdateParams();
	int GetEmitterType();
	void SetPlayingEnabled( bool bPlayingEnabled);

protected:
	// ������������� �����
	int				m_iSoundId;

	// ��� �����
	int				m_iEmitterType;

	// ������� �������� � ������������� ����� (� ��)
	DWORD			m_dwPos;

	// ��������� �� ��, ��� ������ �������� ResLoader � ������ SoundMan
	BOOL			m_bLoaded;	

	// ��������� �� ��, ��� ������ �������� ResLoader
	BOOL			m_bLoading;

	// ��������� �� ��, ��� ���� ����������� SoundMan
	BOOL			m_bOpening;

	// ��������� �� ��, ��� ���� ������������� �� ����� �������� �������
	BOOL			m_bPlayingByLoading;
#if SND_BUILD_H_STREAMING

	// ��� �����, ��������� �� ����������� ������������ ����� �� ����� �������� �����
	BOOL			m_bStreaming;

	// ���-�� ������ ������������ �������
	DWORD			m_dwLoadedBytes;

	// ������������ �����
	DWORD			m_dwDuration;

	// ������ �����
	DWORD			m_dwSize;	

	// ��������� �� ������� ��������
	BOOL			m_bRemoving;

	// ����������� ������ �� �������� �����
	CMutex			m_removingMutex;

	// ����������� ������ �� �������� �����
	CMutex			m_loadingUpdateMutex;
#endif
	// ��������� �� 3D-������
	nrm3DObject		*m_p3DObj;

	// ������� ����� � 3D ����
	ml3DPosition	pos3D;

	// ����� ������ �������� �������
	DWORD			m_dwStartLoadTime;

	// ������ ����������� �������� �������
	IAsynchResource*	m_pAsyncResource;

	// ����������� ������� ���� ����� (� ��)
	double			m_dFactorDist;

	// ����������, �� ������� ��� ����� �� ����������� ��� �������� ������������ �� �����
	double			m_dMinDist;

	// �����: �������������� ���� ��� ��������� ��������������� �����
	enum{ DIS_PLAY, ENABLE_PLAY};
	int				m_iMode;

	// ������ �������������� ���� ���������� �� ������ m_iMode (�������� ��������)
	BOOL			m_bAlwaysPlaying;

	// ��� �������
	std::wstring	m_wsSrc; 

	// ������ ���������� ���������
	BOOL			m_bLoadNeed; 

	// ������� ��������� ����� (���)
	float			m_fVolume;

	// ������� ��������� ����� (���), �������� ��������
	// ��������� < 0 ��������, ��� ������� ��������� ����� �� ������� �� ����� (��� �������)
	int				m_iScriptVolume;

	// ���������� �������� ������ � �������� ������� � SoundMan
	BOOL			m_bNeedUpdate;	

	std::wstring	m_sScriptLocation;

#ifdef DEBUG
	int destroy_counter;
#endif
	
	// ������ ������
	CBSpline *m_pSpline;
	// ������ � �������� ����� �������
	CSplinePoints *m_pSplinePoints;

	// ������� ��������� �����
	enum{ PLAY, STOP_BY_USER, STOP_BY_ENDFILE, STOP_BY_DISTANCE, STOP_BY_DISALLOW};
	int		m_iStopReason;

	// ���� �������������
	BOOL	m_bPlaying;
	// ����������� ������ �� ��������� ������� �����: ������������ ��� ���������
	CMutex	m_playingMutex;
	// ���� ��������
	BOOL	m_bPlayed;
	// ���������� �� ������������ �����
	BOOL	m_bPlayingEnabled;
	// ������ �������, ������� ���� ���������� update
	//MP_VECTOR<TAudioEvent*> m_audioEvents;
	//CMutex	m_eventMutex;

public:
	void HandleOnLoaded( int iError);
	void HandleOnStopped( int iReason);
	void HandleOnDestroy();
	void Destroy();

private:
	//CEvent	evEventsUpdated;
};

#endif // !defined(AFX_nrmAudio_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_)
