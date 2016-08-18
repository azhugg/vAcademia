#include "StdAfx.h"
#include <algorithm>
#include <atldef.h>
#include "..\include\voipmanager.h"
#include "ILogWriter.h"

#include "DeviceList\WTMicContoller.h"

#include "..\include\Voip3DSoundCalcManager.h"
#include "DataBuffer2.h"
#include "include/PipeProtocol/ConnectToEVoipIn.h"

#include "rm.h"

CVoipManagerEVO::CVoipManagerEVO(oms::omsContext* aContext) :
mUseDS( false)
, micGainLevel( 0.0f)
, micActivationLevel( 0.2f)
, recFreq( 8000)
, voiceQuality( 0.3f)
, m_iLogMode( 1)
, voiceActivated( false)
, autoVolume( true)
//, MP_WSTRING_INIT(currentInDeviceGuid)
//, MP_WSTRING_INIT(currentOutDeviceGuid)
, MP_MAP_INIT(mapVoipUsers)
#if VOIPMAN_BUILD_L
, m_pUser3DPositioning( NULL)
, MP_MAP_INIT(mapVoip3DSoundCalcManagerID)
#endif
{
	context = aContext;	
	bTerminating = FALSE;

	CWTMicContoller micController;
	//currentInDeviceGuid = micController.GetCurrentInDeviceGuid();
	//currentOutDeviceGuid = micController.GetCurrentOutDeviceGuid();	
}

CVoipManagerEVO::~CVoipManagerEVO(void)
{
#if VOIPMAN_BUILD_L
	MapVoip3DSoundCalcManagerID::iterator it = mapVoip3DSoundCalcManagerID.begin(), end = mapVoip3DSoundCalcManagerID.end();
	for ( ; it != end; it++)
	{
		CVoip3DSoundCalcManager *pVoip3DSoundCalcManager = (CVoip3DSoundCalcManager*) it->second;
		if (pVoip3DSoundCalcManager)
			MP_DELETE(pVoip3DSoundCalcManager);
	}
	mapVoip3DSoundCalcManagerID.clear();
	m_pUser3DPositioning = NULL;
#endif	
}

//! ������������ � ������ ������� ��������� �����
voipError	CVoipManagerEVO::Connect( LPCSTR alpcGroupName, LPCSTR alpcUserName, LPCSTR alpcPassword, long long alAvatarID)
{
	ATLASSERT(false);
	return ERROR_NOERROR;
}

//! ��������������� ����������� � �������� ������� ��������� �����
voipError	CVoipManagerEVO::RestoreConnection()
{
	if( connectionStatus == CS_CONNECTING ||
		connectionStatus == CS_CONNECTED)
		return ERROR_NOERROR;

	return ConnectImpl();
}

//! ������� ���������� ���� � �������
voipError	CVoipManagerEVO::CreateServerTalkingBot( unsigned int aClientSideObjectID)
{
	return ERROR_NOERROR;
}

//! ������� ���������� ���� � �������
voipError	CVoipManagerEVO::DestroyServerTalkingBot( unsigned int aClientSideObjectID)
{
	return ERROR_NOERROR;
}

//! ������� �������� ��������� �����
void CVoipManagerEVO::Release()
{
	Destroy();
	MP_DELETE_THIS;
}

//! ������������ �� DirectSound ��� ��������������� �����
//! �������� �� ���������: 1
void CVoipManagerEVO::SetDirectSoundUsed( bool val)
{
	if( mUseDS == val)
		return;
	mUseDS = val;
	OnDirectSoundUsedChanged();
}


//! ������� �������� ������� � ���������. �������� �������� 100-4000. 1000 - �������� ���. 4000 - x4, 100-x0.1
//! �������� �� ���������: 1000
void CVoipManagerEVO::SetMicGainLevel(float val)
{
	if( micGainLevel == val)
		return;
	micGainLevel = val;
	OnGainLevelChanged();
}

//! ������� ������ �������� �������. �������� �������� 0-20. 
//! 0 - ������ ���������� ���������, 10 - ������ ���������� ������ ���� �� ������� ��� 50% ���������, 
//! 20 - ������ ������������ �� �����
//! �������� �� ���������: 5
void CVoipManagerEVO::SetMicActivationLevel(float val)
{
	if( micActivationLevel == val)
		return;
	micActivationLevel = val;
	OnActivationLevelChanged();
}

//! ������� ������������� ��� ������. ��������: 8000, 16000, 32000
//! �������� �� ���������: 8000
void CVoipManagerEVO::SetRecorderFreq(unsigned int  val)
{
	ATLASSERT( FALSE);
	if( recFreq == val)
		return;
	recFreq = val;
}

//! �������� �����. �������� �������� 1-10. 1 - ����������� ��������. 10 - ������������ ��������
//! �������� �� ���������: 4
void CVoipManagerEVO::SetVoiceQuality(float val)
{
	if( voiceQuality == val)
		return;
	voiceQuality = val;
	OnVoiceQualityChanged();
}

void CVoipManagerEVO::SetVoiceActivated(bool enabled)
{
	if( voiceActivated == enabled)
		return;

	voiceActivated = enabled;
	OnVoiceActivatedChanged();
}

//! ��������� ���������� �����
bool CVoipManagerEVO::IsDirectSoundUsed()
{
	return mUseDS;
}

float CVoipManagerEVO::GetMicGainLevel()
{
	return micGainLevel;
}

float CVoipManagerEVO::GetMicActivationLevel()
{
	return micActivationLevel;
}

unsigned int CVoipManagerEVO::GetRecorderFreq()
{
	return recFreq;
}

float CVoipManagerEVO::GetVoiceQuality()
{
	return voiceQuality;
}

