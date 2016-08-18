#pragma once

class CRecordState
{
public:
	CRecordState();
	virtual ~CRecordState();

public:
	// ���������� ��������� ������
	void setRecordState( BOOL bRecordState);
	// ���������� ��� �����, � ������� ����� ���� ������ mp3-������
	void setFileName( wchar_t *sFileName);
	// ���������� ��� �����
	wchar_t *getFileName();
	// ���������� ��������� ������
	BOOL getRecordState();
	// ���������� ��������� ���������������� ����� ������ (���� ������ ������ �� 1152)
	//void setBlockReady( BOOL bBlockReady);
	// ���������� ��������� ���������������� ����� ������ (���� ������ ������ �� 1152)
	//BOOL getBlockReady();
	// ���������� ��������� �������� �����
	void setBackgroundNoise( BOOL bBackgroundNoise);
	// �������� ��������� �������� ����� (��������� �����)
	BOOL getBackgroundNoise();
	// ���������� ����� �������
	void setChannels( int channels);
	// ���������� ����� �������
	int getChannels();
	// ���������� �������
	void setSamples_per_sec( int samples_per_sec);
	// ���������� �������
	int getSamples_per_sec();
	// ���������� ����� ����� ������
	void setBits_per_sample( int bits_per_sample);
	// ���������� ����� ����� ������
	int getBits_per_sample();

private:
	// ��������� ������. �c�� true, �� ��������� ����������
	BOOL bRecordState;
	// ��� �����
	WCHAR sFileName[ MAX_PATH];
	//
	//BOOL bBlockReady;
	// true, �� ������� ���� (���)
	BOOL bBackgroundNoise;
	int channels;
	int samples_per_sec;
	int bits_per_sample;
};
