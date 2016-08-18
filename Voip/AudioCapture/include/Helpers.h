#pragma once

#include <mmsystem.h>
#include <xstring>
#include <vector>

/*
��������� WAVEFORMATEX 20-� ���������� ��������������� ����������
*/
VOID GetWaveFormatFromIndex( INT nIndex, WAVEFORMATEX* pwfx );

/*
��������� ���� nBlockAlign � nAvgBytesPerSec � WAVEFORMATEX
*/
VOID FillWaveFormatAlignAndBPS( WAVEFORMATEX* pwfx );

/*
���������: ������ �� ����������� ��������� � �������������� �������� WAVEFORMATEX. 
�������� ����������� ����� ����� GetWaveFormatFromIndex
*/
bool IsAudioFormatEnumerated( unsigned int auSampleRate, unsigned int auChannelsCount,	unsigned int auBitsPerSample );

/*
����� GUID �������� ����������
*/
struct DEVICE_ENUMERATION_PARAMS
{
	MP_WSTRING		inDeviceName;
	GUID			outDeviceGuid;

	DEVICE_ENUMERATION_PARAMS():
	MP_WSTRING_INIT(inDeviceName)
	{

	}

};
BOOL CALLBACK DirectSoundEnumerationRoutine(LPGUID guid, LPCWSTR desc, LPCWSTR driver, LPVOID user);


typedef std::vector<DEVICE_ENUMERATION_PARAMS> VecOutDevicesParams;
typedef VecOutDevicesParams::iterator VecOutDevicesParamsIt;
/*
����� ���� ������� ����������
*/
BOOL CALLBACK DirectSoundEnumerateAllRoutine(LPGUID guid, LPCWSTR desc, LPCWSTR driver, LPVOID user);