void CVoipManagerEVO::Destroy()
{
}

int CVoipManagerEVO::GetUserIDByName( LPCWSTR apwsName)
{
	if( !apwsName)
		return -1;

	/*CComString	nameToFound( alpcName);
	nameToFound.MakeLower();*/
	std::wstring nameToFound = apwsName;
	std::transform( nameToFound.begin(), nameToFound.end(), nameToFound.begin(), ::towlower);	
	MapVoipUsers::iterator it = mapVoipUsers.begin(), end = mapVoipUsers.end();
	for ( ; it!= end; it++)
	{
		//CComString& userName = (it->second).name;
		if( nameToFound == (it->second).name.c_str())
		{
			return (int)it->first;
		}
	}
	return -1;
}

LPCTSTR CVoipManagerEVO::GetUserNameByID( int aiAvatarID )
{
	MapVoipUsers::iterator it = mapVoipUsers.find( aiAvatarID );
	if ( it != mapVoipUsers.end())
		return (LPCTSTR)((it->second).name.c_str());
	return NULL;
}

long CVoipManagerEVO::GetUserVolume( int aiAvatarID)
{
	MapVoipUsers::iterator it = mapVoipUsers.find( aiAvatarID );
	if ( it != mapVoipUsers.end())
	{
		return (it->second).volume;
	}
	return -1;
}

bool CVoipManagerEVO::SaveUserVolume( int aiAvatarID, long adUserVolume)
{
	MapVoipUsers::iterator it = mapVoipUsers.find( aiAvatarID );
	if ( it != mapVoipUsers.end())
	{		
		if( adUserVolume != (it->second).volume)
		{
			UserVolume userVolume;
			userVolume.name = (it->second).name;
			userVolume.volume = adUserVolume;
			mapVoipUsers.erase(it);
			mapVoipUsers.insert( MapVoipUsers::value_type( aiAvatarID, userVolume));
			return true;
		}
	}
	return false;
}

#if VOIPMAN_BUILD_L
CVoip3DSoundCalcManager *CVoipManagerEVO::GetVoip3DSoundManagerID( int aiAvatarID )
{
	MapVoip3DSoundCalcManagerID::iterator it = mapVoip3DSoundCalcManagerID.find( aiAvatarID );
	if ( it != mapVoip3DSoundCalcManagerID.end())
		return ( CVoip3DSoundCalcManager *)(it->second);
	return NULL;
}
#endif

#include <math.h>

void CVoipManagerEVO::HandleVoiceData( int nUserID, int nSampleRate, const short* pRawAudio, int nSamples)
{		
}

bool CVoipManagerEVO::SetUserPosition( int nUserID, float x, float y, float z)
{
	return false;
}

//! ���������� ��������� ����� ������� ������
bool CVoipManagerEVO::OnBeforeStartRecording()
{
	//recordIsOnGoing = true;
	return true;
}

bool CVoipManagerEVO::SetUserVolume( const wchar_t* alpwcUserLogin, double adUserVolume)
{
	ATLASSERT( FALSE);
	return false;
}

//! ������������� ������� �������� ��������� �� ������������
bool CVoipManagerEVO::SetUserGainLevel( const wchar_t* alwpcUserLogin, double adUserGainLevel)
{
	ATLASSERT( FALSE);
	return false;

}

//! ���������� ��� ��������� ����� - ����� ��� ���� � �����
voipAudioSource CVoipManagerEVO::GetAudioSource()
{
	return audioSource;
}

//! ���������� ����� ��������� ���������. �������������� ��� ������
bool CVoipManagerEVO::GetAutoVolume()
{
	return autoVolume;
}

//! ������������� ����� ��������� ���������. �������������� ��� ������
void CVoipManagerEVO::SetAutoVolume(bool abAuto)
{
	autoVolume = abAuto;
	OnAutoVolumeChanged();
}

void CVoipManagerEVO::OnPlayConnectionLost()
{
	//SetEvoVoipClientWorkState( ECST_PLAYING, ECWS_CLOSED, 0);
}

void CVoipManagerEVO::OnPlayConnectionRestored()
{
	// ??
}

/*void CVoipManagerEVO::WriteLogLine(LPCSTR alpString)
{
	if( alpString && context->mpLogWriter)
	{
		context->mpLogWriter->WriteLnLPCSTR( alpString, ltDefault);
	}
}*/

//! �������������� ���������� ����������
bool CVoipManagerEVO::IsAutoGaneControlEnabled()
{
	ATLASSERT( false);
	return false;
}
bool CVoipManagerEVO::SetAutoGaneControlEnabled(bool abEnabled)
{
	ATLASSERT( false);
	return false;
}
//! �������������� ���������� �����
bool CVoipManagerEVO::IsDenoisingEnabled()
{
	ATLASSERT( false);
	return false;
}
bool CVoipManagerEVO::SetDenoisingEnabled(bool abEnabled)
{
	ATLASSERT( false);
	return false;
}

//! ���������� ��� ��������� � �����
void CVoipManagerEVO::OnChannelJoined(const char* alpcChannelName)
{
	ATLASSERT(false);
}

voipError CVoipManagerEVO::ConnectToSelfVoiceChannel()
{
	ATLASSERT(false);
	return 0;
}

//! ���� � ����� ������������� ������ ������
int CVoipManagerEVO::EnterSelfRecordingMode()
{
	ATLASSERT(false);
	return true;
}


void CVoipManagerEVO::SetSRModeState( voip::voipSRModeStatusChanged	aState)
{
	//selfRecordingIsOnGoing = aState;
	//CALLBACK_ONE_PARAM(onSRModeStatusChanged, aState);
}