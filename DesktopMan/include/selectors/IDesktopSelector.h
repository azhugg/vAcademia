#pragma once

const int RECT_SELECTOR = 1;
const int WND_SELECTOR = 2;
const int VIDEOIN_SELECTOR = 3;
const int INTERACTIVEBOARD_SELECTOR = 4;
const int DROPFILE_SELECTOR = 5;
const int CLIPBOARD_SELECTOR = 6;
const int VIDEOFILE_SELECTOR = 7;

struct IDesktopFrame
{
	virtual void			Release() = 0;
	virtual IDesktopFrame*	Clone() = 0;
	virtual unsigned int	GetWidth() = 0;
	virtual unsigned int	GetHeight() = 0;
	virtual unsigned int	GetDataSize() = 0;
	virtual HBITMAP	GetHBitmap() = 0;
	virtual void*	GetData() = 0;
};

struct IDesktopSelectorEvents
{
	virtual void OnSelectionCompleted(int resultCode) = 0;
	virtual void OnSelectionEscape() = 0;
	virtual void OnSetBarPosition( LPCSTR aTarget, std::vector<double> v1) = 0;
	virtual void OnVideoEnd( LPCSTR aTarget) = 0;
};

struct Resolution
{
	unsigned int width;
	unsigned int height;
}; 

struct IWebCameraSelector
{
	//��������� ����� ��������
	virtual int GetWebNumsCameraDevices() = 0;
	//��������� ���������� ���������
	virtual std::vector<Resolution> GetWebCameraResolutions(int numDevice) = 0;
	virtual std::string GetWebCameraName(int numDevice) = 0;
	virtual bool SetResolution(Resolution newResolution, int numDevice, bool reRun) = 0;
	virtual bool SetActiveDevice(int numDevice) = 0;
};

struct IWebCameraSelectorEvents
{
	virtual void OnSetResolutionCompleted(int resultCode) = 0;
};

struct IDesktopSelector
{
	// �������� ������� �������
	virtual bool Release() = 0;
	// �������� ������� �������
	virtual bool StartSourceSelection() = 0;
	// ����������� ������� �������
	virtual void CancelSourceSelection() = 0;
	// ���������� ��������� ���������� ����
	virtual IDesktopFrame* GetCurrentFrame() = 0;
	// ������ ������ ������ ����� � �������� �����������
	virtual IDesktopFrame* RecapFrame() = 0;
	// ���������� ������� ������� ������� � ���������� ����
	virtual RECT GetCurrentSelectingRect() = 0;
	//���������� true ���� ������ ��������
	virtual bool IsCaptureAvailable() = 0; 
	//���������� handle ���� � �������� ������������� ����������� ��� NULL � ��������� ������
	virtual HWND GetCapturedWindow() = 0;
	//����� �� ���������� ������� � ������� ������ ������
	virtual bool CanShowCaptureRect() = 0;
	//���������� ����� ����������� ������� (0-�����, 1-������������, 2-������������ �����������)
	virtual void SetShowCursorState(int state) = 0;
	//���������� ����� ������� � ������������� �����
	virtual void SetInteractiveBoardCapture(int aMonitorID) = 0;
	//
	virtual void UpdateCurrentSelectingRect() = 0;
	virtual bool IsLive() = 0;
	//���������� ��� ��������� - ������� / ���� � ��
	virtual int GetType() = 0;

	virtual void EmulateSelection( int aLeft, int aRight, int aTop, int aBottom) = 0;

	virtual bool Run() = 0;

	virtual void Seek( double aSeek)=0;
	virtual void Play()=0;
	virtual void Pause()=0;
	virtual double GetDuration() = 0;
};