#pragma once

#include "../lame/lame.h"

#ifdef HAVE_LIMITS_H
# include <limits.h>
#endif

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

#ifdef UINT_MAX
# define         MAX_U_32_NUM            UINT_MAX
#else
# define         MAX_U_32_NUM            0xFFFFFFFF
#endif

#define         Min(A, B)       ((A) < (B) ? (A) : (B))
#define         Max(A, B)       ((A) > (B) ? (A) : (B))

#define LR  0
#define MS  2


typedef enum sound_file_format_e 
{
	sf_unknown, 
	sf_raw, 
	sf_wave, 
	sf_aiff, 
	sf_mp1,  /* MPEG Layer 1, aka mpg */
	sf_mp2,  /* MPEG Layer 2 */
	sf_mp3,  /* MPEG Layer 3 */
	sf_ogg 
} sound_file_format;

#include "cthread.h"
#include "mutex.h"

class CFrameManager;
class CRecordState;

class CMP3Encoder : public CThread
{
public:
	CMP3Encoder( CComString aUserName);	
	virtual ~CMP3Encoder();

public:
	//void create( CRecordState *recordState);
	// �������� ������
	int addData( void *data, int length);	
	// ������� ��� ������
	void removeAllData();
	void destroy();
	// ���� �����������? True -��, ����� ���
	BOOL isBlockReady();
	// MP3-���� (�������) - ����� ������ ������ ����� blockNumber? True - ��, ����� ���
	BOOL isMP3FrameReady();

protected: 	
	// �������������
	int lameInit();
	// ��������� ���������� ������
	int setMP3Params( int channels, int samples_per_sec, int bits_per_sample);
	// ������� ���� ��� ������
	BOOL openMP3File( WCHAR *sFileName);
	// ������� ����� ���� ��� ������
	BOOL openNewMP3File( WCHAR *sFileName);
	// ����������� ������
	int encode_sample();
	// ���������� ������ ���� (�����)
	int setEmptySample();
	// ��������� �����������
	int encode_flush();
	// ������� ����
	void closeMP3File();
	//
	void lameClose();

private:
	//
	int  lame_set_stream_binary_mode ( FILE* const fp );
	//
	FILE * init_newoutfile( wchar_t *outPath);
	//
	FILE * init_outfile( wchar_t *outPath, int decode);
	// 
	int unpackReadSamples( const int samples_to_read, const int bytes_per_sample,
						const int swap_order, int *sample_buffer);
	//
	int readSamplesPcm( int sample_buffer[ 2304], int frame_size, int samples_to_read);
	//
	int getAudioCommon( int buffer[2][1152], short buffer16[2][1152]);
	//
	int getAudio( int buffer[2][1152]);

public:
	CFrameManager *getFrameManager();

private:	
	// ���-�� ������
	int frameCount;
	// ����� �������� �����
	int currentFrame;
	// �������� ������ - ������������ �������������� ������
	CFrameManager *frameManager;
	// �������� ������ - ������������ �������������� ������
	CFrameManager *frameManager2;
	//
	BOOL bFirst;
	// ����� ������ � ����
	FILE *outfile;
	// ����� ����� �� ����
	FILE *infile;
	// ��������� ���������� (������) �����������
	lame_global_flags *gf;
	// ������ �����
	sound_file_format input_format;
	// ���-�� ���
	int pcmbitwidth;
	//
	int pcmswapbytes;
	//
	int count_samples_carefully;
	// ���-�� ����������� �������
	int num_samples_read;
	// ������ ���� (�����)
	BOOL bEmpty;
	// mp3-�����
	unsigned char mp3buffer[ LAME_MAXMP3BUFFER];
	// ����� ������
	int Buffer[ 2][ 1152];
	// ����� ������, ������� ������������ � ����
	int blockNumber;

private:
	//	
	CMutex updateStateMutex;
	// TRUE: �������� �� ���������� ������
	BOOL bRunSaver;	

#ifdef _DEBUG
	CFrameManager *sampleFrameManager;
#endif
	// ��������� ������: ��������� ������
	CRecordState *recordState;
	//
	CComString m_sUserName;
	//
	BOOL bTerminating;
	BOOL bTerminated;
	// ���������, ��� ��������� ��� �����������
	BOOL bParamsSetted;

	// �������
	int m_iBitBrate;
	// �����: ���� ��� ������
	int m_iChannelMode;
	// ������� ��������
	int m_iQuality;
	// �������
	int m_iSampleRate;

protected:
	int encode();

public:
	BOOL setFileName( WCHAR *sFileName);
	// ��������� ���������� ������������� ����� � ���������
	void setParams( int channels, int sampleRate, int sampleSize);
	// ��������� ���������� ������ ������ � �������� ����
	int setMP3FileParams( int bit_brate, int iChannelMode, int iQuality, int iSampleRate);
	BOOL init( int channels, int sampleRate, int sampleSize, WCHAR *sFileName);
	BOOL clearSoundFileLog();
	BOOL run(  BOOL isBufferSaving = FALSE);
	BOOL save();
	BOOL isRecording();
	void setBlockNumber( int blockNumber);	
	WCHAR *getFileName();
	void Main();
	void Terminating();
	void startSaving();

	static BOOL getFileFullNameFromUuid( CWComString &tmpFileFullName, CWComString &tmpFileName, CWComString firstLine = L"", CWComString lastLine = L"");
	static BOOL getPathFromName( char *name, wchar_t *path);

public:
	static void turnOn();
	static void turnOff();
	static BOOL isTurn();
};
