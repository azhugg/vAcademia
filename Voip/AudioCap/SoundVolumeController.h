#pragma once

#define MAXVOLUME (( WORD) 0xFFFF)

class CSoundVolumeController
{
public:
	CSoundVolumeController();
	virtual ~CSoundVolumeController();

protected:
	enum{ MIC_VOLUME, PLAY_VOLUME};
	//
	BOOL setVolume( int type, DWORD inout, DWORD ComponentType, DWORD dwVol);
	//
	BOOL getVolume( int type, DWORD inout, DWORD ComponentType, DWORD &dwVol);

public:
	// ��������� ������ ��������� ������ ���������
	BOOL setMicVolume( float coef);
	// ��������� ������ ��������� ������ ���������
	float getMicVolume();

	// ��������� ������ ��������� ������
	BOOL setRecordVolume( float coef);
	// ��������� ������ ��������� ������
	float getRecordVolume();

	// ��������� ������ ��������� ��������������� ����������
	BOOL setHeadPhoneVolume( float coef);
	// ��������� ������ ��������� ��������������� ����������
	float getHeadPhoneVolume();

	// ��������� ������ ��������� ���������������
	BOOL setPlayVolume( float coef);
	// ��������� ������ ��������� ���������������
	float getPlayVolume();

	void restore();

private:
	// ������� ����������� ������ ��������� ������ ���������
	float micVolumeCoef;
	// ���������� ����������� ������ ��������� ������ ���������
	float micVolumeCoefSaver;
	// ������� ����������� ������ ��������� ������
	float recordVolumeCoef;
	// ���������� ����������� ������ ��������� ������
	float recordVolumeCoefSaver;

	// ������� ����������� ������ ��������� ��������������� ���������
	float headPhoneVolumeCoef;
	// ���������� ����������� ������ ��������� ��������������� ���������
	float headPhoneVolumeCoefSaver;

	// ������� ����������� ������ ��������� ���������������
	float playVolumeCoef;
	// ���������� ����������� ������ ��������� ���������������
	float playVolumeCoefSaver;

};
