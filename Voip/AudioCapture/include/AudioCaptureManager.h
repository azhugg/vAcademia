#pragma once

#include "../AudioCapture.h"

class CDefaultAudioDeviceManager;
class CAudioRepeater;

class VOIPAUDIOCAPTURE_API CAudioCaptureManager
{
public:
	CAudioCaptureManager(void);
	~CAudioCaptureManager(void);


public:

	/** �������������� ������ �����, �������� ��������� ����.
	1. ��������� ����� ���������� � ������ Virtual Cable 1
	2. ���� ���������� �� ������� ���������� ������
	3. ���������� ������� ���������� ��������������� � ������� �����, ��� �� ����������� ��������������
	4. ��������� AudioRepeater � Virtual Cable 1 �� ������� ���������� ���������������
	5. ������������� Virtual Cable 1 � �������� ��������� ���������� ��������������� � ������
	��� ������������� ������ � ����� �� ����� ���������� ������
	\return VOIPAC_ERROR
		ACERROR_NOERROR - ��� ��
		ACERROR_NOAUDIOCABLEINSTALLED - �� ������ ����������� VAC
		ACERROR_REPEATERSTARTFAILED - �� �������� ��� 4
		ACERROR_PRIMARYCHANGEFAILED - �� �������� ��� 5
		ACERROR_ISBISY - ��� ��������

	\details
	*/
	VOIPAC_ERROR	StartCapture();

	/** ���������� ������ ����� � ��������������� �������� ���������� �� ���������
	1. ������������� AudioRepeater � Virtual Cable 1 �� ������� ���������� ���������������
	2. ��������������� ����������� ������� ���������� ��������������� � ������� �����
	\return VOIPAC_ERROR
	ACERROR_NOERROR - ��� ��
	ACERROR_DEFAULTSRESTOREFAILED - �� �������� ��� 2
	ACERROR_DEFAULTALLREADY - �� ��� ������� StartCapture
	\details
	*/
	VOIPAC_ERROR	StopCapture();

	/** ���������� ��� �������� ���������� ���������������
	\param wchar_t * alpcwBuffer -
	\param unsigned int bufferSize -
	\return VOIPAC_ERROR
	ACERROR_NOERROR
	\details
	*/
	const wchar_t* GetReplacedAudioDeviceName();

	/** ��������������� ���������� ��������������� �� ��������� 
	\param wchar_t * alpcwBuffer -
	\param unsigned int bufferSize -
	\return VOIPAC_ERROR
		ACERROR_NOERROR
		ACERROR_OUTDEVICENOTFOUND - �� ����� ����������, ������� ����� ������������
		ACERROR_DEVICECHANGEFAILED - �� ������ ��������
		ACERROR_DEFAULTALLREADY - �� �������� ������
	\details
	*/
	VOIPAC_ERROR	RestoreReplacedDeviceAfterError( const wchar_t* alpwcDeviceName);

private:
	bool							captureOnProgress;
	CDefaultAudioDeviceManager*		m_pDefaultAudioDeviceManager;
	CAudioRepeater*					m_pAudioRepeater;
};
