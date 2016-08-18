#pragma once

#include "../VoipManager.h"
#include "VoiceIPClient.h"
#include "VoipBase.h"

class CMicrophoneControllerThread;
class CWavePlaying;

class CVoipManagerH323 : public CVoIPBase, public CVoipManager
{
public:
	CVoipManagerH323(oms::omsContext* aContext);
	~CVoipManagerH323(void);

	// ���������� voip::IVoipManager
public:
	//! ������������ � ������������� ������� ��������� �����
	voipError	Connect( LPCSTR alpcServer, LPCSTR alpcGroupName
						, LPCSTR alpcUserName, LPCSTR alpcPassword = NULL, SHORT aiAvatarID = -1);

	//! ����������� �� �������� ������� ��������� �����
	voipError	Disconnect();

	//! ������� ���������� ���� � �������
	voipError	CreateServerTalkingBot( unsigned int aClientSideObjectID);

	//! ������� ���������� ���� � �������
	voipError	DestroyServerTalkingBot( unsigned int aClientSideObjectID);

public:
	// ��������� �������
	virtual void onCommand( CVoiceError *voiceError){}
	virtual void onCommand( CVoiceError::VOIP_ERRORS iErrorCode, LPCTSTR sCmd, LPCTSTR sPar){}
	virtual void OnConnectionEstablished( BOOL bSuccess);
	virtual void OnConnectionCleared( BOOL bError){}
	virtual bool Call( char* apProcName, char* aArgTypes, ...){ return true;}

protected:
	CVoiceIPClient	voiceClient;

	CMicrophoneControllerThread *micrCtrl;
	//CUpdaterLexThread *updaterLex;
	CWavePlaying *wavePlaying;
	//CTestPlay3DSoundThread *testPlay3DSound;
};
