#pragma once

//#include "TrayController.h"
#include "CaptureWindow.h"
#include "selectors/IDesktopSelector.h"
#include "view/DrawWindow.h"
#include <vector>

//////////////////////////////////////////////////////////////////////////
class	CDrawWindow;
class	CDesktopRectSelector;
class	CClipboardSelector;
class	CDropFileSelector;

struct	IDesktopSelector;

//////////////////////////////////////////////////////////////////////////
namespace oms
{
	class omsContext;
}

//////////////////////////////////////////////////////////////////////////
#define MAX_TEXTURE_SIZE	1024

//////////////////////////////////////////////////////////////////////////
class CDesktopManager : public IDesktopManager
						,	public IDesktopSelectorEvents
						,	public IPreviewWindowEvents
{
public:
	CDesktopManager(oms::omsContext* aContext);
	virtual ~CDesktopManager();

	// ���������� IDesktopSelectorEvents
public:
	void OnSelectionCompleted(int resultCode);
	void OnSelectionEscape();
	void OnSetBarPosition( LPCSTR aTarget, std::vector<double> v1){};
	void OnVideoEnd( LPCSTR aTarget){};

	// ���������� IDesktopManager
public:
	//! ������� ������
	virtual void Release();
	//! ��������� ������ ��� ��������� ������ ������
	void SetScreenCaptureObjectTarget( LPCSTR lpcTarget, float proportion = 1.33f);
	//! ��������� ������ ��� ��������� ����������� �� ������
	void SetScreenCaptureObjectTarget( LPCSTR lpcTarget, int mode, float proportion = 1.33f, LPCWSTR filePath = NULL );
	//! ��������� ���������� ��������������� �������
	bool AddCallback( desktopIDesktopManCallbacks* apCallback);
	//! ������� ���������� ��������������� �������
	bool RemoveCallback( desktopIDesktopManCallbacks* apCallback);
	bool Update();

	//! ��������� PopupMenu
public:
	void	StartDefineRegion();
	void	RecapSelection();
	void	CancelSelection();

	// ���������� IPreviewWindowEvents
public:
	void OnPreviewWindowOK(int aScaleMode);
	void OnPreviewWindowCancel();

protected:
	IDesktopSelector*		currentSelector;
	CDesktopRectSelector*	desktopRectSelector;
	CClipboardSelector*		clipboardRectSelector;
	CDropFileSelector*		dropFileSelector;
	void	ShowPreviewWindow(int aMode);

private:
	//CTrayController		trayController;
	oms::omsContext*	context;
	CDrawWindow*		pDrawWindow;
	MP_STRING			sTarget;
	float				fProportion;
	bool				isBuffer;
	int					currentMode;

	//! Callbacks
	typedef	MP_VECTOR<desktopIDesktopManCallbacks*>	VecCallbacks;
	VecCallbacks	vecCallbacks;
};