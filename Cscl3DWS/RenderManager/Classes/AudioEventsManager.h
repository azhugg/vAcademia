// nrmAudioEventsManager.h: interface for the nrmAudio class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "ViskoeThread.h"

class nrmAudio;

struct TAudioEvent
{
	int type;
	int code;
	nrmAudio*	pAudio;

	TAudioEvent(nrmAudio*	apAudio, int aType, int aCode)
	{
		pAudio = apAudio;
		code = aCode;
		type = aType;
	}
};

class nrmAudioEventsManager : ViskoeThread::CThreadImpl<nrmAudioEventsManager>
{
public:
	nrmAudioEventsManager();
	virtual ~nrmAudioEventsManager();

// ���������� 
public:
	void	AddEvents( nrmAudio* apAudio, int aType, int aCode);
	void	RemoveEvents( nrmAudio* apAudio);

	DWORD	Run();

private:
	CEvent	evEventsUpdated;

	// ������ �������, ������� ���� ���������� update
	MP_VECTOR<TAudioEvent*> m_audioEvents;

	CCriticalSection	m_csEvents;
};