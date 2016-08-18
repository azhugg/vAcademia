#pragma once

// Received Type - ���� ����������� ���������
#define RT_RecordStart					1  // ������ ��������
#define RT_RecordStop  					2  // ��������� ������
#define RT_VoipServerRequest			3  // ������ ���������� � ��������� �������
#define RT_VoipFileCreated				4  // ������ ����
#define RT_RecordStartResult			5  // ������ ��� ������� EVO �������
#define RT_RecordFinished				6  // ������ ������ ��������
#define RT_AudioPlayed					7  // ����� ���� ����������
#define RT_MixedAudioCreated			8  // ����� ������������ ��������� ������������� ����
#define RT_PlayFinished					9  // ������ ��������������� ��������
#define RT_PlaySessionConnected			10 // ������������ � ������ ���������������
#define RT_AudioFilesInfo				11 // ������ ������ ��� ����������
#define RT_EvoClientWorkStateChanged	12 // ��������� ������ ���������� EvoVOIP �������
#define RT_Notified						13 // �������� �����������

// ���� �����������. ��. RT_Notified � VoipSession::public const byte nm..
#define NC_SelfRecordStarted			1 // �������� ������ ������������ ������
#define NC_SelfRecordStopped			2 // ����������� ������ ������������ ������
#define NC_SelfPlayingStarted			3 // �������� ��������������� ����������� ������������ ������
#define NC_SelfPlayingStopped			4 // ����������� ��������������� ����������� ������������ ������

// Send Type - ���� ������������ ���������
#define ST_RecordStart				1  // ������ ��������
#define ST_RecordStop  				2  // ��������� ������
#define ST_ChangeServer				3  // ����� ������� �� ����� ������
#define ST_RecordPause 				4  // ������������ ������. ���� ��������������� �� ������
#define ST_RecordDelete				5  // �������� ������
#define ST_PlaySessionStart			6  // ������ ������ ���������������
#define ST_PlayUserAudioFile		7  // ��������� ���������� ���� � ��������� ������
#define ST_PlaySessionStop			8  // ��������� ������ ���������������
#define ST_ChangePlayServer			9  // ����� ������� �� ����� ���������������
#define ST_SetUserName				10 // ������������� �� ������� ��� ������ ������������
#define ST_ServerAudioCommand		11 // ���������������� ��������������� ������������ ������
#define ST_CopyAudioFile			12 // �������� ��������� ���� �� ��� ������������ ������ � �����
#define ST_ForceSynchronization		13 // �������� ������� �������������� �������������
#define ST_PauseUserAudio			14 // �������� ������ �� ��������� ��������������� ����������� ��������� �����
#define ST_ResumeUserAudio			15 // �������� ������ �� ����������� ��������������� ����������� ��������� �����
#define ST_JoinToRecord				16 // �������� ����������� � ������ ������������� ������. ����� ��� ����, ����� �������� ����� � ������ ��������
#define ST_RecordDeletes			17 // �������� ������
#define ST_RecordExport				18 // ������� ������
#define ST_RecordExportFile			19 // ������� ��������� ����� �� ������
#define ST_RecordImportFile			20 // ������ ��������� ����� � ������
#define ST_RemoveUserAudio			21 // ������ ��� ����� ������������
#define ST_StartSelfRecording		22 // ������ ������ ������ ������
#define ST_ResumeSelfRecording		23 // ������������� � ������ ������ ������
#define ST_PauseSelfRecording		24 // ���������� ������ ������ ������
#define ST_PlaySelfRecording		25 // ������������� ������ ������ ������
