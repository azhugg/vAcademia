// Wave.h: interface for the CWave class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVE_H__54B7993A_3F57_4AB0_9CCD_A55C2A1B2871__INCLUDED_)
#define AFX_WAVE_H__54B7993A_3F57_4AB0_9CCD_A55C2A1B2871__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mmsystem.h>
#include <mmreg.h>
#include "WaveBuffer.h"	// Added by ClassView

//class CFrame;
//class CFrameManager;

class CWave
{	
public:
	CWave();
	CWave( const CWave& copy);
	CWave& operator = ( const CWave& wave);
	virtual ~CWave();

public:
	// ���������� �����
	void SetBuffer( void* pBuffer, DWORD dwNumSamples, bool bCopy = false);	
	// �������� ����� ������ � ������ �������� (8, 16, 32)
	DWORD GetBufferLength() const;
	// �������� ����� ������ ��� ����� ��������
	DWORD GetNumSamples() const;
	// ���������� �����
	void* GetBuffer() const;

	WAVEFORMATEX GetFormat() const;
	void BuildFormat( WORD nChannels, DWORD nFrequency, WORD nBits);
	
private:
	CWaveBuffer m_buffer;
	WAVEFORMATEX m_pcmWaveFormat;
};

#endif // !defined(AFX_WAVE_H__54B7993A_3F57_4AB0_9CCD_A55C2A1B2871__INCLUDED_)
