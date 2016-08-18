#ifndef __AUDIO_SLICER_H_
#define __AUDIO_SLICER_H_

#include "Thread.h"

// ������� �������������
// ����� ����������� ��������� ����� �������� ���������� �������������
#define CHECKTIME_NORMAL_DURATION		10000
// ����� ����������� ��������� �� �������� ����� ������������� � ������ �������� ������������
//								� ����� �������� ����������� ����� �������������, ���� ���� ������������ ���-�� �������
#define CHECKTIME_TOOLONG_DURATION		20000

#define WAIT_SILENCE_TIME				500

class CRecordServerConnection;

class CAudioSlicer : public viskoe::CThreadImpl<CAudioSlicer>
{
public:
	CAudioSlicer( IVoipManager* apVolumeControl, CRecordServerConnection* apRecordServerConnection);
	~CAudioSlicer();

	// ���������� CThreadImpl<CCaptureWindow>
public:
	DWORD Run();

public:
	void	Reset();
	void	OnStartTalking( int uiUserID);
	void	OnStopTalking();

private:
	unsigned int GetCurrentSleepTime();
	void ForceSynchronization();

private:
	unsigned int	silenceTime;
	unsigned int	talkingTime;
	int				userID;
	viskoe::CEvent			talkingEvent;
	IVoipManager*	volumeControl;
	CRecordServerConnection* recordServerConnection;
};

#endif	// __AUDIO_SLICER_H_