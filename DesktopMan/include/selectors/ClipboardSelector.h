#pragma once

#include "IDesktopSelector.h"

class CClipboardSelector : public CEventSource<IDesktopSelectorEvents>,	public IDesktopSelector
{
public:
	CClipboardSelector(oms::omsContext* apContext);
	virtual ~CClipboardSelector();
	// ���������� IDesktopSelector
public:
	virtual bool Release()
	{
		MP_DELETE_THIS;
		return true;
	}

	// �������� ������� �������
	virtual bool StartSourceSelection()
	{
		return false;
	}

	// ����������� ������� �������
	virtual void CancelSourceSelection(){}
	// ���������� ��������� ���������� ����
	virtual IDesktopFrame* GetCurrentFrame();

	// ������ ������ ������ ����� � �������� �����������
	virtual IDesktopFrame* RecapFrame()
	{
		return NULL;
	}

	// ���������� ������� ������� �������
	virtual RECT GetCurrentSelectingRect();


	//���������� true ���� ������ ��������
	virtual bool IsCaptureAvailable() 
	{
		return true;
	}

	//���������� handle ���� � �������� ������������� ����������� ��� NULL � ��������� ������
	virtual HWND GetCapturedWindow()
	{
		return NULL;
	}

	//����� �� ���������� ������� � ������� ������ ������
	virtual bool CanShowCaptureRect()
	{
		return false;
	}

	//���������� ����� ����������� ������� (0-�����, 1-������������, 2-������������ �����������)
	virtual void SetShowCursorState(int state){}

	virtual bool IsLive()
	{
		return true;
	}

	virtual int GetType()
	{
		return CLIPBOARD_SELECTOR;
	}
	virtual void EmulateSelection( int aLeft, int aRight, int aTop, int aBottom){};
	virtual bool Run(){ return true;};
	void SetInteractiveBoardCapture(int aMonitorID){};
	void UpdateCurrentSelectingRect(){};
	void Seek( double aSeek){};
	void Play(){};
	void Pause(){};
	double GetDuration(){return 0;};

private:
	void ClearCapturedImage();

	CDesktopFrame*		currentFrame;
	oms::omsContext*	context;
};