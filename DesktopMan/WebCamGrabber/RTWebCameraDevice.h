#pragma once

//#include "CaptureVideo.h"
#include ".\rtbasedevice.h"

class RTWebCameraDevice: public CRTBaseDevice 
{
public:
	RTWebCameraDevice( unsigned int auCameraIndex);
	~RTWebCameraDevice(void);
	
public:
	int PreparationForStart();
	void UpdateCameraIndex(unsigned int auCameraIndex);
	// ���������� ������� �������� � ����������
	HBITMAP	GetSnapshotFrame(bool isFirstFrame);
	// ������������� ������ �����������
	bool	SetFrameSize( unsigned int auWidth, unsigned int auHeight);

	vecResolutions&	GetSupportedResolutions()
	{
		if( !resolutionsHasEnumerated)
			EnumerateResolutions();
		return resolutions;
	}

	// ������� ���������� ���� � ����������� ����������
	HWND	CreateDialogPanel( HWND hParent);

	virtual int	StartImpl(HWND ahWnd);
	virtual void	StopImpl();

private:
//	CSampleDeviceDialog	dialog;
	CCaptureVideo		videoCap;
	unsigned int		m_uiCameraIndex;

	// resolution enumeration
	BOOL				resolutionsHasEnumerated;
	int					EnumerateResolutions();
	vecResolutions		resolutions;
};