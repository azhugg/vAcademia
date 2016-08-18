#pragma once

typedef unsigned int VOIPAC_ERROR;

#define	ACERROR_NOERROR						0	/*��� ��*/
#define	ACERROR_NOAUDIOCABLEINSTALLED		1	/*�� ������ ����������� VAC*/
#define	ACERROR_PRIMARYCHANGEFAILED			2	/*�� ������� ������� ������� ����������*/
#define	ACERROR_REPEATERSTARTFAILED			3	/*�� ������� ��������� AudioRepeater*/
#define	ACERROR_DEFAULTSRESTOREFAILED		4	/*�� ������� ������������ ���������� �� ���������*/
#define	ACERROR_INDEVICENOTFOUND			5	/*�� ������� ����� ��������� ���������� �����*/
#define	ACERROR_OUTDEVICENOTFOUND			6	/*�� ������� ����� ��������� ���������� ������*/
#define	ACERROR_INCORRECTAUDIOPARAMES		7	/*�������� ��������� �����*/
#define	ACERROR_DIRECTSOUNDERROR			8	/*������ DirectSound*/
#define	ACERROR_NOTINITIALIZED				9	/*�� ���� �������������*/
#define	ACERROR_INDEVICEPARAMSFAILED		10	/*��������� �� �������� ��� ���������� ���������������*/
#define	ACERROR_OUTDEVICEPARAMSFAILED		11	/*��������� �� �������� ��� ���������� ������*/
#define	ACERROR_ISBISY						12	/*���������� �� ����� ������ �������� ��������� �����*/
#define	ACERROR_DATATRUNCATED				13	/*����� ������ ��� ������, ����� �������� �� ���*/
#define	ACERROR_BUFFERLOST					14	/*����� �������*/
#define	ACERROR_INDEVICEFAILED				15	/*������ ������ �������� ������*/
#define	ACERROR_OUTDEVICEFAILED				16	/*������ ������ �������� ������*/
#define	ACERROR_VACNOTINSTALLED				17	/*�� ������ ������� VAC*/
#define	ACERROR_DEVICECHANGEFAILED			18	/*������ ����� ����������*/
#define	ACERROR_DEFAULTALLREADY				19	/*���������� ��� �� �������*/
#define	ACERROR_BUFFERTOOSMALL				20	/*�� ������� ������*/
#define	ACERROR_ACCESSDENIED				21	/*�� ������� ����*/
#define	ACERROR_COMFAILED					22	/*������ �������� COM ����������*/

#define	ACERROR_NOTIMPLEMENTED			0x8FFFFFFF	/*����� ��� �� ����������*/
#define	ACERROR_UNKNOWNERROR			0xFFFFFFFF	/*����������� ������*/